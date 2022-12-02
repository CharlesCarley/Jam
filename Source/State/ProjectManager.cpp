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
#include "State/ProjectManager.h"
#include <iostream>
#include "Interface/Defaults/Builtin.h"
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

        XmlFile psr(Tags::ProjectFileTags, Tags::ProjectFileTagsMax);
        psr.read(stream, projectPath);

        bool status = false;

        if (const XmlNode* jam = psr.root(Tags::JamTagId))
        {
            clearProjectState();

            if (const XmlNode* mainLayout = jam->firstChildOf(Tags::TreeTagId))
            {
                Xml::Writer::toString(_layout, mainLayout);
                if (!_layout.empty())
                    status = true;
            }

            if (const XmlNode* stack = jam->firstChildOf(Tags::FrameStackTagId))
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
            out << "<" << Tags::JamTag.typeName << ">" << std::endl;
            out << layout;

            FrameStackSerialize serialize(layerStack()->stack());
            serialize.save(out);

            out << "</" << Tags::JamTag.typeName << ">" << std::endl;
            return true;
        }

        Console::writeLine("failed to open file for saving: '", path, "'");
        return false;
    }

    void ProjectManager::clearProjectState()
    {
        _path   = {};
        _layout = {};

        loadDefaultStack();
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
        layerStack()->clear();

        Builtin::ByteArray ba;
        Builtin::Layouts::getStack(ba);

        if (!ba.empty())
        {
            StringStream ss;
            ss.write((char*)ba.data(), (std::streamsize)ba.size());
            layerStack()->load(ss);
        }
        else
        {
            const auto stack = layerStack();
            stack->addLayer(new GridLayer());
            stack->addLayer(new FunctionLayer());
        }
    }
}  // namespace Jam::Editor::State
