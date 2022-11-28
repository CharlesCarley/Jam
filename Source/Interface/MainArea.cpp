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
#include "Interface/MainArea.h"
#include "Area/AreaCreator.h"
#include "Interface/Area/Area.h"
#include "Interface/Area/AreaBranch.h"
#include "Interface/Area/AreaContent.h"
#include "Interface/Area/AreaLeaf.h"
#include "Interface/Area/AreaNode.h"
#include "Interface/Areas/OutputArea.h"
#include "Interface/Events/BranchEvent.h"
#include "Interface/Extensions.h"
#include "State/ProjectTags.h"
#include "State/ProjectManager.h"
#include "Utils/XmlConverter.h"
#include "Xml/File.h"
#include "Xml/Node.h"
#include "MainArea.Creator.inl"

namespace Jam::Editor
{

    MainArea::MainArea(const QString& layout, QWidget* parent) :
        QWidget(parent),
        _creator(new MainAreaCreator())
    {
        if (layout.isEmpty())
            construct();
        else
            construct(layout);
    }

    MainArea::~MainArea()
    {
        delete _layout;
        _layout = nullptr;

        delete _creator;
        _creator = nullptr;
    }

    void MainArea::construct()
    {
        View::widgetDefaults(this);
        View::applyColorRoles(this);

        _layout = new QVBoxLayout();
        View::layoutDefaults(_layout);

        _root = new AreaNode(_creator);
        _root->setArea(_creator->fromType(0));

        _layout->addWidget(_root);
        setLayout(_layout);
    }

    void MainArea::construct(const QString& layout)
    {
        try
        {
            XmlFile parser(State::AreaLayoutTags,
                           State::AreaLayoutTagsMax);

            StringStream ss;
            ss << layout.toStdString();
            parser.read(ss);
            if (const auto root = parser.root(State::TreeTag))
                construct(root);
            else
                handleBuildError("invalid root node");
        }
        catch (Exception& ex)
        {
            handleBuildError(ex.what());
        }
    }

    void MainArea::construct(const XmlNode* node)
    {
        // Note: used only as an extension of construct(const QString& layout)

        _layout = new QVBoxLayout();
        View::layoutDefaults(_layout);

        _root = new AreaNode(_creator);

        if (node)
        {
            if (!node->children().empty())
            {
                applyNode(
                    node->children().front(),  // current
                    nullptr,                   // no parent
                    _root,                     // dest
                    2,                         // type
                    0                          // ori
                );
            }
            else
                _root->setArea(_creator->fromType(0));
        }

        _layout->addWidget(_root);

        if (_root->hasContent())
            _root->content()->updateMask();

        setLayout(_layout);
    }


    void MainArea::clear()
    {
        delete _root;
        delete _layout;
        StringStream ss;
        ss << R"(<tree>)";
        ss << R"( <branch ratio="0.0">)";
        ss << R"(  <leaf type="1"/>)";
        ss << R"(  <branch ratio="0.8">)";
        ss << R"(   <branch ratio="1" orientation="2">)";
        ss << R"(    <leaf type="2"/>)";
        ss << R"(    <leaf type="0"/>)";
        ss << R"(   </branch>)";
        ss << R"(   <branch ratio="0.25" orientation="2">)";
        ss << R"(    <leaf type="4"/>)";
        ss << R"(    <leaf type="3"/>)";
        ss << R"(   </branch>)";
        ss << R"(  </branch>)";
        ss << R"( </branch>)";
        ss << R"(</tree>)";
        construct(QString::fromStdString(ss.str()));
    }



    bool MainArea::event(QEvent* event)
    {
        // TODO: Relay events down the tree here
        if (event)
        {
            switch ((int)event->type())
            {
            case ProjectChanged:
            case ProjectOpened:
            case ProjectClosed:
            case LayerSelect:
                if (_root)
                    _root->propagate(event);
                return false;
            default:
                break;
            }
        }

        // nothing was handled so, send
        // it back up the widget tree..
        return QWidget::event(event);
    }

    void MainArea::notify(QEvent* evt)
    {
        if (evt)
            this->event(evt);
    }

    ScopePtr<XmlNode*> MainArea::serialize()
    {
        if (!_root)
        {
            Console::writeLine(
                "MainArea->serialize was called "
                "without a valid root node to serialize.");
            return ScopePtr(new XmlNode());
        }

        const auto root = new XmlNode("tree", State::TreeTag);
        serialize(_root, root);
        return ScopePtr(root);
    }

    void MainArea::serialize(String& dest)
    {
        ScopePtr<XmlNode*> root = serialize();
        XmlConverter::toString(dest, root.ref(), 4);
    }

    void MainArea::serialize(const AreaNode* source, XmlNode* destination)
    {
        if (!source)
        {
            Log::writeLine("serialize: Invalid source area node");
            return;
        }

        if (!destination)
        {
            Log::writeLine("serialize: Invalid destination xml node");
            return;
        }

        if (source->isLeaf())
        {
            if (const AreaLeaf* areaLeaf = (AreaLeaf*)source->content())
            {
                if (const Area* area = areaLeaf->contents())
                {
                    XmlNode* leafTag = new XmlNode("leaf", State::LeafTag);
                    leafTag->insert("type", area->type());

                    // <leaf type="x" />
                    destination->addChild(leafTag);
                }
                else
                    Log::writeLine("serialize: Missing area content in the supplied source area node");
            }
            else
                Log::writeLine("serialize: Missing content in the supplied area node");
        }
        else
        {
            if (const AreaBranch* branch = (AreaBranch*)source->content())
            {
                XmlNode* branchTag = new XmlNode("branch", State::BranchTag);
                branchTag->insert("ratio", branch->ratio());
                branchTag->insert("orientation", branch->orientation());

                // <branch ratio=[0..1] orientation="1|2">
                //  <... recursive left/>
                //  <... recursive right/>
                // </branch
                destination->addChild(branchTag);

                serialize(branch->left(), branchTag);
                serialize(branch->right(), branchTag);
            }
            else
                Log::writeLine("serialize: Missing content in the supplied area node");
        }
    }

    bool MainArea::validateType(int& dest, XmlNode* fromNode)
    {
        dest = 0;
        if (fromNode && fromNode->contains("type"))
        {
            if (auto value = fromNode->integer("type");
                value >= 0 && value < AtMax)
            {
                dest = (AreaType)value;
                return true;
            }
        }
        return false;
    }

    void MainArea::handleBuildError(const char* message)
    {
        construct();
        Log::writeLine(message);
    }

    void MainArea::dumpDisplayTree()
    {
        String data;
        serialize(data);
        Log::writeLine(data);
    }


    void MainArea::applyNode(XmlNode*        node,
                             const AreaNode* parent,
                             AreaNode*       current,
                             const int       type,
                             const int       ori)
    {
        if (!node || !current)  // parent is the only node allowed to be null
            return;

        if (node->isTypeOf(State::LeafTag))
            applyLeaf(node, parent, current, type, ori);
        else if (node->isTypeOf(State::BranchTag))
            applyBranch(node, current);
        else
        {
            handleBuildError(
                "unknown node type "
                "found when building the layout");
        }
    }

    void MainArea::applyLeaf(XmlNode*        node,
                             const AreaNode* parent,
                             AreaNode*       current,
                             const int       type,
                             const int       ori)
    {
        if (int areaType; validateType(areaType, node))
        {
            current->setArea(_creator->fromType(areaType),
                             AreaEdgeRect::computeMask(parent, current, type, ori));
        }
        else
            handleBuildError("Area type index out of range");
    }

    void MainArea::applyBranch(XmlNode* node, AreaNode* parent)
    {
        if (node->children().size() <= 2)
        {
            const int64_t ori = node->integer("orientation",
                                              Qt::Orientation::Horizontal);

            AreaNode* left  = new AreaNode(_creator, parent);
            AreaNode* right = new AreaNode(_creator, parent);

            AreaBranch* branch = new AreaBranch(
                (Qt::Orientation)ori,
                left,
                right);

            branch->setRatio(node->float64("ratio", 0.5));

            parent->setContent(branch);

            applyNode(node->at(0), parent, left, 0, (int)ori);
            applyNode(node->at(1), parent, right, 1, (int)ori);
        }
        else
        {
            Console::writeLine(
                "ignoring branch node "
                "with less than two children");
        }
    }
}  // namespace Jam::Editor
