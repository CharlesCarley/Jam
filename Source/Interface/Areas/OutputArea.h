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
#include "Interface/Area/Area.h"
#include "State/App.h"
#include "State/OutputLogMonitor.h"
#include "Utils/Console.h"
#include "Utils/String.h"

class QPlainTextEdit;

namespace Jam::Editor
{
    class OutputArea final : public Area
    {
        Q_OBJECT
    private:
        QPlainTextEdit* _edit{nullptr};

        void construct();

        void constructToolbar();

        QLayout* constructContent();

        void refreshOutput() const;

        static void clearOutput();

    public:
        explicit OutputArea(AreaCreator* creator, QWidget* parent = nullptr);
        ~OutputArea() override;

    protected:
        bool event(QEvent* event) override;
    };

    namespace Log
    {
        // redirects the supplied calls to the
        // output log window..


        template <typename... Args>
        void writeLine(Args&&... args)
        {
            OutputStringStream oss;
            ((oss << std::forward<Args>(args)), ...);

            if (const auto out = State::outputState())
                out->writeLine(QString::fromStdString(oss.str()));
            else
                Con::println(oss.str().c_str());
        }

        template <typename... Args>
        void write(Args&&... args)
        {
            OutputStringStream oss;
            ((oss << std::forward<Args>(args)), ...);

            if (const auto out = State::outputState())
                out->write(QString::fromStdString(oss.str()));
            else
                Con::println(oss.str().c_str());
        }

        inline void clear()
        {
            if (const auto out = State::outputState())
                out->clear();
        }

    }  // namespace Log
}  // namespace Jam::Editor
