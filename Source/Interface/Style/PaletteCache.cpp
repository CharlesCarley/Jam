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
#include "PaletteCache.h"

namespace Jam::Editor
{

    void PaletteCache::setCacheContext(
        const QStyleOption* option,
        const QWidget*      widget) const
    {
        _option = option;
        _widget = widget;
    }

    void PaletteCache::clear()
    {
        _cacheBit1 = 0;
    }

    const QColor& PaletteCache::cachedColor(const Mapping color, const MethodCall& call)
    {
        const uint32_t c = 1 << color;
        if (_cacheBit1 & c)
            return _colors[color];
        _colors[color] = call();
        _cacheBit1 |= c;
        return _colors[color];
    }

    const QColor& PaletteCache::pushButtonBackground()
    {
        return cachedColor(
            MapPushButtonBackground, [=]
            { return _option->palette.color(_widget->backgroundRole()); });
    }

    const QColor& PaletteCache::pushButtonForeground()
    {
        return cachedColor(
            MapPushButtonForeground, [=]
            { return _option->palette.color(_widget->foregroundRole()); });
    }

    const QColor& PaletteCache::pushButtonHover()
    {
        return cachedColor(
            MapPushButtonHover, [=]
            { return _option->palette.color(_widget->backgroundRole()).lighter(); });
    }

    const QColor& PaletteCache::iconButtonBackground()
    {
        return cachedColor(
            MapIconButtonBackground, [=]
            { return _option->palette.color(_widget->backgroundRole()); });
    }

    const QColor& PaletteCache::iconButtonForeground()
    {
        return cachedColor(
            MapIconButtonForeground, [=]
            { return _option->palette.color(_widget->foregroundRole()); });
    }

    const QColor& PaletteCache::iconButtonHover()
    {
        return cachedColor(
            MapIconButtonHover, [=]
            { return _option->palette.color(_widget->backgroundRole()).lighter(); });
    }

    const QColor& PaletteCache::titleButtonBackground()
    {
        return cachedColor(
            MapTitleButtonBackground, [=]
            { return _option->palette.color(_widget->backgroundRole()); });
    }

    const QColor& PaletteCache::titleButtonForeground()
    {
        return cachedColor(
            MapTitleButtonForeground, [=]
            { return _option->palette.color(_widget->foregroundRole()); });
    }

    const QColor& PaletteCache::titleButtonHover()
    {
        return cachedColor(
            MapTitleButtonHover, [=]
            { return _option->palette.color(_widget->backgroundRole()).lighter(); });
    }

    const QColor& PaletteCache::scrollBarGutter()
    {
        return cachedColor(
            MapScrollBarGutter, [=]
            { return _option->palette.color(_widget->backgroundRole()).darker(Const::NormalFactor); });
    }

    const QColor& PaletteCache::scrollbarSlider()
    {
        return cachedColor(
            MapScrollBarSlider, [=]
            { return _option->palette.color(_widget->backgroundRole()).lighter(Const::NormalFactor); });
    }

    const QColor& PaletteCache::selection()
    {
        return cachedColor(
            MapSelection, [=]
            { return _option->palette.alternateBase().color(); });
    }

    const QColor& PaletteCache::selectionHighlight()
    {
        return cachedColor(
            MapSelectionHighlight, [=]
            { return _option->palette.alternateBase().color().lighter(Const::SubtleFactor); });
    }

    const QColor& PaletteCache::panelText()
    {
        return cachedColor(
            MapPanelText, [=]
            { return _option->palette.color(_widget->foregroundRole()); });
    }

    const QColor& PaletteCache::menuBarBackground()
    {
        return cachedColor(
            MapMenuBarBackground, [=]
            { return _option->palette.color(_widget->backgroundRole()); });
    }

    const QColor& PaletteCache::menuBarForeground()
    {
        return cachedColor(
            MapMenuBarForeground, [=]
            { return _option->palette.color(_widget->foregroundRole()); });
    }

    const QColor& PaletteCache::menuBarHighlight()
    {
        return cachedColor(
            MapMenuBarHighlight, [=]
            { return _option->palette.color(_widget->foregroundRole()).lighter(); });
    }
}  // namespace Jam::Editor
