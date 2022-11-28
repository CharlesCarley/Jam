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
#include "Interface/PersistentSettings.h"
#include <QRect>
#include "Utils/Exception.h"

namespace Jam::Editor
{
    PersistentSettings::PersistentSettings(
        const QString& org,
        const QString& app)
    {
        _settings = new QSettings(org, app);
        load();
    }

    PersistentSettings::~PersistentSettings()
    {
        save();
        delete _settings;
        _settings = nullptr;
    }

    void PersistentSettings::load()
    {
        _state.minify = _settings->value("/settings/minify",
                                         _state.minify)
                            .value<bool>();

        _state.spaces = _settings->value("/settings/spaces",
                                         _state.spaces)
                            .value<int>();

        _state.reload = _settings->value("/settings/reload",
                                         _state.reload)
                            .value<bool>();

        _state.clearOnRun = _settings->value("/settings/clearOnRun",
                                             _state.clearOnRun)
                                .value<bool>();
    }

    void PersistentSettings::save() const
    {
        _settings->setValue("/settings/minify", _state.minify);
        _settings->setValue("/settings/spaces", _state.spaces);
        _settings->setValue("/settings/reload", _state.reload);
        _settings->setValue("/settings/clearOnRun", _state.clearOnRun);
    }

    void PersistentSettings::apply(const SettingsDataState& state)
    {
        _state = SettingsDataState(state);
    }

    int PersistentSettings::spaces() const
    {
        return _state.spaces;
    }

    QRect PersistentSettings::geometry(const QRect& def) const
    {
        if (_settings)
            return _settings->value("/editor/geometry", def).toRect();
        return def;
    }

    bool PersistentSettings::maximize(const bool def) const
    {
        if (_settings)
            return _settings->value("/editor/maximize", def).value<bool>();
        return def;
    }

    QStringList PersistentSettings::recentFiles(const QStringList& def) const
    {
        if (_settings)
            return _settings->value("/editor/recentFiles", def).toStringList();
        return def;
    }

    QString PersistentSettings::lastOpenDirectory(const QString& def) const
    {
        if (_settings)
            return _settings->value("/editor/lastOpenDirectory", def).toString();
        return def;
    }

    QString PersistentSettings::lastProject(const QString& def) const
    {
        if (_settings)
            return _settings->value("/editor/lastProject", def).toString();
        return def;
    }

    void PersistentSettings::setGeometry(const QRect& geom) const
    {
        if (_settings)
            _settings->setValue("/editor/geometry", geom);
    }

    void PersistentSettings::setMaximize(const bool v) const
    {
        if (_settings)
            _settings->setValue("/editor/maximize", v);
    }

    void PersistentSettings::setRecentFiles(const QVariant& recentFiles) const
    {
        if (_settings)
            _settings->setValue("/editor/recentFiles", recentFiles);
    }

    void PersistentSettings::setLastOpenDirectory(const QString& lastOpenDirectory) const
    {
        if (_settings)
            _settings->setValue("/editor/lastOpenDirectory", lastOpenDirectory);
    }

    void PersistentSettings::setLastProject(const QString& lastProject) const
    {
        if (_settings)
            _settings->setValue("/editor/lastProject", lastProject);
    }
}  // namespace Jam::Editor
