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
#include "PaletteCache.h"

namespace Jam::Editor::Const
{
    extern void initializePalette();
    extern void applyPalette(QPalette& palette, QPalette::ColorGroup group = QPalette::All);

    extern QColor ModColor(const QColor& color, int hue, qreal saturation = 1, qreal value = 1);

    constexpr QColor Blue = QColor(0x34, 0x43, 0x82);
    const QColor Red    = ModColor(Blue, 345, 1, 1);
    const QColor Green  = ModColor(Blue, 70, 1, 1);
    const QColor Yellow = ModColor(Blue, 45, .75, 1);

    const QColor Blue00 = ModColor(Blue, 190, 1, 0.6);
    const QColor Blue01 = ModColor(Blue, 190, 1, 0.7);
    const QColor Blue02 = ModColor(Blue, 190, 1, 0.8);
    const QColor Blue03 = ModColor(Blue, 190, 1, 0.9);
    const QColor Blue04 = ModColor(Blue, 190, 1, 1.0);

    const QColor Green00 = ModColor(Blue, 70, 1, 0.6);
    const QColor Green01 = ModColor(Blue, 70, 1, 0.7);
    const QColor Green02 = ModColor(Blue, 70, 1, 0.8);
    const QColor Green03 = ModColor(Blue, 70, 1, 0.9);
    const QColor Green04 = ModColor(Blue, 70, 1, 1.0);

    const QColor Red00 = ModColor(Blue, 345, 1, 0.6);
    const QColor Red01 = ModColor(Blue, 345, 1, 0.7);
    const QColor Red02 = ModColor(Blue, 345, 1, 0.8);
    const QColor Red03 = ModColor(Blue, 345, 1, 0.9);
    const QColor Red04 = ModColor(Blue, 345, 1, 1.0);
}  // namespace Jam::Editor::Const
