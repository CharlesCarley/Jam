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
#include "Interface/Style/MainStyle.h"
#include <QApplication>
#include <QPainter>

#include "Interface/Areas/OutputArea.h"
#include "Utils/String.h"
#include "Interface/Style/Palette.h"
#include "Interface/Style/PaletteCache.h"
#include "Interface/Style/Render.h"
#include "Interface/Constants.h"

namespace Jam::Editor
{
    MainStyle::MainStyle() :
        QProxyStyle()
    {
        Const::initializePalette();
        _cache = new PaletteCache();
    }

    MainStyle::~MainStyle()
    {
        delete _cache;
        _cache = nullptr;
    }

    void MainStyle::drawControl(
        const ControlElement element,
        const QStyleOption*  option,
        QPainter*            painter,
        const QWidget*       widget) const
    {
        const Const::Renderer renderer(_cache, this, option, painter, widget);
        switch (element)
        {
        case CE_MenuBarEmptyArea:
        case CE_MenuEmptyArea:
            renderer.fill();
            break;
        case CE_CheckBox:
            renderer.checkBox();
            break;
        case CE_PushButton:
            renderer.pushButton();
            break;
        case CE_MenuBarItem:
            renderer.menuBarItem();
            break;
        case CE_MenuItem:
            renderer.menuItem();
            break;
        case CE_ItemViewItem:
            renderer.panel();
            break;
        case CE_TabBarTab:
            renderer.fill();
            break;
        case CE_ShapedFrame:
            break;
        case CE_DockWidgetTitle:
        case CE_ScrollBarAddLine:
        case CE_ScrollBarSubLine:
        case CE_ScrollBarAddPage:
        case CE_ScrollBarSubPage:
        case CE_ScrollBarSlider:
        case CE_ScrollBarFirst:
        case CE_ScrollBarLast:
        case CE_PushButtonBevel:
        case CE_PushButtonLabel:
        case CE_MenuVMargin:
        case CE_MenuHMargin:
        case CE_MenuScroller:
        case CE_MenuTearoff:
        case CE_CheckBoxLabel:
        case CE_RadioButton:
        case CE_RadioButtonLabel:
        case CE_TabBarTabShape:
        case CE_TabBarTabLabel:
        case CE_ProgressBar:
        case CE_ProgressBarGroove:
        case CE_ProgressBarContents:
        case CE_ProgressBarLabel:
        case CE_ToolButtonLabel:
        case CE_Header:
        case CE_HeaderSection:
        case CE_HeaderLabel:
        case CE_ToolBoxTab:
        case CE_SizeGrip:
        case CE_Splitter:
        case CE_RubberBand:
        case CE_FocusFrame:
        case CE_ComboBoxLabel:
        case CE_ToolBar:
        case CE_ToolBoxTabShape:
        case CE_ToolBoxTabLabel:
        case CE_HeaderEmptyArea:
        case CE_ColumnViewGrip:
        case CE_CustomBase:
        default:
            Log::writeLine("CE->Unhandled : ", element);
            renderer.fillRect(option->rect, Const::Empty);
            break;
        }
    }

    void MainStyle::drawPrimitive(
        const PrimitiveElement element,
        const QStyleOption*    option,
        QPainter*              painter,
        const QWidget*         widget) const
    {
        const Const::Renderer renderer(_cache, this, option, painter, widget);

        switch (element)
        {
        case PE_FrameTabWidget:
        case PE_PanelMenu:
        case PE_FrameMenu:
        case PE_Frame:
        case PE_IndicatorBranch:
        case PE_PanelItemViewRow:
            renderer.fill();
            break;
        case PE_FrameFocusRect:
            renderer.fillRect(option->rect, QColor(0xFF, 0x00, 0xFF));
            break;
        case PE_PanelLineEdit:
            renderer.fillLineEdit();
            break;
        case PE_PanelItemViewItem:
        case PE_Widget:
        case PE_FrameDefaultButton:
        case PE_FrameDockWidget:
        case PE_FrameGroupBox:
        case PE_FrameLineEdit:
        case PE_FrameStatusBarItem:
        case PE_FrameWindow:
        case PE_FrameButtonBevel:
        case PE_FrameButtonTool:
        case PE_FrameTabBarBase:
        case PE_PanelButtonCommand:
        case PE_PanelButtonBevel:
        case PE_PanelButtonTool:
        case PE_PanelMenuBar:
        case PE_PanelToolBar:
        case PE_IndicatorArrowDown:
        case PE_IndicatorArrowLeft:
        case PE_IndicatorArrowRight:
        case PE_IndicatorArrowUp:
        case PE_IndicatorButtonDropDown:
        case PE_IndicatorItemViewItemCheck:
        case PE_IndicatorCheckBox:
        case PE_IndicatorDockWidgetResizeHandle:
        case PE_IndicatorHeaderArrow:
        case PE_IndicatorMenuCheckMark:
        case PE_IndicatorProgressChunk:
        case PE_IndicatorRadioButton:
        case PE_IndicatorSpinDown:
        case PE_IndicatorSpinMinus:
        case PE_IndicatorSpinPlus:
        case PE_IndicatorSpinUp:
        case PE_IndicatorToolBarHandle:
        case PE_IndicatorToolBarSeparator:
        case PE_PanelTipLabel:
        case PE_IndicatorTabTear:
        case PE_PanelScrollAreaCorner:
        case PE_IndicatorColumnViewArrow:
        case PE_IndicatorItemViewItemDrop:
        case PE_PanelStatusBar:
        case PE_IndicatorTabClose:
        case PE_IndicatorTabTearRight:
        case PE_CustomBase:
        default:
            Log::writeLine("PE->Unhandled : ", element);
            //renderer.fillRect(option->rect, QColor(0xFF, 0x00, 0xFF));
            break;
        }
    }

    void MainStyle::drawComplexControl(
        const ComplexControl       control,
        const QStyleOptionComplex* option,
        QPainter*                  painter,
        const QWidget*             widget) const
    {
        const Const::Renderer renderer(_cache, this, option, painter, widget);

        switch (control)
        {
        case CC_ScrollBar:
            renderer.scrollBar();
            break;
        case CC_Slider:
            renderer.slider();
            break;
        case CC_SpinBox:
            renderer.spinBox();
            break;
        case CC_ComboBox:
        case CC_ToolButton:
        case CC_TitleBar:
        case CC_Dial:
        case CC_GroupBox:
        case CC_MdiControls:
        case CC_CustomBase:
        default:
            Log::writeLine("CC->Unhandled : ", control);
            // renderer.fillRect(option->rect, QColor(0xFF, 0x00, 0xFF));
            break;
        }
    }

    int MainStyle::styleHint(const StyleHint     hint,
                             const QStyleOption* option,
                             const QWidget*      widget,
                             QStyleHintReturn*   returnData) const
    {
        if (hint == SH_ScrollBar_Transient)
            return 1;

        return QProxyStyle::styleHint(hint, option, widget, returnData);
    }

    void MainStyle::initialize()
    {
        // Ownership of the style object is transferred to QApplication,
        // so QApplication will delete the style object on application exit
        // or when a new style is set and the old style is still the parent
        // of the application object.
        // https://doc.qt.io/qt-6/qapplication.html#setStyle
        QApplication::setStyle(new MainStyle());
    }

}  // namespace Jam::Editor
