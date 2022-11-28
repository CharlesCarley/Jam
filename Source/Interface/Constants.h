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
#include <QPalette>

namespace Jam::Editor::Const
{
    using QRole             = QPalette::ColorRole;
    constexpr QColor Base   = QColor(0x1D, 0x1D, 0x1E);
    constexpr QColor NoRole = QColor(0xFF, 0x00, 0xFF, 0x00);
    constexpr QColor Empty  = QColor(0xFF, 0x00, 0xFF);

    constexpr int SplitterSize   = 4;
    constexpr int ToolbarSpacing = 10;
    constexpr int IconPadding    = 3;
    constexpr int IconSize       = 12;
    constexpr int NormalFactor   = 125;
    constexpr int SubtleFactor   = 105;
    constexpr int AreaPadding    = 2;

    constexpr QRole SplitterRole  = QRole::Shadow;
    constexpr QRole ToolRole      = QRole::Midlight;
    constexpr QRole AreaRole      = QRole::Dark;
    constexpr QRole AreaLightRole = QRole::Mid;
    constexpr QRole MenuBarRole   = QRole::Dark;
    constexpr QRole BorderRole    = QRole::Mid;
    constexpr QRole MenuRole      = QRole::Shadow;

}  // namespace Jam::Editor::Const

