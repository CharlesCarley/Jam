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
#include <QObject>

class QFileSystemWatcher;

namespace Jam::Editor::State
{
    // Monitors the output file in
    // the bin directory and synchronizes
    // it with the Internal console.

    class OutputLogMonitor final : public QObject
    {
        Q_OBJECT
    signals:
        void fileChanged();

    private:
        friend class App;

        QFileSystemWatcher* _watcher{nullptr};
        QString             _logName;
        QString*            _text{nullptr};

        OutputLogMonitor();
        OutputLogMonitor(const OutputLogMonitor&);
        ~OutputLogMonitor() override;

        void construct();

        void onFileChanged();

        static QByteArray filter(const QByteArray& ar); 

    public:
        const QString* text() const;

        void writeLine(const QString& message) const;

        void write(const QString& message) const;

        void clear() const;
    };

}  // namespace Jam::Editor::State
