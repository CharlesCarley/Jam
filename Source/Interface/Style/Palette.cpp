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
#include "Interface/Constants.h"

namespace Jam::Editor::Const
{

     const QColor AlternateBase   = Blue;
     const QColor Shadow          = Base.lighter(NormalFactor);
     const QColor Dark            = Shadow.lighter(NormalFactor);
     const QColor Mid             = Dark.lighter(NormalFactor);
     const QColor MidLight        = Mid.lighter(NormalFactor);
     const QColor Light           = MidLight.lighter(NormalFactor);
     const QColor Highlight       = Light.lighter(NormalFactor);
     const QColor Text            = Highlight.lighter(/*NormalFactor*/);
     const QColor BrightText      = Text.lighter(NormalFactor);
     const QColor HighlightedText = BrightText.lighter(NormalFactor);
     const QColor PlaceholderText = Empty;
     const QColor WindowText      = Text;
     const QColor Button          = Mid;
     const QColor ButtonText      = Text;
     const QColor Window          = Shadow;
     const QColor Link            = Red;
     const QColor LinkVisited     = Green;
     const QColor ToolTipBase     = Yellow;
     const QColor ToolTipText     = Highlight;


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
                palette.setColor((QPalette::ColorGroup)group, (QPalette::ColorRole)color, Empty);
        }
    }

    void applyPalette(QPalette& palette, const QPalette::ColorGroup group)
    {
        palette.setColor(group, QPalette::WindowText, WindowText);
        palette.setColor(group, QPalette::Button, Button);
        palette.setColor(group, QPalette::Light, Light);
        palette.setColor(group, QPalette::Midlight, MidLight);
        palette.setColor(group, QPalette::Dark, Dark);
        palette.setColor(group, QPalette::Mid, Mid);
        palette.setColor(group, QPalette::Text, Text);
        palette.setColor(group, QPalette::BrightText, BrightText);
        palette.setColor(group, QPalette::ButtonText, ButtonText);
        palette.setColor(group, QPalette::Base, Base);
        palette.setColor(group, QPalette::Window, Window);
        palette.setColor(group, QPalette::Shadow, Shadow);
        palette.setColor(group, QPalette::Highlight, Highlight);
        palette.setColor(group, QPalette::HighlightedText, HighlightedText);
        palette.setColor(group, QPalette::Link, Link);
        palette.setColor(group, QPalette::LinkVisited, LinkVisited);
        palette.setColor(group, QPalette::AlternateBase, AlternateBase);
        palette.setColor(group, QPalette::NoRole, NoRole);
        palette.setColor(group, QPalette::ToolTipBase, ToolTipBase);
        palette.setColor(group, QPalette::ToolTipText, ToolTipText);
        palette.setColor(group, QPalette::PlaceholderText, PlaceholderText);

        QGuiApplication::setPalette(palette);
    }

    void initializePalette()
    {
        QPalette palette;

        clearAppPalette(palette);

        applyPalette(palette);
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

}  // namespace Jam::Editor::Locals
