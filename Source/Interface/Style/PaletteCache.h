#pragma once
#include <QColor>
#include <QStyleOption>
#include <cstdint>
#include "Interface/Constants.h"

#define MEMBER_NAME(name) _##name

#define CacheColor(NAME, ENUM, RECACHE)    \
public:                                    \
    const QColor& NAME()                   \
    {                                      \
        if (_cacheBit1 & ENUM)             \
            return MEMBER_NAME(NAME);      \
        MEMBER_NAME(NAME) = [=] RECACHE(); \
        _cacheBit1 |= ENUM;                \
        return MEMBER_NAME(NAME);          \
    }                                      \
                                           \
private:                                   \
    QColor MEMBER_NAME(NAME)               \
    {                                      \
    }

namespace Jam::Editor
{
    class PaletteCache
    {
    private:
        enum CacheBits
        {
            PushButtonBackground  = 1 << 0x00,
            PushButtonForeground  = 1 << 0x01,
            PushButtonHover       = 1 << 0x02,
            ScrollBarGutter       = 1 << 0x03,
            ScrollBarSlider       = 1 << 0x04,
            IconButtonBackground  = 1 << 0x05,
            IconButtonForeground  = 1 << 0x06,
            IconButtonHover       = 1 << 0x07,
            TitleButtonBackground = 1 << 0x08,
            TitleButtonForeground = 1 << 0x09,
            TitleButtonHover      = 1 << 0x0A,
            Selection             = 1 << 0x0B,
            SelectionHighlight    = 1 << 0x0C,
            PanelText             = 1 << 0x0D,
            MenuBarBackground     = 1 << 0x0D,
            MenuBarForeground     = 1 << 0x0E,
            MenuBarHighlight      = 1 << 0x0F,
        };

        uint16_t _cacheBit1{0};

        QColor _primitiveFill{};
        QColor _controlFill{};

        mutable const QStyleOption* _option{nullptr};
        mutable const QWidget*      _widget{nullptr};

    public:
        PaletteCache() = default;

        void setCacheContext(const QStyleOption* option, const QWidget* widget) const
        {
            _option = option;
            _widget = widget;
        }

        void clear()
        {
            _cacheBit1 = 0;
        }

        CacheColor(pushButtonBackground, PushButtonBackground, {
            return _option->palette.color(_widget->backgroundRole());
        });

        CacheColor(pushButtonForeground, PushButtonForeground, {
            return _option->palette.color(_widget->foregroundRole());
        });

        CacheColor(pushButtonHover, PushButtonHover, {
            return _option->palette.color(_widget->backgroundRole()).lighter();
        });

        CacheColor(iconButtonBackground, IconButtonBackground, {
            return _option->palette.color(_widget->backgroundRole());
        });

        CacheColor(iconButtonForeground, IconButtonForeground, {
            return _option->palette.color(_widget->foregroundRole());
        });

        CacheColor(iconButtonHover, IconButtonHover, {
            return _option->palette.color(_widget->backgroundRole()).lighter();
        });

        CacheColor(titleButtonBackground, TitleButtonBackground, {
            return _option->palette.color(_widget->backgroundRole());
        });

        CacheColor(titleButtonForeground, TitleButtonForeground, {
            return _option->palette.color(_widget->foregroundRole());
        });

        CacheColor(titleButtonHover, TitleButtonHover, {
            return _option->palette.color(_widget->backgroundRole()).lighter();
        });

        CacheColor(scrollbarGutter, ScrollBarGutter, {
            return _option->palette.color(_widget->backgroundRole()).darker(Const::NormalFactor);
        });

        CacheColor(scrollbarSlider, ScrollBarSlider, {
            return _option->palette.color(_widget->backgroundRole()).lighter(Const::NormalFactor);
        });

        CacheColor(selection, Selection, {
            return _option->palette.alternateBase().color();
        });

        CacheColor(selectionHighlight, SelectionHighlight, {
            return _option->palette.alternateBase().color().lighter(Const::SubtleFactor);
        });

        CacheColor(panelText, PanelText, {
            return _option->palette.color(_widget->foregroundRole());
        });

        CacheColor(menubarBackground, MenuBarBackground, {
            return _option->palette.color(_widget->backgroundRole());
        });

        CacheColor(menubarForeground, MenuBarForeground, {
            return _option->palette.color(_widget->foregroundRole());
        });

        CacheColor(menubarHighlight, MenuBarHighlight, {
            return _option->palette.color(_widget->foregroundRole()).lighter();
        });
    };
}  // namespace Jam::Editor
