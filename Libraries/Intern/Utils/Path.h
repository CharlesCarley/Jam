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

#include "Utils/String.h"

namespace Jam
{
    class PathUtil
    {
    private:
        String      _root;
        String      _directory;
        String      _file;
        StringDeque _extension;

        void construct(const String& str);

    public:
        PathUtil() = default;

        PathUtil(const PathUtil& oth);

        /**
         * \brief Constructs the path with a file name.
         * \param fileName The file path that will be broken up into sections.
         */
        explicit PathUtil(const String& fileName);

        /**
         * \brief Provides access to the name portion of the file.
         * \return The file name without an extension.
         */
        const String& stem() const;

        /**
         * \brief Provides access to the directory portion of the file.
         * \return a/b/c/d/
         */
        const String& directory() const;

        void directoryList(StringDeque& dir) const;

        bool hasDirectory() const;

        String parentDir(size_t n) const;

        PathUtil parentPath(size_t n) const;

        /**
         * \return true if the file is present at the supplied path
         */
        bool exists() const;

        /**
         * \return The full file path
         */
        String fullPath() const;

        /**
         * \return The combination of all attached extensions
         */
        String rootedDir() const;

        /**
         * \return The file base name and any extensions attached to it
         */
        String fileName() const;

        void fileName(const PathUtil& file);

        void fileName(const String& file);

        /**
         * \return The combination of all attached extensions
         */
        String fullExtension() const;

        String primaryExtension();

        void appendDirectory(const PathUtil& filePath);

        void appendDirectory(const String& filePath);

        bool empty() const;

        static PathUtil construct(const String& path, const String& srcPath);
    };

    inline bool PathUtil::empty() const
    {
        return _file.empty();
    }

}  // namespace Jam
