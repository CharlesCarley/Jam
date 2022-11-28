#include "State/ProjectManager.h"
#include <iostream>
#include "FrameStack/FrameStackSerialize.h"
#include "FrameStackManager.h"
#include "Interface/Areas/OutputArea.h"
#include "State/ProjectTags.h"
#include "Xml/Declarations.h"
#include "Xml/File.h"
#include "Xml/Writer.h"

namespace Jam::Editor::State
{
    ProjectManager::ProjectManager()
    {
        clearProjectState();
    }

    void ProjectManager::handleIoException(const Exception& ex)
    {
        qDebug(ex.what());
        unload();
    }

    bool ProjectManager::loadImpl(const String& projectPath, IStream& stream)
    {
        // on success _path == projectPath

        XmlFile psr(ProjectFileTags, ProjectFileTagsMax);
        psr.read(stream, projectPath);

        bool status = false;

        if (const XmlNode* jam = psr.root(JamProjectTag))
        {
            clearProjectState();

            if (const XmlNode* mainLayout = jam->firstChildOf(TreeTag))
            {
                Xml::Writer::toString(_layout, mainLayout);
                if (!_layout.empty())
                    status = true;
            }

            if (const XmlNode* stack = jam->firstChildOf(FrameStackTag))
            {
                StringStream ss;
                Xml::Writer::toStream(ss, stack);

                const FrameStackSerialize serialize(
                    layerStack()->stack());
                serialize.load(ss);
            }
        }

        if (status)
            _path = projectPath;
        return status;
    }

    bool ProjectManager::saveImpl(const String& path, const String& layout)
    {
        OutputFileStream out;
        out.open(path);

        if (out.is_open())
        {
            _path = path;
            out << "<jam>" << std::endl;
            out << layout;

            const FrameStackSerialize serialize(layerStack()->stack());
            serialize.save(out);

            // XmlProject::saveFrameStack(stream);
            out << "</jam>" << std::endl;
            return true;
        }
        else
        {
            Console::writeLine(
                "failed to open file for "
                "saving: '",
                path,
                "'");
        }
        return false;
    }
    
    void ProjectManager::clearProjectState()
    {
        _path   = {};
        _layout = {};

        
        if (const auto stack = layerStack())
        {
            stack->clear();
            loadDefaultStack();
        }
    }

    void ProjectManager::unload()
    {
        clearProjectState();
    }

    String ProjectManager::layout() const
    {
        String copy = _layout;
        _layout     = {};
        return copy;
    }

    bool ProjectManager::saveAs(const String& path,
                                const String& layout)
    {
        try
        {
            return saveImpl(path, layout);
        }
        catch (Exception& ex)
        {
            handleIoException(ex);
        }
        return false;
    }

    bool ProjectManager::load(const String& projectPath)
    {
        try
        {
            InputFileStream stream(projectPath);
            return loadImpl(projectPath, stream);
        }
        catch (Exception& ex)
        {
            handleIoException(ex);
            return false;
        }
    }

    void ProjectManager::loadDefaultStack()
    {
        StringStream ss;

        ss << R"(<stack>)";
        ss << R"(<grid origin="0,0" axis="25,1,25,1"/>)";
        ss << R"(<function text="y=x"/>)";
        ss << R"(</stack>)";

        if (const auto stack = layerStack())
            stack->load(ss);
    }
}  // namespace Jam::Editor::State
