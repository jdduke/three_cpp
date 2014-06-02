#ifndef THREE_EVENT_LISTENER_H
#define THREE_EVENT_LISTENER_H

#include <three/common.h>

#include <three/events/event.h>

#include <functional>

namespace three {

template<typename TEvent>
class EventListener {
public:

  typedef std::function<void(const TEvent&)> EventListenerFunc;

  EventListener( const EventListenerFunc& funcIn )
  : id( EventListenerCount()++ ),
    func( std::move(funcIn) ) {}

  EventListener( const EventListener& other )
  : id( other.id ),
    func( std::move(other.func) ) {}

  EventListener( EventListener&& other )
  : id( other.id ),
    func( std::forward<EventListenerFunc>(other.func) ) {}


  EventListener& operator=( const EventListener& other ) {
    id = other.id;
    func = std::move(other.func);
    return *this;
  }

  EventListener& operator=( EventListener&& other ) {
    id = other.id;
    func = std::forward<EventListenerFunc>(other.func);
    return *this;
  }

  void operator()( const TEvent& event ) const { func(event); }

  bool operator==( const EventListener& listener ) const { return id == listener.id; }
  bool operator!=( const EventListener& listener ) const { return id != listener.id; }

private:
  
  static int& EventListenerCount() {
    static int sEventListenerCount = 0;
    return sEventListenerCount;
  }
    
  unsigned int id;

  EventListenerFunc func;

};

} // end namespace three 

#endif // THREE_EVENT_LISTENER_H