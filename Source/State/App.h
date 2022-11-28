#pragma once

namespace Jam
{
    class FrameStack;
}

namespace Jam::Editor::State
{
    class FrameStackManager;
    class OutputLogMonitor;
    class ProjectManager;

    class App
    {
    private:
        static App* _instance;

        // TODO: place pointers to specific state classes here.

        FrameStackManager* _layerStack{nullptr};
        ProjectManager*    _project{nullptr};
        OutputLogMonitor*  _output{nullptr};

    private:
        App();
        ~App();

        void setup();

    public:
        // Singleton access to the application state.
        static App& instance();
        static void initialize();
        static void finalize();
        static bool isValid();

        ProjectManager* projectState() const;

        OutputLogMonitor* outputState() const;

        FrameStackManager* layerStack() const;
    };

    // Public api for state access.
    //
    // (Favor access this way vs ApplicationState::instance()->...)
    // The assumption is it's either valid or an exception.

    extern ProjectManager* projectState();

    extern OutputLogMonitor* outputState();

    extern FrameStackManager* layerStack();

}  // namespace Jam::Editor::State
