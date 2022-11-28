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
#include <unordered_map>
#include "Utils/CommandLine/Option.h"
#include "Utils/CommandLine/Scanner.h"
#include "Utils/FileSystem.h"
#include "Utils/Path.h"

/**
 * \brief Provides classes that handle parsing command line options.
 */
namespace Jam::CommandLine
{
    /**
     * \brief Is a basic command line parser that matches
     * arguments supplied by main with predetermined rules.
     */
    class Parser
    {
    public:
        typedef std::unordered_map<String, ParseOption*> Switches;
        typedef std::vector<ParseOption*>                Options;
        typedef std::vector<String>                      StringArray;

    private:
        int         _maxLongSwitch{4};
        int         _requiredOptions{0};
        int         _usedOptions{0};
        Scanner     _scanner;
        Switches    _switches;
        StringArray _argumentList;
        Options     _options;
        PathUtil    _programName;

        bool hasSwitch(const String& sw) const;

        bool initializeOption(ParseOption* opt, const Switch& sw);

        bool initializeSwitches(const Switch* switches, uint32_t count);

        int writeError(const OutputStringStream& stream) const;

        int parseOptions(Token& token, String& tmpBuffer);


        bool setupParse(int argc, char** argv);

        int parseImpl();


    public:
        Parser() = default;
        ~Parser();

        int parse(int argc, char** argv, const Switch* switches, uint32_t count);

        int parseNoSwitch(int argc, char** argv);

        void logInput() const;

        StringArray& arguments()
        {
            return _argumentList;
        }

        String programPath() const
        {
            return _programName.fullPath();
        }

        /// <summary>
        /// Returns only the file name portion of the program that was supplied to main via argv[0]
        /// </summary>
        String programName() const;

        /// <summary>
        /// Extracts the directory from the supplied path to main.
        /// </summary>
        /// <returns>The directory name of the program from argv[0]</returns>
        String programDirectory() const;

        /// <summary>
        /// Returns the current working directory for this process.
        /// </summary>
        /// <returns>The current working directory.</returns>
        static String currentDirectory();

        /// <param name="enumId">The switch id</param>
        /// <returns>true if it is supplied on the command line false otherwise</returns>
        bool isPresent(const uint32_t& enumId) const;

        /// <param name="enumId">The switch id</param>
        /// <returns> the option at the enumId or null if the id is out of bounds</returns>
        ParseOption* option(const uint32_t& enumId) const;

        int32_t int32(const uint32_t& enumId,
                      size_t          idx          = 0,
                      int32_t         defaultValue = -1,
                      int32_t         base         = 10) const;

        int64_t int64(const uint32_t& enumId,
                      size_t          idx          = 0,
                      int64_t         defaultValue = -1,
                      int32_t         base         = 10) const;

        const String& string(const uint32_t& enumId,
                             size_t          idx          = 0,
                             const String&   defaultValue = "") const;

        void usage() const;

        void usage(String& dest) const;
    };
}  // namespace Jam::CommandLine
