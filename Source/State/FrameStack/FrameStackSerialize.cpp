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

namespace Jam
{
    using namespace Editor::State;
    using Xc = XmlConverter;
    using Sc = StringUtils;

    FrameStackSerialize::FrameStackSerialize(FrameStack* stack) :
        _stack{stack}
    {
    }

    void FrameStackSerialize::loadGrid(const XmlNode* root) const
    {
        const auto grid = new Layers::GridLayer();

        grid->setMajorGrid(0x2b2b2bFF);
        grid->setMinorGrid(0x212121FF);
        grid->setOrigin(0x4B4B4BFF);

        grid->setOrigin(Xc::toVec2F("origin", root, {0.f, 0}));
        grid->setAxis(Xc::toAxis("axis", root));

        _stack->addLayer(grid);
    }

    void FrameStackSerialize::loadFunction(const XmlNode* root) const
    {
        const auto fnc = new Layers::FunctionLayer();

        fnc->injectText(root->attribute("text", "y=x"));
        _stack->addLayer(fnc);
    }

    void FrameStackSerialize::load(IStream& stream) const
    {
        try
        {
            XmlFile fp(FrameStackTags,
                       FrameStackTagsMax);
            fp.read(stream);

            _stack->clear();

            if (const auto root = fp.root(FrameStackTag))
            {
                for (const auto node : root->children())
                {
                    if (node->isTypeOf(GridTag))
                        loadGrid(node);
                    else if (node->isTypeOf(FunctionTag))
                        loadFunction(node);
                }
            }

            JAM_ASSERT(_stack->layers().size() > 1)
        }
        catch (Exception& ex)
        {
            Con::println(ex.what());
        }
    }

    void saveGridLayer(
        XmlNode*                 root,
        const Layers::GridLayer* layer)
    {
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
        grid->insert("selected", layer->isSelected() ? "true" : "false");
        root->addChild(grid);
    }

    void saveFunctionLayer(
        XmlNode*                     root,
        const Layers::FunctionLayer* layer)
    {
        XmlNode* func = new XmlNode("function", FunctionTag);

        func->insert("text", layer->getText());

        root->addChild(func);
    }

    void FrameStackSerialize::save(OStream& out) const
    {
        XmlNode* root = new XmlNode("stack", FrameStackTag);

        saveGridLayer(root, _stack->cast<Layers::GridLayer>(0));

        saveFunctionLayer(root, _stack->cast<Layers::FunctionLayer>(1));

        Xc::toStream(out, root, 4);
        delete root;
    }
}  // namespace Jam
