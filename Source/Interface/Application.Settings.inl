// ReSharper disable CppNonInlineFunctionDefinitionInHeaderFile
// ReSharper disable once CppMissingIncludeGuard

namespace Jam::Editor
{
    void Application::saveSettings() const
    {
        if (_flags & NoSettings)
            return;

        Log::writeLine("Saving settings");

        const PersistentSettings settings;

        if (isMaximized())
        {
            settings.setGeometry(QRect{0, 0, 800, 600});
            settings.setMaximize(true);
        }
        else
        {
            settings.setGeometry(this->geometry());
            settings.setMaximize(false);
        }

        settings.setLastOpenDirectory(_lastOpenDir);
        settings.setRecentFiles(_recentFiles.data());

        if (settings.reloadLast())
            settings.setLastProject(_cachedProjectPath);
        else
            settings.setLastProject({});
    }

    void Application::loadSettings()
    {
        // Reload any application specific user settings.
        // things like window state, size and location.
        // Restrict to only window state data..
        if (_flags & NoSettings)
            return;

        Log::clear();
        Log::writeLine("Loading settings");

        const PersistentSettings settings;

        setGeometry(settings.geometry(QRect{0, 0, 800, 600}));

        if (settings.maximize())
            setWindowState(Qt::WindowMaximized);
        else
            setWindowState(Qt::WindowNoState);

        _recentFiles = RememberLastCache(settings.recentFiles());

        // used to remember last open directory for the file dialog.
        _lastOpenDir = settings.lastOpenDirectory();

        _cachedProjectPath = settings.lastProject();
    }

}  // namespace Jam::Editor
