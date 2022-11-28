#include "WidgetTests.h"
#include "Interface/Area/Area.h"
#include "Interface/Area/AreaContent.h"
#include "Interface/Area/AreaLeaf.h"
#include "Interface/Area/AreaNode.h"
#include "Interface/MainArea.h"
#include "Interface/RememberLastCache.h"
#include "Interface/Widgets/IconButton.h"
#include "Math/Real.h"
#include "State/ProjectManager.h"

namespace Jam::Editor::Testing
{
    WidgetTests::WidgetTests() = default;

    void WidgetTests::initTestCase() const
    {
        QCOMPARE(_testWidget, nullptr);
    }

    void WidgetTests::testIcon()
    {
        _testWidget = IconButton::createToolButton(Icons::Add);

        const QPushButton* local = (QPushButton*)_testWidget;
        const QSize        sz    = local->size();

        QCOMPARE(local->isFlat(), true);

        QCOMPARE(sz.width(), 24);
        QCOMPARE(sz.height(), 24);
    }

    void WidgetTests::pathList()
    {
        RememberLastCache cache;
        QVERIFY(cache.size() == 0);
        QVERIFY(cache.at(-1).isEmpty());
        QVERIFY(cache.at(1000).isEmpty());

        cache.store("A");
        QVERIFY(cache.size() == 0);
        QVERIFY(cache.at(-1).isEmpty());
        QVERIFY(cache.at(1000).isEmpty());
        QVERIFY(cache.find("A") == -1);
    }

    void WidgetTests::cleanup()
    {
        delete _testWidget;
        _testWidget = nullptr;
    }

    void WidgetTests::cleanupTestCase() const
    {
        QCOMPARE(_testWidget, nullptr);
    }
}  // namespace Jam::Editor::Testing

QTEST_MAIN(Jam::Editor::Testing::WidgetTests)
