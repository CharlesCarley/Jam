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
#include "Interface/Constants.h"
#include "Interface/Widgets/WindowTitlebar.h"

class QVBoxLayout;
class QTreeWidgetItem;
class QHBoxLayout;
class QTreeWidget;
class QLineEdit;

namespace Jam::Editor
{
    class OkCancelWidget;
    class ContentContainer;

    class Dialog : public QDialog
    {
        Q_OBJECT
    signals:
        void okClicked();
        void cancelClicked();

    public:
        enum ScaleEdge
        {
            None,
            LeftTop,
            RightTop,
            RightBottom,
            LeftBottom,
            Left,
            Top,
            Right,
            Bottom,
        };

    private:
        QVBoxLayout*      _layout{nullptr};
        QVBoxLayout*      _content{nullptr};
        ContentContainer* _center{nullptr};
        ScaleEdge         _scaleEdge{None};
        bool              _drag{false};
        QPointF           _last{};
        int               _border{1};
        Const::QRole      _borderRole{Const::BorderRole};
        QString           _title{};

        void constructDialog(int options);

        void bindSignals(
            int                   opts,
            const WindowTitlebar* title,
            const OkCancelWidget* applyAndExitOrExit);

        void maximize() const;

        void drag(const QPointF& offset) const;

        void clearDrag();

        void computeCursor(const QPointF& offset);

        void resetCursor();

        ScaleEdge computeEdge(const QPointF& offset) const;

        void mousePressEvent(QMouseEvent* event) override;

        void mouseReleaseEvent(QMouseEvent* event) override;

        void mouseMoveEvent(QMouseEvent* event) override;

        void enterEvent(QEnterEvent* event) override;

        void leaveEvent(QEvent* event) override;

    protected:
        void applyLayout(QLayout* layout);

    public:
        explicit Dialog(QString title, int options = Default, QWidget* parent = nullptr);
        explicit Dialog(int options = Default, QWidget* parent = nullptr);
        ~Dialog() override;

        int border() const;

        void setBorder(int border);
    };

    inline int Dialog::border() const
    {
        return _border;
    }

}  // namespace Jam::Editor
