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

#include <deque>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

namespace Jam
{
    using String      = std::string;
    using StringDeque = std::deque<std::string>;
    using StringArray = std::vector<std::string>;
    using StringMap   = std::unordered_map<std::string, std::string>;

    using IStream = std::istream;
    using OStream = std::ostream;

    using StringStream       = std::stringstream;
    using InputStringStream  = std::istringstream;
    using OutputStringStream = std::ostringstream;
    using InputFileStream    = std::ifstream;
    using OutputFileStream   = std::ofstream;

    class StringUtils
    {
    public:
        static void split(
            StringArray&  destination,
            const String& input,
            char          separator);

        static void splitRejectEmpty(
            StringArray&  destination,
            const String& input,
            char          separator);

        static void split(
            StringArray&  destination,
            const String& input,
            const String& separator);

        static void split(
            StringDeque&  destination,
            const String& input,
            const String& separator);

        static void splitLine(
            StringArray&  dest,
            const String& input,
            char          swap = '$');

        static void trimWs(
            String&       di,
            const String& in);

        static void trimL(
            String&       destination,
            const String& input,
            char          character);

        static void trimR(
            String&       destination,
            const String& input,
            char          character);

        static void trimRelPath(
            String&       destination,
            const String& input);

        static void trim(
            String&       destination,
            const String& input,
            char          character);

        static void replaceAll(
            String&       dest,
            const String& input,
            const String& a,
            const String& b);

        static void toLower(String& dest, const String& in);

        static void toUpper(String& dest, const String& in);

        static String toLowerFirst(const String& in);

        static String toUpperFirst(const String& in);

        template <typename... Args>
        static String join(Args&&... args)
        {
            OutputStringStream oss;
            ((oss << std::forward<Args>(args)), ...);
            return oss.str();
        }

        template <typename... Args>
        static OStream& merge(OStream& out, Args&&... args)
        {
            ((out << std::forward<Args>(args)), ...);
            return out;
        }
    };

    using Su = StringUtils;

}  // namespace Jam
