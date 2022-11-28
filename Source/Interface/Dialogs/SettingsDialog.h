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
#pragma once

#include <QFileDialog>
#include "Dialog.h"
#include "Interface/PersistentSettings.h"

class QTreeWidgetItem;
class QHBoxLayout;
class QTreeWidget;
class QLineEdit;

namespace Jam::Editor
{
    class SettingsDialog final : public Dialog
    {
        Q_OBJECT
    public:
        enum Option
        {
            SettingRoot = 1,
            SettingProject,
            SettingRun,
        };

    private:
        QTreeWidget*      _settingTree{nullptr};
        QHBoxLayout*      _settingsLayout{nullptr};
        SettingsDataState _state;

        void construct();

        QTreeWidgetItem* findItem(Option item) const;

        void updateTree(QWidget* widget) const;

        QWidget* createPanel() const;

        void showProject();

        void showRun();

        void showRoot();

        void selectionChanged(const QTreeWidgetItem* item, int col);

        void okClicked();

        void cancelClicked();

        static QTreeWidgetItem* constructTree();

        static bool isTypeOf(const QTreeWidgetItem* item, Option opt);

    public:
        explicit SettingsDialog(QWidget* parent = nullptr);
        ~SettingsDialog() override;

        void selectOption(Option opt);
    };
}  // namespace Jam::Editor
