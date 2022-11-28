#pragma once
#include <functional>
#include <iostream>
#include "Math/Integer.h"
#include "Utils/StreamMethods.h"
#include "Math/Real.h"
#include "Utils/String.h"

namespace Jam
{
    class Print
    {
    public:
        using Hook = std::function<void(const String&)>;

    private:
        static Hook _log;

    public:
        static constexpr U8 len = 4;

        static void bind(Hook hook);

        template <typename... Args>
        static void print(Args&&... args)
        {
            if (_log != nullptr)
            {
                OutputStringStream oss;
                ((oss << std::forward<Args>(args)), ...);
                oss << std::endl;
                _log(oss.str());
            }
            else
                ((std::cout << std::forward<Args>(args)), ...) << std::endl;
        }
        static void line();

        static void print1I(const I32& i);

        static void print2I(const I32& x1, const I32& x2);

        static void print1F(const R32& x0, U8 p = 3);

        static void print2F(const R32& x1, const R32& x2, U8 p = 3);

        static void printIa(const I32Array& ia);

        static void printV1F(const String& s0, const R32& x0, U8 p = 3);
    };
    using Lg = Print;
}  // namespace Jam
