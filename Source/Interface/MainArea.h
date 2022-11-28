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
#include <QWidget>
#include "Areas/AreaType.h"
#include "Utils/ScopePtr.h"
#include "Utils/String.h"
#include "Xml/Declarations.h"

class QVBoxLayout;
class QTabWidget;

namespace Jam::Editor
{
    class AreaNode;
    class AreaCreator;

    // Holds the root of the node tree.

    class MainArea final : public QWidget
    {
        Q_OBJECT
    private:
        AreaNode*    _root{nullptr};
        AreaCreator* _creator{nullptr};
        QVBoxLayout* _layout{nullptr};

        void construct();

        void construct(const QString& layout);

        void construct(const XmlNode* node);

        void applyNode(XmlNode*        node,
                       const AreaNode* parent,
                       AreaNode*       current,
                       int             type,
                       int             ori);

        void applyLeaf(XmlNode*        node,
                       const AreaNode* parent,
                       AreaNode*       current,
                       int             type,
                       int             ori);

        void applyBranch(XmlNode*  node,
                         AreaNode* parent);

        void serialize(const AreaNode* source,
                       XmlNode*        destination);

        bool event(QEvent* event) override;

        static bool validateType(int& dest, XmlNode* fromNode);

        void handleBuildError(const char* message);

        ScopePtr<XmlNode*> serialize();

    public:
        explicit MainArea(const QString& layout = "",
                          QWidget*       parent = nullptr);
        ~MainArea() override;

        void dumpDisplayTree();

        void clear();

        AreaNode* root() const;

        void serialize(String& dest);

        void notify(QEvent* evt);
    };

    inline AreaNode* MainArea::root() const
    {
        return _root;
    }

}  // namespace Jam::Editor
