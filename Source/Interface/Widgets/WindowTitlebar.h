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
#include <QWidget>

class QHBoxLayout;

namespace Jam::Editor
{
    enum WindowTitlebarOptions
    {
        Minimize  = 1 << 0,
        Maximize  = 1 << 1,
        Close     = 1 << 2,
        Title     = 1 << 3,
        OkCancel  = 1 << 4,
        Locked    = 1 << 5,
        Default   = Minimize | Maximize | Close,
        Reserved  = 1 << 30,
        Reserved1 = 1 << 29,
    };

    class WindowTitlebar final : public QWidget
    {
        Q_OBJECT
    signals:
        void exit();
        void maximize();
        void minimize();

        void dragDialog(const QPointF& clickPoint);

    private:
        QHBoxLayout* _toolbar{nullptr};
        int          _titleBarBit;
        QPointF      _lastPos{};
        QString      _title{};

        void construct();

        void appendButton(int icon, const std::function<void()>& function);

        void mousePressEvent(QMouseEvent* event) override;

        void mouseReleaseEvent(QMouseEvent* event) override;

        void mouseDoubleClickEvent(QMouseEvent* event) override;

        void mouseMoveEvent(QMouseEvent* event) override;

    public:
        explicit WindowTitlebar(int opts = Default, QWidget* parent = nullptr);
        explicit WindowTitlebar(QString title, int opts = Default, QWidget* parent = nullptr);
        ~WindowTitlebar() override;
    };
}  // namespace Jam::Editor
