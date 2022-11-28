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
#include <QRect>
#include <QSettings>

namespace Jam::Editor
{
    constexpr const char* Org = "Hack-er-!";
    constexpr const char* App = "JamEngine";

    class SettingsDataState
    {
    public:
        SettingsDataState()                             = default;
        SettingsDataState(const SettingsDataState& rhs) = default;

        bool minify{false};
        int  spaces{4};
        bool reload{false};

        QString lastProject{};
        bool    clearOnRun{false};
    };

    class PersistentSettings
    {
    private:
        QSettings*        _settings{nullptr};
        SettingsDataState _state;

        void load();

        void save() const;

    public:
        explicit PersistentSettings(const QString& org = Org,
                                    const QString& app = App);
        ~PersistentSettings();

        SettingsDataState copy() const;

        SettingsDataState& ref();

        void apply(const SettingsDataState& state);

        bool minify() const;

        int spaces() const;

        bool reloadLast() const;

        bool clearOutputOnRun() const;

        QRect       geometry(const QRect& def = {}) const;
        bool        maximize(bool def = false) const;
        QStringList recentFiles(const QStringList& def = {}) const;
        QString     lastOpenDirectory(const QString& def = {}) const;
        QString     lastProject(const QString& def = {}) const;

        [[deprecated]]
        const QString& lastOpenProjectFile() const;

        void setGeometry(const QRect& geom) const;
        void setMaximize(bool v) const;
        void setRecentFiles(const QVariant& recentFiles) const;
        void setLastOpenDirectory(const QString& lastOpenDirectory) const;
        void setLastProject(const QString& lastProject) const;
    };

    inline bool PersistentSettings::minify() const
    {
        return _state.minify;
    }

    inline bool PersistentSettings::reloadLast() const
    {
        return _state.reload;
    }

    inline const QString& PersistentSettings::lastOpenProjectFile() const
    {
        return _state.lastProject;
    }

    inline bool PersistentSettings::clearOutputOnRun() const
    {
        return _state.clearOnRun;
    }

    inline SettingsDataState PersistentSettings::copy() const
    {
        return _state;
    }

    inline SettingsDataState& PersistentSettings::ref()
    {
        return _state;
    }
}  // namespace Jam::Editor
