#ifndef THREE_SDL_HPP
#define THREE_SDL_HPP

#include <three/core/color.hpp>
#include <three/utils/noncopyable.hpp>
#include <three/renderers/renderer_parameters.hpp>

#include <SDL_events.h>

#include <functional>

namespace three {
namespace sdl {

typedef SDL_Event                         Event;
typedef SDL_EventType                     EventType;
typedef std::pair<SDL_EventType, int>     EventKey;
typedef std::function<void(const Event&)> EventListener;

/////////////////////////////////////////////////////////////////////////

THREE_DECL bool init( RendererParameters& parameters );
THREE_DECL bool swapBuffers();
THREE_DECL void quit();

/////////////////////////////////////////////////////////////////////////

THREE_DECL EventKey addEventListener( EventType, EventListener );
THREE_DECL void removeEventListener( EventKey );
THREE_DECL void clearEventListeners();
THREE_DECL void processEvents();

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
# include <three/extras/impl/sdl.ipp>
#endif // defined(THREE_HEADER_ONLY)

#endif // THREE_SDL_HPP