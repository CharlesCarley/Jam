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
#include "Interface/Style/Icons.h"

class QCheckBox;
class QLabel;
class QLineEdit;
class QLayout;
class QPlainTextEdit;
class QPushButton;
class QBoxLayout;
class QHBoxLayout;
class QVBoxLayout;
class QTreeWidget;

namespace Jam::Editor
{

    enum StyleTypes
    {
        TransparentStyle,
        ApplicationStyle,
        AreaStyle,
        AreaEdgeRectStyle,
        AreaNodeStyle,
        AreaToolBarStyle,
        AreaGridStyle,
        AreaProjectTreeStyle,
        AreaSettingsTreeStyle,
        AreaDialogStyle,
        AreaDialogContainerStyle,
        AreaSliderStyle,
        AreaMenuItemStyle,
        AreaToolMenuItemStyle,
        AreaToolLabelStyle,
        AreaToolButtonStyle,
        AreaLabelStyle,
        AreaPushButtonStyle,
        MenuBarStyle,
        AreaLineStyle,
        AreaDarkLineStyle,
        QCheckBoxStyle,
    };

    enum Hints
    {
        SplitterSizeHint,
        ButtonWidth,
        ButtonHeight,
        PushButtonWidth,
        PushButtonHeight,

    };

    class Style
    {
    private:
        static void treeWidgetDefaults(QTreeWidget* widget);
        static void pushButtonDefaults(QWidget* widget);

    public:
        static void apply(QWidget* widget, StyleTypes type);
        static int  hint(Hints type);

        static void sliderPalette(QPalette& palette);

        static QTreeWidget* treeWidget(const StyleTypes type, QWidget* parent = nullptr);

        static void layoutDefaults(QLayout* widget, int margin = 0, int spacing = 0);
        static void buttonDefaults(QWidget* widget);

        static QVBoxLayout* verticalLayout(int margin = 0, int spacing = 0, QWidget* parent = nullptr);

        static QHBoxLayout* horizontalLayout(int margin = 0, int spacing = 0, QWidget* parent = nullptr);

        static QHBoxLayout* paddedArea(QWidget* child, int margin = 2);

        static QPushButton* toolButton(Icons::Icon ico, QWidget* parent = nullptr);

        static QPlainTextEdit* plainText(QWidget* parent = nullptr);

        static QLineEdit* line(QWidget* parent = nullptr);

        static QLineEdit* darkLine(QWidget* parent = nullptr);

        static QPushButton* button(const QString& label, QWidget* parent = nullptr);

        static QLabel* text(const QString& label, QWidget* parent = nullptr);

        static QCheckBox* checkBox(const QString& label, bool initialState);

    };

}  // namespace Jam::Editor
