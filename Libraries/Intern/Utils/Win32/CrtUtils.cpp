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
#include "Utils/Win32/CrtUtils.h"

#ifdef _WIN32
#include <crtdbg.h>
#include "Utils/Console.h"
#endif

namespace Jam
{
#ifdef _WIN32
    static _CrtMemState __startState = {};

    int CrtReportHook(const int type, char* msg, int*)
    {
        if (msg != nullptr)
            (void)fprintf(stderr, "%s", msg);
        return 1;
    }
#endif

    void CrtTestMemory()
    {
#ifdef _WIN32

        // enable reporting and keep any flags that are present.
        int flag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
        flag |= _CRTDBG_LEAK_CHECK_DF;

        // ignore internal CRT memory allocations
        flag &= ~_CRTDBG_CHECK_CRT_DF;

        _CrtSetDbgFlag(flag);
        _CrtSetReportHook(CrtReportHook);

        __startState = {};
        _CrtMemCheckpoint(&__startState);
#endif
    }

    void CrtDump()
    {
#ifdef _WIN32
        if (__startState.pBlockHeader != nullptr)
        {
            _CrtMemState endState = {};
            _CrtMemCheckpoint(&endState);

            _CrtMemState dumpState = {};
            if (_CrtMemDifference(&dumpState, &__startState, &endState) != 0)
            {
                _CrtMemDumpStatistics(&dumpState);

                int flag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
                flag &= ~_CRTDBG_LEAK_CHECK_DF;

                // ignore internal CRT memory allocations
                flag &= ~_CRTDBG_CHECK_CRT_DF;

                _CrtSetDbgFlag(flag);
                _CrtSetReportHook(nullptr);
            }

            __startState = {};
        }
        else
        {
            if (_CrtDumpMemoryLeaks() == 0)
            {
                // Console::writeLine("No memory leaks were found");
            }
        }

        _CrtSetReportHook(nullptr);
#endif
    }

    void CrtBreakOn(uint64_t alloc)
    {
#ifdef _DEBUG
#ifdef _WIN32
        _crtBreakAlloc = alloc;
#endif
#endif
    }

}  // namespace Jam
