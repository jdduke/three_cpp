#ifndef THREE_EVENTS_HPP
#define THREE_EVENTS_HPP

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
  THREE_DECL static EventHandler& instance();

  THREE_DECL EventKey addEventListener( EventType, EventListener ) = 0;
  THREE_DECL void removeEventListener( EventKey ) = 0;
  THREE_DECL void clearEventListeners() = 0;
  THREE_DECL void processEvents() = 0;
};

/////////////////////////////////////////////////////////////////////////

class ScopedEventListener : public NonCopyable {
public:
  THREE_DECL ScopedEventListener( EventType, EventListener );
  THREE_DECL ~ScopedEventListener();
private:
  EventKey eventKey;
};

} // namespace sdl
} // namespace three

#if defined(THREE_HEADER_ONLY)
# include <three/extras/impl/events.ipp>
#endif // defined(THREE_HEADER_ONLY)

#endif // THREE_SDL_HPP