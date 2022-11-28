#include "State/OutputLogMonitor.h"
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QFileSystemWatcher>

namespace Jam::Editor::State
{
    OutputLogMonitor::OutputLogMonitor() :
        QObject()
    {
        construct();
    }

    OutputLogMonitor::OutputLogMonitor(const OutputLogMonitor&)
    {
        construct();
    }

    OutputLogMonitor::~OutputLogMonitor()
    {
        delete _text;
        _text = nullptr;

        delete _watcher;
        _watcher = nullptr;
    }

    void OutputLogMonitor::construct()
    {
        _text    = new QString();
        _logName = QCoreApplication::applicationDirPath() + "/output.log";

        _watcher = new QFileSystemWatcher();
        _watcher->addPath(_logName);

        connect(_watcher, &QFileSystemWatcher::fileChanged, this, [=]
                { onFileChanged(); });
    }

    void OutputLogMonitor::onFileChanged()
    {
        QFile file(_logName);
        if (const QDir dir(_logName);
            dir.exists())
            file.open(QIODeviceBase::ReadOnly);
        else
            file.open(QIODeviceBase::ReadWrite);

        // TODO: buffer into this from the file.

        *_text = QString::fromLocal8Bit(
            filter(file.readAll()));

        emit fileChanged();
    }

    QByteArray OutputLogMonitor::filter(const QByteArray& ar)
    {
        QByteArray ba;
        for (const char& ch : ar)
        {
            if (ch >= 32 && ch < 127 ||
                ch == '\r' ||
                ch == '\n' ||
                ch == '\t')
                ba.append(ch);
        }
        return ba;
    }

    const QString* OutputLogMonitor::text() const
    {
        if (_text)
            return _text;
        return nullptr;
    }

    void OutputLogMonitor::writeLine(const QString& message) const
    {
        QFile file(_logName);
        file.open(QIODeviceBase::Append | QIODeviceBase::ReadWrite);

        file.write(message.toLocal8Bit());
        file.write("\n");
    }

    void OutputLogMonitor::write(const QString& message) const
    {
        QFile file(_logName);
        file.open(QIODeviceBase::Append | QIODeviceBase::ReadWrite);

        file.write(message.toLocal8Bit());
    }

    void OutputLogMonitor::clear() const
    {
        QFile file(_logName);
        file.open(QIODeviceBase::Truncate | QIODeviceBase::WriteOnly);
    }

}  // namespace Jam::Editor::State
