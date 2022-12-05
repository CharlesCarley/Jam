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
#include "Interface/Widgets/WindowTitlebar.h"
#include <QBoxLayout>
#include <QLabel>
#include <QMouseEvent>
#include <QPushButton>

#include "Interface/Constants.h"
#include "Interface/Extensions.h"
#include "Interface/Style/Palette.h"
#include "Interface/Style/Style.h"

namespace Jam::Editor
{
    WindowTitlebar::WindowTitlebar(const int opts, QWidget* parent) :
        QWidget(parent),
        _titleBarBit(opts & ~(Reserved1 | Reserved))
    {
        construct();
    }

    WindowTitlebar::WindowTitlebar(QString title, const int opts, QWidget* parent) :
        QWidget(parent),
        _titleBarBit(opts & ~(Reserved1 | Reserved)),
        _title(std::move(title))
    {
        if (!_title.isEmpty())
            _titleBarBit |= Title;
        construct();
    }

    WindowTitlebar::~WindowTitlebar() = default;

    void WindowTitlebar::construct()
    {
        Style::apply(this, MenuBarStyle);

        _toolbar = Style::horizontalLayout(0, Const::ToolbarSpacing);
        _toolbar->setContentsMargins((_titleBarBit & Title) != 0 ? 6 : 1, 1, 0, 1);

        if (_titleBarBit & Title)
        {
            const auto title = new QLabel(_title);
            Style::apply(title, TransparentStyle);
            _toolbar->addWidget(title, 1);
        }
        else
            _toolbar->addStretch();

        if (_titleBarBit & Minimize)
            appendButton(Icons::Minimize, -1, [=]
                         { emit minimize(); });
        if (_titleBarBit & Maximize)
            appendButton(Icons::Maximize, -1, [=]
                         { emit maximize(); });
        if (_titleBarBit & Close)
            appendButton(Icons::XIcon, 2, [=]
                         { emit exit(); });

        setLayout(_toolbar);
    }

    void WindowTitlebar::appendButton(int icon, int type, const std::function<void()>& function)
    {
        const auto btn = Style::toolButton((Icons::Icon)icon);
        Style::apply(btn, TransparentStyle);
        if (type == 2)
            Palette::setAccentRole(btn, QPalette::AlternateBase, Const::R03);

        _toolbar->addWidget(btn);
        connect(btn, &QPushButton::clicked, this, function);
    }

    void WindowTitlebar::mousePressEvent(QMouseEvent* event)
    {
        if (event->button() == Qt::LeftButton)
        {
            const QPointF mco = event->globalPosition();
            if (!(_titleBarBit & Reserved1))
            {
                QRect rect;
                View::windowRect(rect, this);

                _titleBarBit |= Reserved1;
                _lastPos = QPoint((int)mco.x(), (int)mco.y()) - QPointF(rect.x(), rect.y());
            }
        }
        _titleBarBit |= Reserved;
        QWidget::mousePressEvent(event);
    }

    void WindowTitlebar::mouseReleaseEvent(QMouseEvent* event)
    {
        if (event->button() == Qt::LeftButton)
            _titleBarBit &= ~Reserved;

        _lastPos = {};
        _titleBarBit &= ~Reserved1;
        QWidget::mouseReleaseEvent(event);
    }

    void WindowTitlebar::mouseDoubleClickEvent(QMouseEvent*)
    {
        emit maximize();
    }

    void WindowTitlebar::mouseMoveEvent(QMouseEvent* event)
    {
        if (const QWidget* win = window())
        {
            if (win->windowState() != Qt::WindowMaximized)
            {
                if (_titleBarBit & Reserved)
                    emit dragDialog(event->globalPosition() - _lastPos);
            }
        }
    }

}  // namespace Jam::Editor
