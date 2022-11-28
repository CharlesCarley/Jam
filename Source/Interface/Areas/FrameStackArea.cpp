#include "FrameStackArea.h"
#include <QLabel>
#include "FrameStackAreaPrivate.h"
#include "Interface/Area/Area.h"
#include "Interface/Area/AreaToolbar.h"
#include "Interface/Constants.h"
#include "Interface/Extensions.h"
#include "Interface/Widgets/IconButton.h"

namespace Jam::Editor
{
    FrameStackArea::FrameStackArea(AreaCreator* creator, QWidget* parent) :
        Area(creator, AtFrameStack, parent),
        _private(nullptr)
    {
        construct();
    }

    FrameStackArea::~FrameStackArea() = default;

    void FrameStackArea::construct()
    {
        View::widgetDefaults(this);
        View::applyColorRoles(this, Const::AreaRole);

        const auto layout = new QVBoxLayout();
        View::layoutDefaults(layout);
        AreaToolBar* tools = toolbar();

        const auto home = IconButton::createToolButton(Icons::Home);
        tools->addWidget(
            home,
            0,
            Qt::AlignRight);

        _private = new FrameStackAreaPrivate();

        layout->addWidget(tools);
        layout->addWidget(_private, 1);

        connect(home, &QPushButton::clicked, this, [=]
                { _private->resetAxis(); });

        setLayout(layout);
    }

}  // namespace Jam::Editor
