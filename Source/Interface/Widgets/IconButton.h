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
#include <QCheckBox>
#include <QIcon>
#include <QPushButton>
#include "OkCancelWidget.h"
#include "Interface/Style/Icons.h"

class QWidget;
class QPushButton;

namespace Jam::Editor
{
    class IconButton final : public QPushButton
    {
        Q_OBJECT
        Q_PROPERTY(int iconType MEMBER _type)
    private:
        int _type{0};

        explicit IconButton(QWidget* parent = nullptr);

        static IconButton* createButton(
            const Icons::Icon& icon,
            QWidget*           parent);

    public:
        static IconButton* createToolButton(
            const Icons::Icon& icon,
            QWidget*           parent = nullptr);

        [[deprecated("Remove: use Buttons::createAreaToolButton instead")]]
        static IconButton* createTitleButton(
            const Icons::Icon& icon,
            QWidget*           parent = nullptr);
        static IconButton* create(
            const Icons::Icon& icon,
            QWidget*           parent = nullptr);
    };

    class Buttons
    {
        // TODO: Move and rename methods in IconButton here.
        // TODO: Keep usage to types that need created
        // with common settings and style properties
    public:
        static QCheckBox* createCheckBox(const QString& label, bool initialState);

        static IconButton* createTitleButton(
            const Icons::Icon& icon,
            QWidget*           parent = nullptr);

        static IconButton* createAreaToolButton(
            const Icons::Icon& icon,
            QWidget*           parent = nullptr);
    };

}  // namespace Jam::Editor
