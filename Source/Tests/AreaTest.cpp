#include "AreaTest.h"
#include <QScrollArea>
#include "Interface/Area/AreaLeaf.h"
#include "Interface/Area/AreaNode.h"
#include "Interface/Area/AreaToolbar.h"
#include "Interface/Areas/AreaType.h"
#include "Interface/Areas/FunctionArea.h"
#include "Interface/Areas/FunctionAreaContent.h"
#include "Interface/MainArea.h"
#include "Interface/Style/MainStyle.h"
#include "Interface/Widgets/ExpressionWidget.h"
#include "Interface/Widgets/StackedPanel.h"
#include "Interface/Widgets/VariableWidget.h"
#include "State/App.h"
#include "State/FrameStackManager.h"
#include "Utils/Char.h"

namespace Jam::Editor::Testing
{
    class FunctionAreaApp final : public QMainWindow
    {
    private:
        MainArea*             _root{nullptr};
        FunctionArea*         _vfa{nullptr};
        FunctionArea*         _efa{nullptr};
        State::FunctionLayer* _fncLayer{nullptr};
        int                   _quitTestCount{0};
        String                _eventTest1Stack;

    public:
        FunctionAreaApp(MainArea* area, FunctionArea* vfa, FunctionArea* efa);

    private:
        void eventTest1();
        void eventTest2();
        void eventTest3();

        void runEventTests();

        void quitTest();
        void nextTest();

        void runSerializationTest();

        bool event(QEvent* event) override;
    };

    FunctionAreaApp::FunctionAreaApp(MainArea* area, FunctionArea* vfa, FunctionArea* efa) :
        _root(area),
        _vfa(vfa),
        _efa(efa)
    {
        _fncLayer = State::functionLayer();
        setCentralWidget(_root);
    }

    void FunctionAreaApp::eventTest1()
    {
        for (int i = 0; i < 10; ++i)
        {
            ExpressionWidget* widget = _efa->addExpression();
            QVERIFY(widget != nullptr);
            widget->setText("x + " + Char::toString(i));

            State::ExpressionStateObject* eso =
                _fncLayer->findExpression(widget->refId());

            QVERIFY(eso != nullptr);
            QVERIFY(eso->text() == widget->text());
            QVERIFY(eso->location() == _efa->refId());
            QVERIFY(eso->location() == 2);
        }

        for (int i = 0; i < 10; ++i)
        {
            VariableWidget* widget = _vfa->addSlider();
            QVERIFY(widget != nullptr);

            int    ch = (int)'a' + i;
            String v;
            v.push_back((char)ch);
            widget->setName(v);
            widget->setRange({-100, 100.f});
            widget->setValue(25.987f);
            widget->setRate(0.001f);

            State::VariableStateObject* vso =
                _fncLayer->findVariable(widget->refId());

            QVERIFY(vso != nullptr);
            QVERIFY(vso->name() == widget->data().name);
            QVERIFY(vso->location() == _vfa->refId());
            QVERIFY(vso->location() == 1);
            QVERIFY(equals(vso->range().x, -100));
            QVERIFY(equals(vso->range().y, 100));
            QVERIFY(equals(vso->range().y, 100));
            QVERIFY(equals(vso->value(), 25.987f));
            QVERIFY(equals(vso->rate(), 0.001f));
        }
        StringStream ss;
        State::layerStack()->save(ss);
        _eventTest1Stack = ss.str();

        nextTest();
    }

    void FunctionAreaApp::eventTest2()
    {
        State::layerStack()->clear();
        StringStream ls;
        ls << "<stack>";
        ls << "   <grid />";  //
        ls << "   <func>";    //
        ls << "   </func>";   //
        ls << "</stack>";
        State::layerStack()->load(ls);
        nextTest();
    }

    void FunctionAreaApp::eventTest3()
    {
        StringStream prev(_eventTest1Stack);
        State::layerStack()->load(prev);


        _root->setParent(nullptr);
        delete _root;

        StringStream ss;
        ss << R"(<tree>)";
        ss << R"(  <branch ratio="0.5">)";
        ss << R"(     <leaf type="3" ref="1"/>)";  // variables
        ss << R"(     <leaf type="3" ref="2"/>)";  // expressions
        ss << R"(  </branch>)";
        ss << R"(</tree>)";
        _root = new MainArea(ss.str());
        setCentralWidget(_root);
        nextTest();
    }

    void FunctionAreaApp::runEventTests()
    {
        _quitTestCount = 0;
        nextTest();
    }

    void FunctionAreaApp::quitTest()
    {
        QGuiApplication::postEvent(this, new QEvent(QEvent::Quit));
    }

    void FunctionAreaApp::nextTest()
    {
        ++_quitTestCount;
        QGuiApplication::postEvent(this, new QEvent((QEvent::Type)UnitTestEventId));
    }

    void FunctionAreaApp::runSerializationTest()
    {
        String dest;
        _root->serialize(dest);
        Console::writeLine(dest);

        StringStream ss;
        State::layerStack()->save(ss);
        State::layerStack()->clear();
        State::layerStack()->load(ss);
        ss.str({});
        ss.clear();

        State::layerStack()->save(ss);
        Console::writeLine(ss.str());
        quitTest();
    }

    bool FunctionAreaApp::event(QEvent* event)
    {
        int evtId = _quitTestCount;

        switch ((int)event->type())
        {
        case UnitTestEventId:
            if (_quitTestCount == 1)
                eventTest1();
            else if (_quitTestCount == 2)
                eventTest2();
            else if (_quitTestCount == 3)
                eventTest3();
            else if (_quitTestCount == 4)
                runSerializationTest();
            break;
        case LayerUpdate:
            _root->notify(event);
            return false;
        case QEvent::WindowActivate:
            runEventTests();
            return false;
        case QEvent::Quit:
            QGuiApplication::exit(0);
            return false;
        default:
            break;
        }
        return QMainWindow::event(event);
    }

    void AreaStateTest::initTestCase()
    {
        QCOMPARE(_test, nullptr);
        _test = nullptr;
        State::App::initialize();
        MainStyle::initialize();
    }

    void AreaStateTest::functionState()
    {
        // remove any default state
        State::layerStack()->clear();
        StringStream ls;
        ls << "<stack>";
        ls << "   <grid />";  //
        ls << "   <func>";    //
        ls << "   </func>";   //
        ls << "</stack>";
        State::layerStack()->load(ls);

        StringStream ss;
        ss << R"(<tree>)";
        ss << R"(  <branch ratio="0.5">)";
        ss << R"(     <leaf type="3" ref="1"/>)";  // variables
        ss << R"(     <leaf type="3" ref="2"/>)";  // expressions
        ss << R"(  </branch>)";
        ss << R"(</tree>)";
        _test = new MainArea(ss.str());

        const AreaNode* nd = _test->root();
        QVERIFY(nd->isBranch());

        const AreaNode* varFunctionBranch = nd->left();
        QVERIFY(varFunctionBranch->isLeaf());
        AreaLeaf* vc = (AreaLeaf*)varFunctionBranch->content();

        const AreaNode* expFunctionBranch = nd->right();
        QVERIFY(expFunctionBranch->isLeaf());
        AreaLeaf* ec = (AreaLeaf*)expFunctionBranch->content();

        FunctionArea* vfa = (FunctionArea*)vc->contents();
        QVERIFY(vfa->refId() == 1);

        FunctionArea* efa = (FunctionArea*)ec->contents();
        QVERIFY(efa->refId() == 2);

        FunctionAreaApp app(_test, vfa, efa);
        app.show();
        QApplication::exec();
        _test = nullptr;
    }

    void AreaStateTest::cleanup()
    {
        delete _test;
        _test = nullptr;
    }

    void AreaStateTest::cleanupTestCase()
    {
        State::App::finalize();
        QCOMPARE(_test, nullptr);
        _test = nullptr;
    }

}  // namespace Jam::Editor::Testing

QTEST_MAIN(Jam::Editor::Testing::AreaStateTest)
