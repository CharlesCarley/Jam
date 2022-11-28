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
#include "Interface/Style/Render.h"
#include <QApplication>
#include <QCheckBox>
#include <QPainter>
#include <QStyleOption>
#include <QWidget>
#include "Interface/Style/PaletteCache.h"
#include "Interface/Constants.h"
#include "Math/Integer.h"
#include "Math/Real.h"

namespace Jam::Editor::Const
{
    inline const QStyleOptionButton* Renderer::buttonOption() const
    {
        return qstyleoption_cast<const QStyleOptionButton*>(_option);
    }

    inline const QStyleOptionMenuItem* Renderer::menuOption() const
    {
        return qstyleoption_cast<const QStyleOptionMenuItem*>(_option);
    }

    inline const QStyleOptionViewItem* Renderer::viewItemOption() const
    {
        return qstyleoption_cast<const QStyleOptionViewItem*>(_option);
    }

    inline const QStyleOptionComplex* Renderer::complexOption() const
    {
        return qstyleoption_cast<const QStyleOptionComplex*>(_option);
    }

    inline const QStyleOptionSlider* Renderer::sliderOption() const
    {
        return qstyleoption_cast<const QStyleOptionSlider*>(_option);
    }

    const QCheckBox* Renderer::checkBoxWidget() const
    {
        if (_widget->inherits("QCheckBox"))
            return (const QCheckBox*)_widget;
        return nullptr;
    }

    inline QColor Renderer::foreground(QPalette::ColorGroup fromGroup) const
    {
        return _option->palette.color(fromGroup, _widget->foregroundRole());
    }

    inline QColor Renderer::background(QPalette::ColorGroup fromGroup) const
    {
        return _option->palette.color(fromGroup, _widget->backgroundRole());
    }

    QPoint Renderer::calculateAlignment(
        const Qt::Alignment& alignment,
        const QRect&         inRect,
        const QRect&         contentRect,
        const bool           flipY) const
    {
        int       x0 = contentRect.x();
        int       y0 = contentRect.y();
        const int cw = contentRect.width();
        const int ch = contentRect.height();

        int x1, y1, x2, y2;
        inRect.getCoords(&x1, &y1, &x2, &y2);

        const int cx = x1 + ((x2 - x1) >> 1);
        const int cy = y1 + ((y2 - y1) >> 1);

        if (alignment & Qt::AlignVCenter)
        {
            y0 += cy;
            if (flipY)
                y0 += ch >> 1;
            else
                y0 -= ch >> 1;
        }
        else if (alignment & Qt::AlignBottom)
            y0 += y2 - ch;
        else
            y0 += y1;

        if (alignment & Qt::AlignHCenter)
        {
            x0 += cx;
            x0 -= cw >> 1;
        }
        else if (alignment & Qt::AlignRight)
            x0 += x2 - cw;
        else
            x0 += x1;

        x0 += _margin.x();
        y0 += _margin.y();

        return {x0, y0};
    }

    void Renderer::alignedText(
        const QString&       text,
        const QRect&         inRect,
        const Qt::Alignment& alignment,
        const QColor&        color) const
    {
        const QFontMetrics fm = _painter->fontMetrics();

        const int width = fm.horizontalAdvance(text);

        const QRect content = {0, 0, width, fm.ascent()};
        _painter->save();
        _painter->setPen(color);
        _painter->setClipRect(inRect);
        _painter->drawText(
            calculateAlignment(
                alignment,
                inRect,
                {0, 0, content.width(), content.height()},
                true),
            text);
        _painter->restore();
    }

    void Renderer::alignedIcon(
        const QIcon&         icon,
        const QRect&         inRect,
        const Qt::Alignment& alignment,
        const QColor&        color) const
    {
        _margin = {0, 0};

        const QPoint align = calculateAlignment(
            alignment,
            inRect,
            {0, 0, 14, 14},
            false);
        icon.paint(
            _painter,
            {
                align.x(),
                align.y(),
                14,
                14,
            });
    }

    void Renderer::buttonImpl(const QColor& bg, const QColor& fg, const QColor& hv) const
    {
        if (const QStyleOptionButton* item = buttonOption())
        {
            if (_option->state & QStyle::State_MouseOver)
            {
                fillRect(item->rect, hv);
                strokeRect(item->rect, hv.lighter(SubtleFactor));
            }
            else if (_option->state & QStyle::State_HasFocus)
                strokeRect(item->rect, _option->palette.color(QPalette::Active, QPalette::ColorRole::AlternateBase));
            else
            {
                if (bg.alpha() > 0)
                    fillRect(item->rect, bg);
            }

            const QRect& button = _style->subElementRect(
                QStyle::SE_PushButtonContents,
                item);

            if (!item->icon.isNull())
                alignedIcon(item->icon, item->rect, Qt::AlignCenter, Empty);
            else if (!item->text.isEmpty())
            {
                if (_option->state & QStyle::State_MouseOver)
                    alignedText(item->text, button, Qt::AlignCenter, fg.lighter());
                else
                    alignedText(item->text, button, Qt::AlignCenter, fg);
            }
        }
    }

    void Renderer::titlebarIconButtonImpl() const
    {
        buttonImpl(_cache->titleButtonBackground(),
                   _cache->titleButtonForeground(),
                   _cache->titleButtonHover());
    }

    void Renderer::toolbarIconButtonImpl() const
    {
        buttonImpl(_cache->iconButtonBackground(),
                   _cache->iconButtonForeground(),
                   _cache->iconButtonHover());
    }

    void Renderer::pushButtonImpl() const
    {
        buttonImpl(_cache->pushButtonBackground(),
                   _cache->pushButtonForeground(),
                   _cache->pushButtonHover());
    }

    Renderer::Renderer(PaletteCache*       cache,
                       const QStyle*       parent,
                       const QStyleOption* option,
                       QPainter*           painter,
                       const QWidget*      widget) :
        _cache(cache),
        _style(parent),
        _option(option),
        _painter(painter),
        _widget(widget)
    {
        Q_ASSERT(option && widget && painter);
        _cache->setCacheContext(option, widget);
        _painter->save();
    }

    Renderer::~Renderer()
    {
        _painter->restore();
    }

    void Renderer::horizontalRule(const QColor& color) const
    {
        int x1, y1, x2, y2;
        _option->rect.getCoords(&x1, &y1, &x2, &y2);
        const int cy = y1 + ((y2 - y1) >> 1);

        _painter->setPen(color);
        _painter->drawLine(x1, cy, x2, cy);
    }

    void Renderer::stroke() const
    {
        _painter->setPen(background().lighter());
        _painter->drawRect(_option->rect);
    }

    void Renderer::fill() const
    {
        if (_option->state & QStyle::State_ReadOnly)
            _painter->fillRect(_option->rect, background().lighter());
        else
            _painter->fillRect(_option->rect, background());
    }

    void Renderer::fillRect(const QRect& rect, const QColor& color) const
    {
        _painter->fillRect(rect, color);
    }

    void Renderer::strokeRect(const QRect& rect, const QColor& color) const
    {
        _painter->save();
        _painter->setPen(color);
        _painter->drawRect(rect.adjusted(1, 1, -1, -1));
        _painter->restore();
    }

    void Renderer::pushButton() const
    {
        if (const QVariant type = _widget->property("iconType");
            type.isValid())
        {
            switch (type.toInt())
            {
            case 1:
                toolbarIconButtonImpl();
                break;
            case 2:
                titlebarIconButtonImpl();
                break;
            default:
            case 0:
                pushButtonImpl();
                break;
            }
        }
        else
            pushButtonImpl();
    }

    void Renderer::menuBarItem() const
    {
        if (const QStyleOptionMenuItem* item = menuOption())
        {
            fillRect(item->rect, _cache->menubarBackground());

            if (!item->text.isEmpty())
            {
                _margin.setX(5);

                if (item->state & QStyle::State_Selected)
                    alignedText(item->text, item->rect, Qt::AlignVCenter, _cache->menubarHighlight());
                else
                    alignedText(item->text, item->rect, Qt::AlignVCenter, _cache->menubarForeground());
            }
        }
    }

    void Renderer::menuItem() const
    {
        if (const QStyleOptionMenuItem* item = menuOption())
        {
            if (item->state & QStyle::State_Selected)
                fillSelection(item->rect);
            else
                fill();

            if (!item->icon.isNull())
                alignedIcon(item->icon, item->rect, Qt::AlignVCenter, Empty);

            _margin.setX(24);
            if (item->menuItemType == QStyleOptionMenuItem::Separator)
                horizontalRule(background().lighter(SubtleFactor));
            else
            {
                if (QStringList sl = item->text.split(QChar('\t'), Qt::SkipEmptyParts);
                    sl.size() > 1)
                {
                    alignedText(
                        sl[0],
                        item->rect.adjusted(0, 0, -item->reservedShortcutWidth, 0),
                        Qt::AlignLeft | Qt::AlignVCenter,
                        foreground());

                    _margin.setX(0);
                    alignedText(
                        sl[1],
                        item->rect,
                        Qt::AlignRight | Qt::AlignVCenter,
                        foreground());
                }
                else
                {
                    alignedText(
                        item->text,
                        item->rect,
                        Qt::AlignLeft | Qt::AlignVCenter,
                        foreground());
                }
            }
        }
    }

    void Renderer::scrollBar() const
    {
        if (const QStyleOptionSlider* item = sliderOption())
        {
            QRect slider = _style->subControlRect(QStyle::CC_ScrollBar, item, QStyle::SC_ScrollBarSlider, _widget);

            if (item->orientation == Qt::Vertical)
                slider.adjust(3, 1, -3, 1);
            else
                slider.adjust(1, 3, -1, -3);

            _painter->fillRect(item->rect, _cache->scrollbarGutter());
            _painter->fillRect(slider, _cache->scrollbarSlider());
        }
    }

    void Renderer::panel() const
    {
        if (const QStyleOptionViewItem* item = viewItemOption())
        {
            const int x1 = item->rect.left();
            const int x2 = item->rect.right();
            const int y1 = item->rect.top();
            const int y2 = item->rect.bottom();

            if (item->state & QStyle::State_Selected)
                fillSelection({0, y1, x1 + x2, y2 - y1});

            if (!item->icon.isNull())
            {
                alignedIcon(item->icon, item->rect, Qt::AlignVCenter, Empty);
                _margin.setX(18);
            }

            if (!item->text.isEmpty())
                alignedText(item->text, item->rect, Qt::AlignVCenter, foreground());
        }
    }

    void Renderer::checkBox() const
    {
        if (const QStyleOptionButton* item = buttonOption())
        {
            if (const QCheckBox* check = checkBoxWidget())
            {
                const QRect checkRect =
                    _style->subElementRect(
                        QStyle::SE_CheckBoxIndicator,
                        _option,
                        _widget);

                if (check->isChecked())
                    _painter->fillRect(checkRect, _option->palette.linkVisited());
                else
                    _painter->fillRect(checkRect, _option->palette.link());

                const QRect textRect =
                    _style->subElementRect(
                        QStyle::SE_CheckBoxContents,
                        _option,
                        _widget);

                alignedText(item->text,
                            textRect,
                            Qt::AlignVCenter,
                            foreground());
            }
        }
    }

    void Renderer::slider() const
    {
        if (const auto slider = sliderOption())
        {
            const QRect handle = _style->subControlRect(
                QStyle::CC_Slider,
                slider,
                QStyle::SC_SliderHandle,
                _widget);

            const QRect groove = _style->subControlRect(
                                           QStyle::CC_Slider,
                                           slider,
                                           QStyle::SC_SliderGroove,
                                           _widget)
                                     .adjusted(0, 5, 0, -5);

            _painter->fillRect(_option->rect, _option->palette.base());
            _painter->fillRect(groove, _option->palette.shadow());
            _painter->fillRect(handle, _option->palette.mid());
        }
    }

    void Renderer::fillLineEdit() const
    {
        _painter->setPen(_option->palette.shadow().color());

        if (_option->state & QStyle::State_ReadOnly)
            _painter->fillRect(_option->rect, background().lighter());
        else
            _painter->fillRect(_option->rect, background().lighter(130));

        _painter->drawRect(_option->rect.adjusted(1, 1, -1, -1));
    }

    void Renderer::spinBox() const
    {
        _painter->fillRect(_option->rect, _option->palette.alternateBase());
        _painter->setPen(_option->palette.midlight().color());
        _painter->drawRect(_option->rect.adjusted(1, 0, 0, 0));
    }

    void Renderer::fillSelection(const QRect& rect) const
    {
        _painter->fillRect(rect, _cache->selection());
    }

}  // namespace Jam::Editor::Const
