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
#include "Interface/Style/Style.h"

namespace Jam::Editor
{
    namespace Const
    {
        constexpr QPalette::ColorRole B00 = QPalette::Shadow;
        constexpr QPalette::ColorRole B01 = QPalette::Dark;
        constexpr QPalette::ColorRole B02 = QPalette::Base;
        constexpr QPalette::ColorRole B03 = QPalette::Mid;
        constexpr QPalette::ColorRole B04 = QPalette::Midlight;
        constexpr QPalette::ColorRole G00 = QPalette::Light;
        constexpr QPalette::ColorRole G01 = QPalette::Highlight;
        constexpr QPalette::ColorRole G02 = QPalette::Window;
        constexpr QPalette::ColorRole G03 = QPalette::WindowText;
        constexpr QPalette::ColorRole G04 = QPalette::Button;
        constexpr QPalette::ColorRole R00 = QPalette::ButtonText;
        constexpr QPalette::ColorRole R01 = QPalette::ToolTipBase;
        constexpr QPalette::ColorRole R02 = QPalette::ToolTipText;
        constexpr QPalette::ColorRole R03 = QPalette::Text;
        constexpr QPalette::ColorRole R04 = QPalette::BrightText;
        constexpr QPalette::ColorRole Y00 = QPalette::HighlightedText;
        constexpr QPalette::ColorRole Y01 = QPalette::Link;
        constexpr QPalette::ColorRole Y02 = QPalette::LinkVisited;
        constexpr QPalette::ColorRole Y03 = QPalette::AlternateBase;
        constexpr QPalette::ColorRole Y04 = QPalette::PlaceholderText;

        [[deprecated("Use Editor::Palette instead")]] extern void accentPalette(QPalette& palette);

        constexpr QColor MakeGrey(const int o)
        {
            return {0x19 + 11 * o, 0x19 + 11 * o, 0x1A + 11 * o};
        }

        [[deprecated("Use Editor::Palette instead")]] extern void initializePalette();

        [[deprecated("Use Editor::Palette instead")]] extern void copyStylePalette(QPalette& palette, QPalette::ColorGroup group = QPalette::All);
    }  // namespace Const


    class Palette
    {
    public:
        static void applyInternal();

        [[deprecated("TODO: reorganize the palette order")]]
        static void getApplicationPalette(QPalette& palette, QPalette::ColorGroup group = QPalette::All);

        [[deprecated("TODO: reorganize the palette order")]]
        static void getAccentPalette(QPalette& palette);

        static void getSliderPalette(QPalette& palette);

        static void swapColorRole(QWidget* widget, QPalette::ColorRole a, QPalette::ColorRole b);

        static void swapAccentRole(QWidget* widget, QPalette::ColorRole a, QPalette::ColorRole b);

        static void setAccentRole(QWidget* widget, QPalette::ColorRole a, QPalette::ColorRole b);

        static void setColorRole(QWidget* widget, QPalette::ColorRole a, QPalette::ColorRole b);
    };

}  // namespace Jam::Editor
