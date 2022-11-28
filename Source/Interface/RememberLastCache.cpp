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

#include "Interface/RememberLastCache.h"
#include "Utils/Exception.h"
#include "Utils/Path.h"

namespace Jam::Editor
{

    RememberLastCache::RememberLastCache(const QStringList& items)
    {
        validate(items);
    }

    RememberLastCache::RememberLastCache(const RememberLastCache& cache)
    {
        validate(cache._pathList);
    }

    RememberLastCache::~RememberLastCache()
    {
        clear();
    }

    void RememberLastCache::validate(const QStringList& items)
    {
        for (const QString& path : items)
        {
            if (PathUtil ut(path.toStdString());
                ut.exists())
            {
                _pathList.push_back(path);
            }
            else
            {
                Console::writeLine("skipping non existent item ",
                                   path.toStdString());
            }
        }
    }

    void RememberLastCache::validate()
    {
        const QStringList pruned = _pathList;
        _pathList.clear();
        validate(pruned);
    }

    void RememberLastCache::clear()
    {
        _pathList.clear();
    }

    int RememberLastCache::find(const QString& fileName) const
    {
        // The list size should always remain less than ten.
        // As long as that remains true, a linear search over strings
        // will suffice.
        for (int i = 0; i < _pathList.size() && i < RememberLastMax; ++i)
        {
            if (_pathList[i] == fileName)
                return i;
        }
        return -1;
    }

    void RememberLastCache::store(const QString& fileName)
    {
        if (const int loc = find(fileName);
            loc == -1)
        {
            // It does not have a recent file in the list so
            // track up to MaxRecent number of files.
            // With the most current being pushed to the front of the list.
            // Any files over MaxRecent are dropped.

            if (_pathList.size() + 1 < RememberLastMax)
                _pathList.push_front(fileName);
            else
            {
                _pathList.push_front(fileName);
                _pathList.pop_back();
            }
        }
        else
        {
            // It does exist in the list, so remove it
            // from its current location an push it to
            // the front of the list.
            _pathList.remove(loc);
            _pathList.push_front(fileName);
        }

        validate();
    }

    int RememberLastCache::size() const
    {
        return (int)std::clamp<qsizetype>(_pathList.size(), 0, RememberLastMax);
    }

    QString RememberLastCache::at(const int idx) const
    {
        if (idx >= 0 && idx < size())
            return _pathList.at(idx);
        return {};
    }

    QVariant RememberLastCache::data() const
    {
        return _pathList;
    }

}  // namespace Jam::Editor
