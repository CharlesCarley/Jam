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
#include <QPlainTextEdit>
#include "Interface/Area/Area.h"
#include "Interface/Area/AreaToolbar.h"
#include "Interface/Areas/AreaType.h"
#include "Interface/Widgets/IconButton.h"
#include "State/App.h"
#include "State/OutputLogMonitor.h"

namespace Jam::Editor
{
    OutputArea::OutputArea(AreaCreator* creator,
                           const size_t refId,
                           QWidget*     parent) :
        Area(creator, AtOutput, refId, parent)
    {
        construct();
    }

    OutputArea::~OutputArea()
    {
        disconnect(State::outputState(),
                   &State::OutputLogMonitor::fileChanged,
                   this,
                   &OutputArea::refreshOutput);

        delete _edit;
        _edit = nullptr;
    }

    void OutputArea::construct()
    {
        Style::apply(this, AreaStyle);

        connect(State::outputState(),
                &State::OutputLogMonitor::fileChanged,
                this,
                &OutputArea::refreshOutput);

        constructToolbar();
        setLayout(constructContent());
        refreshOutput();
    }

    void OutputArea::constructToolbar()
    {
        const auto clear = Style::toolButton(Icons::Clear);

        connect(clear,
                &QPushButton::clicked,
                this,
                &OutputArea::clearOutput);

        _toolbar->addStretch();
        _toolbar->addWidget(clear);
    }

    QLayout* OutputArea::constructContent()
    {
        _edit             = Style::plainText();
        const auto layout = Style::verticalLayout();

        layout->addWidget(toolbar());
        layout->addWidget(_edit, 1);
        return layout;
    }

    void OutputArea::refreshOutput() const
    {
        if (_edit)
        {
            if (const auto text = State::outputState()->text())
            {
                _edit->setPlainText(*text);
                _edit->moveCursor(QTextCursor::End);
            }
        }
    }

    void OutputArea::clearOutput()
    {
        State::outputState()->clear();
    }

}  // namespace Jam::Editor
