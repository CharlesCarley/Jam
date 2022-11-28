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
#include "Interface/Areas/ProjectArea.h"
#include <qboxlayout.h>
#include <QApplication>
#include <QTreeWidget>
#include "Interface/Area/Area.h"
#include "Interface/Area/AreaToolbar.h"
#include "Interface/Areas/OutputArea.h"
#include "Interface/Constants.h"
#include "Interface/Dialogs/SettingsDialog.h"
#include "Interface/Events/EventTypes.h"
#include "Interface/Events/LayerSelectEvent.h"
#include "Interface/Extensions.h"
#include "Interface/Widgets/IconButton.h"
#include "Utils/StreamMethods.h"
#include "State/App.h"
#include "State/ProjectTags.h"
#include "State/ProjectManager.h"
#include "State/FrameStackManager.h"
#include "Xml/Node.h"

class QFileSystemWatcher;
class QTextEdit;

namespace Jam::Editor
{
    ProjectArea::ProjectArea(AreaCreator *creator, QWidget* parent) :
        Area(creator, AtProject, parent)
    {
        construct();
    }

    ProjectArea::~ProjectArea() = default;

    void ProjectArea::construct()
    {
        View::widgetDefaults(this);
        View::applyColorRoles(this, Const::AreaRole);

        const auto layout = new QVBoxLayout();
        View::layoutDefaults(layout);

        _tree = new QTreeWidget(this);
        View::treeWidgetDefaults(_tree, this);

        // add some extra space around the widget ?
        const auto margin = new QHBoxLayout();
        View::layoutDefaults(margin, Const::AreaPadding);

        margin->addWidget(_tree, 1);

        layout->addWidget(toolbar());
        layout->addLayout(margin, 1);

        // set via project
        setState();
        setLayout(layout);
    }

    void ProjectArea::constructToolbar()
    {
        const auto tool = toolbar();

        const auto iconSet =
            IconButton::createToolButton(Icons::Settings, this);

        connect(iconSet,
                &QPushButton::clicked,
                this,
                &ProjectArea::showProjectSettings);

        tool->addWidget(iconSet);
    }

    void ProjectArea::setState() const
    {
        if (!_tree) return;

        // Reconstruct the tree from the project state...
        if (const auto state = State::layerStack())
        {
            _tree->clear();
            (void)_tree->disconnect(this);

            QTreeWidgetItem* root = new QTreeWidgetItem();
            root->setText(0, "Project");
            root->setIcon(0, get(Icons::File));
            root->setData(0, Qt::UserRole, NoType);

            _tree->addTopLevelItem(root);

            connect(_tree,
                    &QTreeWidget::itemDoubleClicked,
                    this,
                    &ProjectArea::itemDoubleClicked);

            constructTree(state);
        }
    }

    void setForType(QTreeWidgetItem* item,
                    const LayerType  type,
                    const U32        index)
    {
        UFrag64 pair;
        pair.i[0] = type;
        pair.i[1] = index;

        switch (type)
        {
        case GridType:
            item->setText(0, "Grid");
            item->setIcon(0, get(Icons::GraphArea));
            item->setData(0, Qt::UserRole, pair.whole);
            break;
        case FunctionType:
            item->setText(0, "Function");
            item->setIcon(0, get(Icons::Cube));
            item->setData(0, Qt::UserRole, pair.whole);
            break;
        case NoType:
        default:
            break;
        }
    }

    void ProjectArea::constructTree(const State::FrameStackManager* stack) const
    {
        if (!stack)
            return;

        const LayerArray& layers = stack->layers();

        U32 i = 0;
        for (const auto layer : layers)
        {
            const auto item = new QTreeWidgetItem();
            setForType(item, layer->type, i++);


            _tree->addTopLevelItem(item);
        }
    }
    
    void ProjectArea::showProjectSettings()
    {
        SettingsDialog settings(this);
        settings.exec();
    }

    bool ProjectArea::event(QEvent* event)
    {
        switch ((int)event->type())
        {
        case ProjectClosed:
        case ProjectOpened:
            setState();
            break;
        default:
            break;
        }
        return false;
    }

    void ProjectArea::itemDoubleClicked(const QTreeWidgetItem* item, int column) const
    {
        if (const QVariant var = item->data(0, Qt::UserRole);
            var.isValid())
        {
            UFrag64 pair;
            pair.whole = var.toLongLong();
            QApplication::postEvent(
                window(),
                new LayerSelectEvent(
                    (LayerType)pair.i[0], pair.i[1]));
        }
    }

}  // namespace Jam::Editor
