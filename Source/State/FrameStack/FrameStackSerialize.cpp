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
    using namespace Editor::State;
    using Xc = XmlConverter;
    using Sc = StringUtils;

    FrameStackSerialize::FrameStackSerialize(FrameStack* stack) :
        _stack{stack}
    {
    }

    GridLayer* FrameStackSerialize::loadGrid(const XmlNode* root) const
    {
        const auto grid = new GridLayer();

        grid->setMajorGrid(0x2b2b2bFF);
        grid->setMinorGrid(0x212121FF);
        grid->setOrigin(0x4B4B4BFF);

        grid->setOrigin(Xc::toVec2F("origin", root, {0.f, 0}));
        grid->setAxis(Xc::toAxis("axis", root));

        return grid;
    }

    FunctionLayer* FrameStackSerialize::loadFunction(const XmlNode* root)
    {
        const auto fnc = new FunctionLayer();

        for (const auto& node : root->children())
        {
            if (node->isTypeOf(ExpressionTag))
            {
                ExpressionStateObject* eso = fnc->createExpression();
                eso->text                  = node->attribute("text");
            }
            else if (node->isTypeOf(VariableTag))
            {
                VariableStateObject* vso = fnc->createVariable();

                vso->name = node->attribute("name");
            }
            else
            {
                // quiet error
            }
        }

        return fnc;
    }

    void FrameStackSerialize::load(IStream& stream) const
    {
        try
        {
            XmlFile fp(FrameStackTags,
                       FrameStackTagsMax);
            fp.read(stream);

            GridLayer*     grid = nullptr;
            FunctionLayer* func = nullptr;

            if (const auto root = fp.root(FrameStackTag))
            {
                for (const auto node : root->children())
                {
                    if (node->isTypeOf(GridTag))
                    {
                        if (grid)
                            throw Exception("multiple grid layers");

                        grid = loadGrid(node);
                    }
                    else if (node->isTypeOf(FunctionTag))
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
            if (!grid || !func)
                throw Exception("missing grid or function layers");

            _stack->clear();
            _stack->addLayer(grid);
            _stack->addLayer(func);
        }
        catch (Exception& ex)
        {
            Con::println(ex.what());
        }
    }

    void FrameStackSerialize::saveGrid() const
    {
        const auto layer = _stack->cast<GridLayer>(0);

        const Vec2F& o  = layer->origin();
        const Axis&  ax = layer->axis();

        XmlNode* grid = new XmlNode("grid", GridTag);

        grid->insert("origin",
                     Sc::join(ValueSetF({o.x, o.y}, 0, 6)));

        grid->insert("axis",
                     Sc::join(ValueSetU({
                         ax.x.n(),
                         ax.x.d(),
                         ax.y.n(),
                         ax.y.d(),
                     })));

        _root->addChild(grid);
    }

    void FrameStackSerialize::saveFunction() const
    {
        const auto layer = _stack->cast<FunctionLayer>(1);

        XmlNode* func = new XmlNode("function", FunctionTag);

        for (const auto id : layer->objects())
        {
            if (id->type == FstExpression)
            {
                const ExpressionStateObject* eso = (ExpressionStateObject*)id;

                XmlNode* expr = new XmlNode("expression", ExpressionTag);
                expr->insert("text", eso->text);
                func->addChild(expr);
            }
            else if (id->type == FstVariable)
            {
                const VariableStateObject* vso = (VariableStateObject*)id;

                XmlNode* expr = new XmlNode("variable", VariableTag);

                expr->insert("name", vso->name);
                expr->insert("range",
                             Sc::join(ValueSetF({vso->range.x, vso->range.y}, 0, 6)));
                func->addChild(expr);
            }
        }
        _root->addChild(func);
    }

    void FrameStackSerialize::save(OStream& out)
    {
        _root = new XmlNode("stack", FrameStackTag);
        saveGrid();
        saveFunction();
        Xc::toStream(out, _root, 4);
        delete _root;
        _root = nullptr;
    }
}  // namespace Jam::Editor::State
