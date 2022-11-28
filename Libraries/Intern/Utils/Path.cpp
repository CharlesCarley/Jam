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
#include "Utils/Path.h"
#include "Utils/Char.h"
#include "Utils/Definitions.h"
#include "Utils/FileSystem.h"

namespace Jam
{
    void PathUtil::construct(const String& str)
    {
        // first swap all \ characters to / characters
        String clean;
        StringUtils::replaceAll(clean, str, "\\", "/");

        size_t n = JtNpos;

        // find the index of the last /

        for (int64_t i = (int64_t)clean.size() - 1;
             n == JtNpos && i >= 0;
             --i)
        {
            if (clean[i] == '/')
                n = i;
        }

        if (n == JtNpos)
            _file = clean;
        else
        {
            _file      = clean.substr(n + 1, clean.size());
            _directory = clean.substr(0, n + 1);

            if (_directory[0] == '/')
            {
                // UNIX
                _root.clear();
                _directory = _directory.substr(1, _directory.size());
            }
            else if (_directory.size() > 1 && isLetter(_directory[0]) && _directory[1] == ':')
            {
                // Windows
                _root      = _directory.substr(0, 2);
                _directory = _directory.substr(3, _directory.size());
            }

            if (!_directory.empty())
            {
                if (_directory.front() == '/')
                    _directory = _directory.substr(1, _directory.size());
                if (_directory.back() != '/')
                    _directory.push_back('/');
            }
        }

        if (!_file.empty())
        {
            StringUtils::split(_extension, _file, ".");

            if (!_extension.empty())
            {
                _file = _extension.front();
                _extension.pop_front();
            }
        }
    }

    PathUtil::PathUtil(const PathUtil& oth)
    {
        construct(oth.fullPath());
    }

    PathUtil::PathUtil(const String& fileName)
    {
        construct(fileName);
    }

    String PathUtil::primaryExtension()
    {
        if (!_extension.empty())
            return _extension.back();
        return "";
    }

    String PathUtil::fileName() const
    {
        return Su::join(stem(), fullExtension());
    }

    void PathUtil::fileName(const PathUtil& file)
    {
        _file      = file._file;
        _extension = file._extension;
    }

    void PathUtil::fileName(const String& file)
    {
        const PathUtil path(file);

        _file      = path._file;
        _extension = path._extension;
    }

    const String& PathUtil::stem() const
    {
        return _file;
    }

    const String& PathUtil::directory() const
    {
        return _directory;
    }

    void PathUtil::directoryList(StringDeque& dir) const
    {
        StringUtils::split(dir, _directory, "/");
    }

    bool PathUtil::hasDirectory() const
    {
        return !_directory.empty();
    }

    String PathUtil::parentDir(size_t n) const
    {
        if (n < 1)
            return rootedDir();

        StringArray dir;
        StringUtils::split(dir, _directory, "/");

        size_t i = 0;
        while (!dir.empty() && i < n)
        {
            dir.pop_back();
            i++;
        }

        if (!dir.empty())
        {
            OutputStringStream oss;
            for (const String& ent : dir)
                oss << ent << '/';
            return Su::join(_root, '/', oss.str());
        }
        return Su::join(_root, '/');
    }

    PathUtil PathUtil::parentPath(size_t n) const
    {
        return PathUtil(parentDir(n));
    }

    bool PathUtil::exists() const
    {
        return StdFileSystem::exists(FilePath(fullPath()));
    }

    String PathUtil::fullPath() const
    {
        String ext = fullExtension();

        if (_directory.empty())
            return Su::join(_file, ext);

        return Su::join(_root, '/', _directory, _file, ext);
    }

    String PathUtil::rootedDir() const
    {
        return Su::join(_root, '/', _directory);
    }

    String PathUtil::fullExtension() const
    {
        OutputStringStream oss;
        for (const String& extension : _extension)
            oss << '.' << extension;
        return oss.str();
    }

    void PathUtil::appendDirectory(const PathUtil& filePath)
    {
        if (!filePath._directory.empty())
            _directory = Su::join((const String&)_directory, filePath._directory);
    }

    void PathUtil::appendDirectory(const String& filePath)
    {
        String temp = filePath;
        if (!temp.empty())
        {
            if (temp.back() != '\\' && temp.back() != '/')
                temp.push_back('/');
            if (temp.front() == '\\' || temp.front() == '/')
                temp = temp.substr(1, temp.size());
        }

        appendDirectory(PathUtil(temp));
    }

    PathUtil PathUtil::construct(const String& path, const String& srcPath)
    {
        PathUtil ut(path);
        ut.fileName(srcPath);
        return PathUtil(ut.fullPath());
    }
}  // namespace Jam
