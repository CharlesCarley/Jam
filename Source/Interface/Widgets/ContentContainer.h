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

#include <QFileDialog>
#include "Interface/Widgets/WindowTitlebar.h"

class QVBoxLayout;

namespace Jam::Editor
{
    class ContentContainer final : public QWidget
    {
        Q_OBJECT

    signals:
        void mouseEntered();
        void mouseExited();

    private:
        QVBoxLayout* _layout{nullptr};

        void construct();

        void clear() const;

        void enterEvent(QEnterEvent* event) override;

        void leaveEvent(QEvent* event) override;
    public:
        explicit ContentContainer(QWidget* parent = nullptr);
        ~ContentContainer() override;

        void setBorder(int border) const;

        void setContentLayout(QLayout* layout) const;
        
    };
}  // namespace Jam::Editor
