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
#include "Utils/Exception.h"
#include "Utils/String.h"

namespace Jam
{
    class FrameStack;
}

namespace Jam::Editor::State
{
    class ProjectManager
    {
    private:
        // This is a friend so that only ApplicationState may
        // instance this class. (do not access class members
        // with the extra freedom)
        friend class App;

        String         _path{};
        mutable String _layout{};

        ProjectManager();
        ProjectManager(const ProjectManager&) = default;

        ~ProjectManager() = default;

        void handleIoException(const Exception& ex);

        bool loadImpl(const String& projectPath, IStream& stream);

        bool saveImpl(const String& path, const String& layout);
        
        void clearProjectState();

    public:
        bool saveAs(const String& path, const String& layout);

        bool load(const String& projectPath);

        void loadDefaultStack();

        void unload();

        String layout() const;

        const String& path() const
        {
            return _path;
        }
    };
}  // namespace Jam::Editor::State
