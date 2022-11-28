#include "MainAreaTest.h"
#include "Interface/Area/Area.h"
#include "Interface/Area/AreaContent.h"
#include "Interface/Area/AreaLeaf.h"
#include "Interface/Area/AreaNode.h"
#include "Interface/MainArea.h"
#include "State/App.h"

namespace Jam::Editor::Testing
{
    void MainAreaTest::initTestCase()
    {
        QCOMPARE(_test, nullptr);
        _test = nullptr;

        State::App::initialize();
    }

    void MainAreaTest::emptyTree()
    {
        _test = new MainArea("");

        const AreaNode* root = _test->root();
        QVERIFY(root != nullptr);

        QVERIFY(root->left() == nullptr);
        QVERIFY(root->right() == nullptr);
        QVERIFY(root->content() != nullptr);
        QVERIFY(root->content()->isLeaf());
        QVERIFY(root->content()->parentNode() == root);

        _test->dumpObjectTree();
    }

    void MainAreaTest::splitTree()
    {
        StringStream ss;
        ss << "<tree>";
        ss << " <branch ratio=\"0.5\">";
        ss << "   <leaf type=\"0\"/>";
        ss << "   <leaf type=\"1\"/>";
        ss << " </branch>";
        ss << "</tree>";
        _test = new MainArea(QString::fromStdString(ss.str()));

        const AreaNode* root = _test->root();
        QVERIFY(root != nullptr);

        QVERIFY(root->left() != nullptr);
        QVERIFY(root->right() != nullptr);
        QVERIFY(root->content() != nullptr);

        QVERIFY(root->content()->isBranch());
        QVERIFY(root->content()->parentNode() == root);

        QVERIFY(root->left()->isLeaf());
        QVERIFY(root->right()->isLeaf());

        const auto leftLeaf  = (AreaLeaf*)root->left()->content();
        const auto rightLeaf = (AreaLeaf*)root->right()->content();

        QVERIFY(leftLeaf->contents());
        QVERIFY(leftLeaf->contents()->type() == 0);

        QVERIFY(rightLeaf->contents());
        QVERIFY(rightLeaf->contents()->type() == 1);

        _test->dumpObjectTree();
    }

    void MainAreaTest::cleanup()
    {
        delete _test;
        _test = nullptr;
    }

    void MainAreaTest::cleanupTestCase()
    {
        State::App::finalize();
        QCOMPARE(_test, nullptr);
        _test = nullptr;
    }
}  // namespace Jam::Editor::Testing

QTEST_MAIN(Jam::Editor::Testing::MainAreaTest)
