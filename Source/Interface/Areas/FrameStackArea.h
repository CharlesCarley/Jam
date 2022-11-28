#pragma once
#include "Interface/Area/Area.h"

namespace Jam::Editor
{
    class FrameStackAreaPrivate;

    class FrameStackArea final : public Area
    {
        Q_OBJECT
    private:
        FrameStackAreaPrivate* _private;

        void construct();

    public:
        explicit FrameStackArea(AreaCreator* creator, QWidget* parent = nullptr);

        ~FrameStackArea() override;
    };

}  // namespace Jam::Editor
