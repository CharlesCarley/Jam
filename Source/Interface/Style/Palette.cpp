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
#include "Interface/Style/Palette.h"
#include <QApplication>
#include <QWidget>
#include "Style.h"

namespace Jam::Editor
{
    constexpr QColor SeedB = QColor(0x34, 0x43, 0x82);
    constexpr QColor SeedY = QColor(0x34, 0x53, 0xA2);

    constexpr QPalette::ColorRole RoleOrder[20] = {
        QPalette::Shadow,
        QPalette::Dark,
        QPalette::Base,
        QPalette::Mid,
        QPalette::Midlight,
        QPalette::Light,
        QPalette::Highlight,
        QPalette::Window,
        QPalette::WindowText,
        QPalette::Button,
        QPalette::ButtonText,
        QPalette::ToolTipBase,
        QPalette::ToolTipText,
        QPalette::Text,
        QPalette::BrightText,
        QPalette::HighlightedText,
        QPalette::Link,
        QPalette::LinkVisited,
        QPalette::AlternateBase,
        QPalette::PlaceholderText,
    };

    constexpr QColor MakeGrey(const int o)
    {
        return {
            0x19 + 11 * o,
            0x19 + 11 * o,
            0x1A + 11 * o,
        };
    }

    constexpr QColor NewPalette1[20] = {
        MakeGrey(0x00),            // Shadow
        MakeGrey(0x01),            // Dark
        MakeGrey(0x02),            // Base
        MakeGrey(0x03),            // Mid
        MakeGrey(0x04),            // MidLight
        MakeGrey(0x08),            // Light
        MakeGrey(0x0B),            // Highlight
        MakeGrey(0x01),            // Window
        MakeGrey(0x09),            // WindowText
        MakeGrey(0x03),            // Button
        MakeGrey(0x09),            // ButtonText
        QColor(0xb6, 0xa6, 0x3b),  // ToolTipBase
        MakeGrey(0x0B),            // ToolTipText
        MakeGrey(0x0D),            // Text
        MakeGrey(0x10),            // BrightText
        MakeGrey(0x12),            // HighlightedText
        QColor(0x00, 0x63, 0x82),  // Link
        QColor(0x00, 0x41, 0x4E),  // LinkVisited
        MakeGrey(0x07),            // AlternateBase
        QColor(0xFF, 0x00, 0xFF),  // PlaceholderText
    };

    void clearAppPalette(QPalette& palette)
    {
        // For all groups and all colors that belong
        // to that group, set the color's value to the
        // pre-determined Empty color.

        int groups[]{
            QPalette::ColorGroup::Active,
            QPalette::ColorGroup::Disabled,
            QPalette::ColorGroup::Inactive,
            QPalette::ColorGroup::Current,
            QPalette::ColorGroup::All,
            QPalette::ColorGroup::Normal,
        };

        int colors[] = {
            QPalette::WindowText,
            QPalette::Button,
            QPalette::Light,
            QPalette::Midlight,
            QPalette::Dark,
            QPalette::Mid,
            QPalette::Text,
            QPalette::BrightText,
            QPalette::ButtonText,
            QPalette::Base,
            QPalette::Window,
            QPalette::Shadow,
            QPalette::Highlight,
            QPalette::HighlightedText,
            QPalette::Link,
            QPalette::LinkVisited,
            QPalette::AlternateBase,
            QPalette::NoRole,
            QPalette::ToolTipBase,
            QPalette::ToolTipText,
            QPalette::PlaceholderText,
        };

        for (int group : groups)
        {
            for (int color : colors)
            {
                palette.setColor((QPalette::ColorGroup)group,
                                 (QPalette::ColorRole)color,
                                 {0xFF, 0x00, 0xFF});
            }
        }
    }

    void copyStylePalette(QPalette& palette, QPalette::ColorGroup group)
    {
        int i = 0;
        for (const QPalette::ColorRole r : RoleOrder)
            palette.setColor(group, r, NewPalette1[i++]);
    }

    void initializePalette()
    {
        QPalette palette;
        clearAppPalette(palette);
        copyStylePalette(palette, QPalette::All);
        QGuiApplication::setPalette(palette);
    }

    inline qreal ByteToReal(const int x)
    {
        return std::clamp<qreal>(x, 0, 255) / 256;
    }

    inline int RealToByte(const qreal x)
    {
        return std::clamp<int>(int(256.0 * x), 0, 255);
    }

    inline qreal Blend(const qreal a, const qreal b, const qreal v)
    {
        return v * a + (1 - v) * b;
    }

    QColor ModColor(
        const QColor& color,
        const int     hue,
        const qreal   saturation,
        const qreal   value)
    {
        int h, s, v;
        color.toHsv().getHsv(&h, &s, &v);

        h = std::clamp<int>(hue, 0, 359);
        s = RealToByte(Blend(1, ByteToReal(s), saturation));
        v = RealToByte(Blend(ByteToReal(v), 0, value));

        QColor r;
        r.setHsv(h, s, v);
        return r.toRgb();
    }

    constexpr qreal StepRates[5] = {
        0.45,
        0.60,
        0.75,
        0.90,
        1.00,
    };

    const QColor Red    = ModColor(SeedB, 345, 1, 1);
    const QColor Green  = ModColor(SeedB, 70, 1, 1);
    const QColor Yellow = ModColor(SeedB, 45, .75, 1);

    const QColor Blue00   = ModColor(SeedB, 190, 1, StepRates[0]);
    const QColor Blue01   = ModColor(SeedB, 190, 1, StepRates[1]);
    const QColor Blue02   = ModColor(SeedB, 190, 1, StepRates[2]);
    const QColor Blue03   = ModColor(SeedB, 190, 1, StepRates[3]);
    const QColor Blue04   = ModColor(SeedB, 190, 1, StepRates[4]);
    const QColor Green00  = ModColor(SeedB, 70, 1, StepRates[0]);
    const QColor Green01  = ModColor(SeedB, 70, 1, StepRates[1]);
    const QColor Green02  = ModColor(SeedB, 70, 1, StepRates[2]);
    const QColor Green03  = ModColor(SeedB, 70, 1, StepRates[3]);
    const QColor Green04  = ModColor(SeedB, 70, 1, StepRates[4]);
    const QColor Red00    = ModColor(SeedY, 345, 1, StepRates[0]);
    const QColor Red01    = ModColor(SeedY, 345, 1, StepRates[1]);
    const QColor Red02    = ModColor(SeedY, 345, 1, StepRates[2]);
    const QColor Red03    = ModColor(SeedY, 345, 1, StepRates[3]);
    const QColor Red04    = ModColor(SeedY, 345, 1, StepRates[4]);
    const QColor Yellow00 = ModColor(SeedY, 45, 1, StepRates[0]);
    const QColor Yellow01 = ModColor(SeedY, 45, 1, StepRates[1]);
    const QColor Yellow02 = ModColor(SeedY, 45, 1, StepRates[2]);
    const QColor Yellow03 = ModColor(SeedY, 45, 1, StepRates[3]);
    const QColor Yellow04 = ModColor(SeedY, 45, 1, StepRates[4]);

    const QColor AccentColors[20] = {
        Blue00,
        Blue01,
        Blue02,
        Blue03,
        Blue04,
        Green00,
        Green01,
        Green02,
        Green03,
        Green04,
        Red00,
        Red01,
        Red02,
        Red03,
        Red04,
        Yellow00,
        Yellow01,
        Yellow02,
        Yellow03,
        Yellow04,
    };

    QColor accentColor(const QPalette::ColorRole cr)
    {
        int i = 0;
        for (const QPalette::ColorRole r : RoleOrder)
        {
            if (r == cr)
                return AccentColors[i];
            ++i;
        }
        return {0xFF, 0x00, 0xFF};
    }

    void accentPalette(QPalette& palette)
    {
        int i = 0;
        for (const QPalette::ColorRole r : RoleOrder)
            palette.setColor(r, AccentColors[i++]);
    }
    void Palette::applyInternal()
    {
        QPalette palette;
        clearAppPalette(palette);

        getApplicationPalette(palette);

        QGuiApplication::setPalette(palette);
    }

    void Palette::getApplicationPalette(QPalette& palette, QPalette::ColorGroup group)
    {
        int i = 0;
        for (const QPalette::ColorRole r : RoleOrder)
            palette.setColor(group, r, NewPalette1[i++]);
    }

    void Palette::getAccentPalette(QPalette& palette)
    {
        int i = 0;
        for (const QPalette::ColorRole r : RoleOrder)
            palette.setColor(r, AccentColors[i++]);
    }

    void Palette::getSliderPalette(QPalette& palette)
    {
        getApplicationPalette(palette);
        Style::sliderPalette(palette);
    }

    void Palette::swapColorRole(QWidget* widget, QPalette::ColorRole a, QPalette::ColorRole b)
    {
        QPalette palette = widget->palette();

        const QColor ca = palette.color(a);
        const QColor cb = palette.color(b);

        palette.setColor(a, cb);
        palette.setColor(b, ca);
        widget->setPalette(palette);
    }

    void Palette::swapAccentRole(QWidget* widget, QPalette::ColorRole a, QPalette::ColorRole b)
    {
        QPalette     palette = widget->palette();
        const QColor ca      = palette.color(a);
        const QColor cb      = accentColor(b);
        palette.setColor(a, cb);
        palette.setColor(b, ca);
        widget->setPalette(palette);
    }

    void Palette::setAccentRole(QWidget* widget, QPalette::ColorRole a, QPalette::ColorRole b)
    {
        QPalette palette = widget->palette();
        palette.setColor(a, accentColor(b));
        widget->setPalette(palette);
    }

    void Palette::setColorRole(QWidget* widget, QPalette::ColorRole a, QPalette::ColorRole b)
    {
        QPalette palette = widget->palette();
        palette.setColor(a, palette.color(b));
        widget->setPalette(palette);
    }

}  // namespace Jam::Editor
