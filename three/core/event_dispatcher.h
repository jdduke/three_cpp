#ifndef THREE_EVENT_DISPATCHER_H
#define THREE_EVENT_DISPATCHER_H

#include <three/common.h>
#include <three/core/event_listener.h>
#include <three/events/event.h>

#include <unordered_map>
#include <vector>
#include <algorithm>
#include <functional>

namespace three {
  
template<typename TKey, typename TEvent>
class EventDispatcher {
public:
  
  EventListener<TEvent> addEventListener( const TKey& type, const std::function<void(const TEvent&)>& func ) {
    
    auto listener = (EventListener<TEvent>) func;

    if(!hasEventListener(type, listener)) {
        listeners[ type ].emplace_back( std::move(listener) );
    }
    
    return listener;
  }

  void addEventListener( const TKey& type, EventListener<TEvent>&& listener ) {
    if(!hasEventListener(type, listener)) {
      listeners[ type ].emplace_back( std::forward<EventListener<TEvent>>( listener ) );
    }
  }
  
  void addEventListener( const TKey& type, EventListener<TEvent>& listener ) {
    if(!hasEventListener(type, listener)) {
      listeners[ type ].emplace_back( std::move( listener ) );
    }
  }

  bool hasEventListener( const TKey& type ) const {
    return listeners.find( type ) != listeners.cend() ;
  }

  bool hasEventListener( const TKey& type, const EventListener<TEvent>& listener ) const {

    auto typeListeners = listeners.find( type );
    if( typeListeners  == listeners.end() ) {
      return false;
    } else {
      return std::find(typeListeners->second.cbegin(), typeListeners->second.cend(), listener) != typeListeners->second.end();
    }
    
  }

  void removeEventListeners() {
    listeners.clear();
  }

  void removeEventListener( const TKey& type ) {
    auto it = listeners.find( type );

    if(it != listeners.end()) {
      it->second.clear();
      listeners.erase(it);
    }

  }

  void removeEventListener( const TKey& type, const EventListener<TEvent>& listener ) {

    auto& typeListeners = listeners[ type ];
    auto it = std::find(typeListeners.cbegin(), typeListeners.cend(), listener);
    
    if( it != typeListeners.cend() ) {

      typeListeners.erase( it );

      if( typeListeners.empty() ){
        removeEventListener( type );
      }

    }

  }
  
  void dispatchEvent( const TEvent& event ) const {

    auto typeListeners = listeners.find( event.type );

    if(typeListeners != listeners.cend()) {

      for ( const auto& listener : typeListeners->second ) {
        listener( event );
      }

    }

  }

  void dispatchEvent( TEvent&& event ) const {
    dispatchEvent( std::forward<TEvent>( event ) );
  }

protected:

  std::unordered_map<TKey, std::vector<EventListener<TEvent>>> listeners;

};

class DefaultEventDispatcher : public EventDispatcher<std::string, Event> {
public:

  void dispatchEvent( Event& event ) {
    
    auto typeListeners = listeners.find( event.type );
    event.target = this;
    
    if(typeListeners != listeners.cend()) {

      for ( const auto& listener : typeListeners->second ) {
        listener( event );
      }

    }
    
  }

  void dispatchEvent( const Event& event ) {
    auto ev = std::move(event);
    dispatchEvent(ev);
  }
  
  void dispatchEvent( const EventType& type ) {
    auto ev = Event( std::move(type) );
    dispatchEvent( ev );
  }

};
  
} // namespace three

#endif // THREE_EVENT_DISPATCHER_H