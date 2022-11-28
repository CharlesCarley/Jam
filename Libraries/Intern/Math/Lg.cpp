#include "Lg.h"
#include "Utils/StreamMethods.h"

namespace Jam
{
    Print::Hook Print::_log = nullptr;

    void Print::bind(Hook hook)
    {
        if (_log == nullptr)
            _log = std::move(hook);
    }

    void Print::line()
    {
        print(FillT<32>());
    }

    void Print::print1I(const I32& i)
    {
        print(
            SetS({
                "i",
            }),
            Equ(),
            SetI({
                i,
            }));
    }

    void Print::print2I(const I32& x1, const I32& x2)
    {
        print(
            SetS({
                "i",
                "j",
            }),
            Equ(),
            SetI({
                x1,
                x2,
            }));
    }

    void Print::print1F(const R32& x0, const U8 p)
    {
        print(SetS({"x0"}),
              Equ(),
              SetF({x0}, 0, p));
    }

    void Print::print2F(const R32& x1, const R32& x2, U8 p)
    {
        print(SetS({"x1", "x2"}),
              Equ(),
              SetF({x1, x2}, 0, p));
    }

    void Print::printIa(const I32Array& ia)
    {
        for (U32 i = 0; i < ia.size(); ++i)
        {
            print(SetS({"i", "ia[i]"}),
                  Equ(),
                  SetI({I32(i), ia[i]}));
        }
    }

    void Print::printV1F(const String& s0, const R32& x0, U8 p)
    {
        print(SetS({s0}),
              Equ(),
              SetF({x0}, 0, p));
    }
}  // namespace Jam
