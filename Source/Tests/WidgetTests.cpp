#include "WidgetTests.h"
#include <QPushButton>
#include "Interface/Area/AreaNode.h"
#include "Interface/MainArea.h"
#include "Interface/RememberLastCache.h"
#include "Interface/Style/Style.h"
#include "Interface/Widgets/I32Widget.h"
#include "Interface/Widgets/SliderWidget.h"
#include "Interface/Widgets/StringWidget.h"

namespace Jam::Editor::Testing
{
    WidgetTests::WidgetTests() = default;

    void WidgetTests::initTestCase() const
    {
        QCOMPARE(_test, nullptr);
    }

    void WidgetTests::testIcon()
    {
        _test = Style::toolButton(Icons::Add);

        const auto local = (QPushButton*)_test;
        const auto size  = local->size();

        QCOMPARE(local->isFlat(), true);
        QCOMPARE(size.width(), 24);
        QCOMPARE(size.height(), 24);
    }

    void WidgetTests::testI32()
    {
        const auto val = new I32Widget();
        _test          = val;
        val->setRange(-1, 1);
        val->setValue(111);
        QCOMPARE(val->value(), 1);
        QCOMPARE(val->string(), "1");
    }

    void WidgetTests::testString()
    {
        const auto val = new StringWidget();
        _test          = val;
        static int nr, nr2;
        nr = nr2 = 0;
        connect(val, &StringWidget::editingFinished, this, [=]
                { ++nr; });
        val->setText("111");
        val->setText("111");
        QCOMPARE(val->text(), "111");

        val->setText("222");
        QCOMPARE(val->text(), "222");
        val->setText("111");
        val->setText("111");
        QCOMPARE(val->text(), "111");
        val->setText("\t\r\n111\r\n\t");
        QCOMPARE(val->text(), "111");
        QCOMPARE(nr, 4);

        delete _test;
        _test = nullptr;

        const auto val2 = new StringWidget();
        _test           = val2;
        connect(val2, &StringWidget::editingFinished, this, [=]
                { ++nr2; });
        val2->setText("... ('nr' should not increment)");
        QCOMPARE(nr, 4);
        QCOMPARE(nr2, 1);
    }

    void WidgetTests::testSlider()
    {
        const auto val = new SliderWidget();

        _test = val;

        val->setRange(-1, 1);
        val->setValue(111);
        val->setLabel("ABC");
        QCOMPARE(val->label(), "A");
        val->setLabel("abc");
        QCOMPARE(val->label(), "a");
        val->setLabel("0a");
        QCOMPARE(val->label(), "a");
        val->setLabel("((*&^%$#@!1234567890,<>?|}{[\\]a");
        QCOMPARE(val->label(), "a");

        QCOMPARE(val->range().x, -1.f);
        QCOMPARE(val->range().y, 1.f);
        QCOMPARE(val->value(), 1);
        QCOMPARE(val->string(), "1");
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
        delete _test;
        _test = nullptr;
    }

    void WidgetTests::cleanupTestCase() const
    {
        QCOMPARE(_test, nullptr);
    }
}  // namespace Jam::Editor::Testing

QTEST_MAIN(Jam::Editor::Testing::WidgetTests)
