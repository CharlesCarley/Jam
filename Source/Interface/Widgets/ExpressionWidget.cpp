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
#include "Interface/Widgets/ExpressionWidget.h"
#include <QBoxLayout>
#include "Interface/Extensions.h"
#include "Interface/Style/Style.h"
#include "Interface/Widgets/IconButton.h"
#include "Interface/Widgets/StringWidget.h"

namespace Jam::Editor
{

    ExpressionWidget::ExpressionWidget(QWidget* parent) :
        QWidget(parent)
    {
        construct();
    }

    void ExpressionWidget::construct()
    {
        _line   = new StringWidget();
        _delete = Style::toolButton(Icons::Delete);

        const auto layout = Style::horizontalLayout();
        layout->addWidget(_line, 1);
        layout->addWidget(_delete);
        setLayout(layout);

        connectSignals();
    }

    void ExpressionWidget::connectSignals()
    {
        connect(_line, &StringWidget::editingFinished, this, [=](const String& text)
                { emit expressionChanged(_refId, text); });
        connect(_delete, &QPushButton::clicked, this, [=]
                { emit deleteExpression(_refId); });
    }

    void ExpressionWidget::setText(const String& text) const
    {
        _line->setText(text);
    }

    String ExpressionWidget::text() const
    {
        return _line->text();
    }

}  // namespace Jam::Editor
