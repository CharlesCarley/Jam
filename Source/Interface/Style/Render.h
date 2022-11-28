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

#include <QStyleOption>
#include "Interface/Style/MainStyle.h"

class QCheckBox;
class QStyleOption;
class QWidget;

namespace Jam::Editor::Const
{
    class Renderer
    {
    private:
        PaletteCache*       _cache;
        const QStyle*       _style;
        const QStyleOption* _option;
        QPainter*           _painter;
        const QWidget*      _widget;
        mutable QPoint      _margin{0, 0};

        inline const QStyleOptionButton* buttonOption() const;

        inline const QStyleOptionMenuItem* menuOption() const;

        inline const QStyleOptionViewItem* viewItemOption() const;

        inline const QStyleOptionComplex* complexOption() const;

        inline const QStyleOptionSlider* sliderOption() const;

        inline const QCheckBox* checkBoxWidget() const;

        inline QColor foreground(QPalette::ColorGroup fromGroup = QPalette::Current) const;

        inline QColor background(QPalette::ColorGroup fromGroup = QPalette::Current) const;

        QPoint calculateAlignment(
            const Qt::Alignment& alignment,
            const QRect&         inRect,
            const QRect&         contentRect,
            bool                 flipY = true) const;

        void alignedText(
            const QString&       text,
            const QRect&         inRect,
            const Qt::Alignment& alignment,
            const QColor&        color) const;

        void alignedIcon(
            const QIcon&         icon,
            const QRect&         inRect,
            const Qt::Alignment& alignment,
            const QColor&        color) const;

        void buttonImpl(const QColor& bg, const QColor& fg, const QColor& hv) const;

        void toolbarIconButtonImpl() const;

        void titlebarIconButtonImpl() const;

        void pushButtonImpl() const;

    public:
        Renderer(
            PaletteCache*       cache,
            const QStyle*       parent,
            const QStyleOption* option,
            QPainter*           painter,
            const QWidget*      widget);
        ~Renderer();

        void horizontalRule(const QColor& color) const;

        void stroke() const;

        void fill() const;

        void fillRect(const QRect& rect, const QColor& color) const;

        void strokeRect(const QRect& rect, const QColor& color) const;

        void fillSelection(const QRect& rect) const;

        void pushButton() const;

        void menuBarItem() const;

        void menuItem() const;

        void scrollBar() const;

        void panel() const;

        void checkBox() const;

        void slider() const;

        void fillLineEdit() const;
        void  spinBox() const;
    };

}  // namespace Jam::Editor::Const
