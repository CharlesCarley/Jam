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
#include "Interface/Dialogs/SettingsDialog.h"
#include <QBoxLayout>
#include <QTreeWidget>
#include "Interface/Areas/OutputArea.h"
#include "Interface/Constants.h"
#include "Interface/Dialogs/SettingsDialogProjectPage.h"
#include "Interface/Dialogs/SettingsDialogRootPage.h"
#include "Interface/Dialogs/SettingsDialogRunPage.h"
#include "Interface/Extensions.h"
#include "Interface/Widgets/OkCancelWidget.h"
#include "Interface/Widgets/WindowTitlebar.h"

namespace Jam::Editor
{
    SettingsDialog::SettingsDialog(QWidget* parent) :
        Dialog("Settings", Close | OkCancel | Maximize, parent)
    {
        construct();
    }

    SettingsDialog::~SettingsDialog() = default;

    void SettingsDialog::construct()
    {
        _state = PersistentSettings().copy();
        View::applyColorRoles(this, Const::AreaRole);

        _settingTree = new QTreeWidget();

        // copies down the color roles from this to to the tree.
        View::treeWidgetDefaults(_settingTree, this);

        _settingTree->setMaximumWidth(120);
        _settingTree->setMinimumWidth(120);
        _settingTree->addTopLevelItem(constructTree());

        _settingsLayout = new QHBoxLayout();
        View::layoutDefaults(_settingsLayout, 7, 7);

        _settingsLayout->addWidget(_settingTree, 0);
        showProject();

        selectOption(SettingProject);

        // apply the layout to the center widget
        // in the Dialog base class.
        applyLayout(_settingsLayout);

        setModal(true);

        connect(_settingTree,
                &QTreeWidget::itemClicked,
                this,
                &SettingsDialog::selectionChanged);

        connect(this,
                &Dialog::okClicked,
                this,
                &SettingsDialog::okClicked);

        connect(this,
                &Dialog::cancelClicked,
                this,
                &SettingsDialog::cancelClicked);

        _settingTree->expandAll();
        show();
    }

    void SettingsDialog::selectOption(const Option opt)
    {
        if (_settingTree)
        {
            if (QTreeWidgetItem* item = findItem(opt))
            {
                _settingTree->setCurrentItem(item);
                selectionChanged(item, 0);
            }
        }
    }

    bool SettingsDialog::isTypeOf(const QTreeWidgetItem* item, const Option opt)
    {
        return item && item->data(0, Qt::UserRole).toInt() == opt;
    }

    QTreeWidgetItem* SettingsDialog::findItem(const Option item) const
    {
        if (_settingTree)
        {
            if (QTreeWidgetItem* cur = _settingTree->topLevelItem(0))
            {
                if (isTypeOf(cur, item))
                    return cur;
                for (int i = 0; i < cur->childCount(); ++i)
                {
                    if (QTreeWidgetItem* find = cur->child(i))
                    {
                        if (isTypeOf(find, item))
                            return find;
                    }
                }
            }
        }
        return nullptr;
    }

    void SettingsDialog::selectionChanged(const QTreeWidgetItem* item, int col)
    {
        if (item)
        {
            switch (item->data(0, Qt::UserRole).toInt())
            {
            case SettingProject:
                showProject();
                break;
            case SettingRun:
                showRun();
                break;
            default:
            case SettingRoot:
                showRoot();
                break;
            }
        }
    }

    void SettingsDialog::okClicked()
    {
        PersistentSettings().apply(_state);
        close();
    }

    void SettingsDialog::cancelClicked()
    {
        close();
    }

    QTreeWidgetItem* SettingsDialog::constructTree()
    {
        auto* rootItem = new QTreeWidgetItem({"Settings"});
        rootItem->setData(0, Qt::UserRole, SettingRoot);

        auto* projectItem = new QTreeWidgetItem();
        projectItem->setText(0, "Project");
        projectItem->setData(0, Qt::UserRole, SettingProject);

        auto* runItem = new QTreeWidgetItem();
        runItem->setText(0, "Run");
        runItem->setData(0, Qt::UserRole, SettingRun);

        rootItem->addChild(projectItem);
        rootItem->addChild(runItem);

        rootItem->setExpanded(true);
        projectItem->setSelected(true);
        return rootItem;
    }

    void SettingsDialog::updateTree(QWidget* widget) const
    {
        if (widget)
        {
            if (_settingsLayout->count() == 2)
            {
                const auto item = _settingsLayout->takeAt(1);
                delete item->widget();
                delete item;
                _settingsLayout->addWidget(widget);
            }
            else
                _settingsLayout->addWidget(widget);
        }
    }

    QWidget* SettingsDialog::createPanel() const
    {
        const auto panel = new QWidget();
        panel->setMinimumSize(200, 200);
        View::widgetDefaults(panel);
        return panel;
    }

    void SettingsDialog::showProject()
    {
        updateTree(new SettingsDialogProjectPage(&_state, this));
    }

    void SettingsDialog::showRun()
    {
        updateTree(new SettingsDialogRunPage(&_state, this));
    }

    void SettingsDialog::showRoot()
    {
        updateTree(new SettingsDialogRootPage(&_state, this));
    }

}  // namespace Jam::Editor
