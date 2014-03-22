#ifndef THREE_EVENTS_H
#define THREE_EVENTS_H

#include <functional>

namespace three {
namespace events {

enum EventType {
  KeyDownEvent = 0,
  KeyUpEvent,
  MouseDownEvent,
  MouseUpEvent,
  MouseMoveEvent,
  MouseScrollEvent,
  ResizeEvent,
  QuitEvent
};

enum MouseButtonType {
  LeftMouseButton = 0,
  MiddleMouseButton,
  RightMouseButton,
  ScrollMouseButton
};

struct KeyboardEvent {
  char type;
  char state;
};

}

class EventHandler {
public:
  static EventHandler& instance();

  EventKey addEventListener( EventType, EventListener ) = 0;
  void removeEventListener( EventKey ) = 0;
  void clearEventListeners() = 0;
  void processEvents() = 0;
};

/////////////////////////////////////////////////////////////////////////

class ScopedEventListener : public NonCopyable {
public:
  ScopedEventListener( EventType, EventListener );
  ~ScopedEventListener();
private:
  EventKey eventKey;
};

} // namespace sdl
} // namespace three

#if defined(THREE_HEADER_ONLY)
# include <three/extras/impl/events.cpp>
#endif // defined(THREE_HEADER_ONLY)

#endif // THREE_SDL_H