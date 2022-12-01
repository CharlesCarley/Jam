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
#include <cstdint>
#include "Interface/Constants.h"

namespace Jam::Editor
{
    class PaletteCache
    {
    private:
        enum Mapping
        {
            MapPushButtonBackground,
            MapPushButtonForeground,
            MapPushButtonHover,
            MapScrollBarGutter,
            MapScrollBarSlider,
            MapIconButtonBackground,
            MapIconButtonForeground,
            MapIconButtonHover,
            MapTitleButtonBackground,
            MapTitleButtonForeground,
            MapTitleButtonHover,
            MapSelection,
            MapSelectionHighlight,
            MapPanelText,
            MapMenuBarBackground,
            MapMenuBarForeground,
            MapMenuBarHighlight,
            MapMax
        };

        enum CacheBits
        {
            PushButtonBackground  = 1 << MapPushButtonBackground,
            PushButtonForeground  = 1 << MapPushButtonForeground,
            PushButtonHover       = 1 << MapPushButtonHover,
            ScrollBarGutter       = 1 << MapScrollBarGutter,
            ScrollBarSlider       = 1 << MapScrollBarSlider,
            IconButtonBackground  = 1 << MapIconButtonBackground,
            IconButtonForeground  = 1 << MapIconButtonForeground,
            IconButtonHover       = 1 << MapIconButtonHover,
            TitleButtonBackground = 1 << MapTitleButtonBackground,
            TitleButtonForeground = 1 << MapTitleButtonForeground,
            TitleButtonHover      = 1 << MapTitleButtonHover,
            Selection             = 1 << MapSelection,
            SelectionHighlight    = 1 << MapSelectionHighlight,
            PanelText             = 1 << MapPanelText,
            MenuBarBackground     = 1 << MapMenuBarBackground,
            MenuBarForeground     = 1 << MapMenuBarForeground,
            MenuBarHighlight      = 1 << MapMenuBarHighlight,
        };

        uint16_t _cacheBit1{0};
        QColor   _primitiveFill{};
        QColor   _controlFill{};

        mutable const QStyleOption* _option{nullptr};
        mutable const QWidget*      _widget{nullptr};

    private:
        QColor _colors[MapMax];

        using MethodCall = std::function<QColor()>;

        const QColor& cachedColor(Mapping color, const MethodCall& call);

    public:
        PaletteCache() = default;
        void setCacheContext(const QStyleOption* option, const QWidget* widget) const;
        void clear();

        const QColor& pushButtonForeground();
        const QColor& pushButtonBackground();
        const QColor& pushButtonHover();
        const QColor& iconButtonBackground();
        const QColor& iconButtonForeground();
        const QColor& iconButtonHover();
        const QColor& titleButtonBackground();
        const QColor& titleButtonForeground();
        const QColor& titleButtonHover();
        const QColor& scrollBarGutter();
        const QColor& scrollbarSlider();
        const QColor& selection();
        const QColor& selectionHighlight();
        const QColor& panelText();
        const QColor& menuBarBackground();
        const QColor& menuBarForeground();
        const QColor& menuBarHighlight();
    };
}  // namespace Jam::Editor
