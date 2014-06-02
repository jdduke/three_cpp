#include "gtest/gtest.h"
#include <tests/test_constants.h>

#include <three/core/event_dispatcher.h>

using namespace three;

typedef EventListener<Event> Listener;

class TestEvent : public Event {
public:
    
    TestEvent( int valIn ) : Event("testEvent"), val(valIn) {
        
    }
    
    int val;
};

void onSomeGlobalEvent( const Event& ev) {
    LOG("onSomeGlobalEvent called with event of type", ev.type);
}

class Button : public DefaultEventDispatcher {
public:

  Button() {}

  void dispose() {
    dispatchEvent("dispose");
  }
    
  void onSomeEvent( const Event& ev ) {
    LOG("Button::onSomeEvent called with event type", ev.type);
  }
    
    void onTestEventHandler( const Event& ev ) {
        LOG("Button::onTestEventHandler called with event", ev.type);
        auto te = static_cast<const TestEvent&>(ev);
        LOG("Button:onTestEventHandler TestEvent has value", te.val);
    }

};

TEST(core_event_listener_test, constructor) {
    
    auto a = Button();
    auto ev = Event("testConstructor");
    
    std::function<void(const Event&)> func = std::bind(&Button::onSomeEvent, a, std::placeholders::_1);
    auto funcListener = (Listener) func;
    
    auto bindExpression = std::bind(&Button::onSomeEvent, a, std::placeholders::_1);
    auto bindListener = (Listener) bindExpression;

    auto lambdaListener = (Listener) [](const Event& ev) {
        LOG("Lambda called with event type", ev.type);
    };

    auto functionPointerListener = (Listener) onSomeGlobalEvent;
    
    EXPECT_NO_FATAL_FAILURE( funcListener(ev) );
    EXPECT_NO_FATAL_FAILURE( lambdaListener(ev) );
    EXPECT_NO_FATAL_FAILURE( bindListener(ev) );
    EXPECT_NO_FATAL_FAILURE( functionPointerListener(ev) );
}

TEST(core_event_listener_test, comparison_operator) {
    auto listener1 = (Listener) [](const Event& ev) {};
    auto listener2 = listener1;
    
    EXPECT_EQ(listener1, listener2);
    EXPECT_NE(listener1, (Listener) [](const Event& ev) {});
}

TEST(core_event_listener_test, move_listener) {
    auto listener1 = (Listener) [](const Event& ev) {};
    auto listener2 = (Listener) [](const Event& ev) {};
    auto listener3 = std::move(listener1);
    
    EXPECT_NE(listener1, listener2);
    EXPECT_EQ(listener1, listener3);
}

TEST(core_event_listener_test, copy_assignment) {
    auto listener1 = (Listener) [](const Event& ev) {};
    auto listener2 = (Listener) [](const Event& ev) {};
    auto listener3 = listener1;
    
    EXPECT_NE(listener1, listener2);
    EXPECT_EQ(listener1, listener3);
}

TEST(core_event_dispatcher_test, addEventListener) {
    
    auto a = Button();
    
    a.addEventListener("click", (Listener) [](const Event& ev) {
        LOG("addEventListener 1 click event", ev.type);
    });
    
    a.addEventListener("click", (Listener) onSomeGlobalEvent );
    a.addEventListener("update", std::bind(&Button::onSomeEvent, a, std::placeholders::_1) );
    
    EXPECT_TRUE(a.hasEventListener("click"));
    EXPECT_TRUE(a.hasEventListener("update"));
}

TEST(core_event_dispatcher_test, removeEventListener) {
    
    auto a = Button();
    
    auto listener = a.addEventListener("click", [](const Event& ev) {} );
    a.removeEventListener("click", listener);
    
    EXPECT_FALSE(a.hasEventListener("click"));
    
    // This will result in has click is TRUE, because the two passed event listeners
    // are differint instantiations and thus have different ids
    a.addEventListener("click", (Listener) [](const Event& ev) {} );
    a.removeEventListener("click", (Listener) [](const Event& ev) {} );
    
    EXPECT_TRUE(a.hasEventListener("click"));
    
    a.removeEventListener("click");
    
    EXPECT_FALSE(a.hasEventListener("click"));
    
    auto listener1 = (Listener) [](const Event& ev) {};
    a.addEventListener("update", listener1);
    a.removeEventListener("update", listener1);

    EXPECT_FALSE(a.hasEventListener("update", listener1));
    EXPECT_FALSE(a.hasEventListener("update"));
}

TEST(core_event_dispatcher_test, removeEventListeners) {
    
    auto a = Button();

    a.addEventListener("click", (Listener) [](const Event& ev) { } );
   
    EXPECT_TRUE(a.hasEventListener("click"));
    
    a.removeEventListeners();
    
    EXPECT_FALSE(a.hasEventListener("click"));
}
