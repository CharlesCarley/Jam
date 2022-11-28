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
#include "Interface/Areas/OutputArea.h"
#include <QBoxLayout>
#include <QCoreApplication>
#include <QDir>
#include <QPlainTextEdit>
#include "Interface/Area/Area.h"
#include "Interface/Area/AreaToolbar.h"
#include "Interface/Constants.h"
#include "Interface/Events/EventTypes.h"
#include "Interface/Extensions.h"
#include "Interface/Widgets/IconButton.h"
#include "State/App.h"
#include "State/OutputLogMonitor.h"

namespace Jam::Editor
{
    OutputArea::OutputArea(AreaCreator* creator,
                           QWidget*     parent) :
        Area(creator, AtOutput, parent)
    {
        construct();
    }

    OutputArea::~OutputArea()
    {
        delete _edit;
        _edit = nullptr;
    }

    void OutputArea::construct()
    {
        if (const auto out = State::outputState())
        {
            connect(out,
                    &State::OutputLogMonitor::fileChanged,
                    this,
                    &OutputArea::refreshOutput);
        }

        constructToolbar();
        setLayout(constructContent());
        refreshOutput();
    }

    void OutputArea::constructToolbar()
    {
        const auto clear = IconButton::createToolButton(Icons::Clear);
        connect(clear,
                &QPushButton::clicked,
                this,
                &OutputArea::clearOutput);

        _toolbar->addStretch();
        _toolbar->addWidget(clear);
    }

    QLayout* OutputArea::constructContent()
    {
        const auto layout = new QVBoxLayout();
        View::layoutDefaults(layout);

        _edit = new QPlainTextEdit();
        View::widgetDefaults(_edit, this);

        layout->addWidget(toolbar());
        View::addLayoutMargin(layout,
                              _edit,
                              Const::AreaPadding);
        return layout;
    }

    bool OutputArea::event(QEvent* event)
    {
        if (event)
        {
            switch ((int)event->type())
            {
            case ProjectOpened:  // handle?
            case ProjectClosed:
            default:
                break;
            }
        }
        return Area::event(event);
    }

    void OutputArea::refreshOutput() const
    {
        if (const auto out = State::outputState())
        {
            if (_edit)
            {
                if (const auto text = out->text())
                {
                    _edit->setPlainText(*text);
                    _edit->moveCursor(QTextCursor::End);
                }
            }
        }
    }

    void OutputArea::clearOutput()
    {
        if (const auto out = State::outputState())
            out->clear();
    }
}  // namespace Jam::Editor
