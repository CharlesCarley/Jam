/*
-------------------------------------------------------------------------------
    Copyright (c) Charles Carley.

  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
-------------------------------------------------------------------------------
*/
#include "FrameStackSerialize.h"
#include "State/FrameStack/FunctionLayer.h"
#include "State/ProjectManager.h"
#include "State/ProjectTags.h"
#include "Utils/XmlConverter.h"
#include "Xml/Declarations.h"
#include "Xml/File.h"

namespace Jam::Editor::State
{
    using namespace Tags;

    using Xc = XmlConverter;
    using Sc = StringUtils;

    FrameStackSerialize::FrameStackSerialize(FrameStack* stack) :
        _stack{stack}
    {
    }

    GridLayer* FrameStackSerialize::loadGrid(const XmlNode* root)
    {
        const auto grid = new GridLayer();
        grid->setMajorGrid(0x2b2b2bFF);
        grid->setMinorGrid(0x212121FF);
        grid->setOrigin(0x4B4B4BFF);

        // grid->setOrigin(Xc::toVec2F("origin", root, {0.f, 0}));
        // grid->setAxis(Xc::toAxis("axis", root));
        return grid;
    }

    FunctionLayer* FrameStackSerialize::loadFunction(const XmlNode* root)
    {
        const auto fnc = new FunctionLayer();

        for (const auto& node : root->children())
        {
            if (node->isTypeOf(ExpressionTagId))
            {
                ExpressionStateObject* eso = fnc->createExpression();

                eso->setText(node->attribute("text"));
            }
            else if (node->isTypeOf(VariableTagId))
            {
                VariableStateObject* vso = fnc->createVariable();

                vso->setName(node->attribute("name"));
                vso->setRange(Xc::toVec2F("range", node, {-10.f, 10}));
                vso->setRate(node->float32("rate", 1));
                vso->setValue(node->float32("value", 1));
            }
        }

        return fnc;
    }

    void FrameStackSerialize::load(IStream& stream) const
    {
        try
        {
            XmlFile fp(FrameStackTags, FrameStackTagsMax);
            fp.read(stream);

            GridLayer*     grid = nullptr;
            FunctionLayer* func = nullptr;

            if (const auto root = fp.root(FrameStackTagId))
            {
                for (const auto node : root->children())
                {
                    if (node->isTypeOf(GridTagId))
                    {
                        if (grid)
                            throw Exception("multiple grid layers");

                        grid = loadGrid(node);
                    }
                    else if (node->isTypeOf(FunctionTagId))
                    {
                        if (func)
                            throw Exception("multiple function layers");

                        func = loadFunction(node);
                    }
                }
            }

            // order here is important
            // Grid = idx0
            // Func = idx1
            if (!grid)
                grid = new GridLayer();
            if (!func)
                func = new FunctionLayer();

            _stack->clear();
            _stack->addLayer(grid);
            _stack->addLayer(func);
            _stack->update();
        }
        catch (Exception& ex)
        {
            Con::println(ex.what());
        }
    }

    void FrameStackSerialize::saveGrid() const
    {
        const auto layer = _stack->cast<GridLayer>(0);
        XmlNode*   grid  = new XmlNode(GridTag);

        // TODO: Save grid control properties, not viewport information...
        _root->addChild(grid);
    }

    void FrameStackSerialize::saveFunction() const
    {
        const auto layer = _stack->cast<FunctionLayer>(1);

        const auto func = new XmlNode(FunctionTag);

        for (const auto id : layer->objects())
        {
            if (id->type() == FstExpression)
            {
                const ExpressionStateObject* eso = (ExpressionStateObject*)id;

                const auto expr = new XmlNode(ExpressionTag);

                expr->insert("text", eso->text());
                func->addChild(expr);
            }

            else if (id->type() == FstVariable)
            {
                const VariableStateObject* vso = (VariableStateObject*)id;

                XmlNode* expr = new XmlNode(VariableTag);

                expr->insert("name", vso->name());
                expr->insert("range",
                             Sc::join(ValueSetF({vso->range().x, vso->range().y}, 0, 6)));
                expr->insert("rate",
                             Sc::join(FloatPrint(vso->rate())));
                expr->insert("value",
                             Sc::join(FloatPrint(vso->value())));
                func->addChild(expr);
            }
        }
        _root->addChild(func);
    }

    void FrameStackSerialize::save(OStream& out)
    {
        _root = new XmlNode(FrameStackTag);

        saveGrid();
        saveFunction();

        Xc::toStream(out, _root, 4);

        delete _root;
        _root = nullptr;
    }
}  // namespace Jam::Editor::State
