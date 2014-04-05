#ifndef THREE_MEMORY_UTILS_H
#define THREE_MEMORY_UTILS_H

#include <memory>

namespace three {

using std::shared_ptr;

#if THREE_HAS_TEMPLATE_ALIAS

template < typename T >
using Ptr = shared_ptr<T>;

#else

template < typename T >
struct Ptr : public shared_ptr<T> { };

#endif

// Workaround for using make_shared with protected constructors

#if THREE_HAS_VARIADIC_TEMPLATES

template < typename T >
struct Derived : public T {
  template < typename... Args >
  Derived( Args&& ... args ) : T( std::move( args )... ) { }
  template < typename... Args >
  Derived( const Args& ... args ) : T( args... ) { }
};

///*
template < typename T >
inline shared_ptr<T> make_shared( ) {
  struct Derived : public T { };
  return std::make_shared<Derived>( );
}
//*/

template < typename T, typename... Args >
inline shared_ptr<T> make_shared( Args&& ... args ) {
  return std::make_shared<Derived<T>>( std::move( args )... );
}

#else // THREE_HAS_VARIADIC_TEMPLATES

template < typename T >
struct Derived : public T {
  Derived( )
    : T( ) { }
  template < typename Arg0 >
  Derived( const Arg0& arg0 )
    : T( arg0 ) { }
  template < typename Arg0 >
  Derived( Arg0 && arg0 )
    : T( std::move( arg0 ) ) { }
  template < typename Arg0, typename Arg1 >
  Derived( Arg0 && arg0, Arg1 && arg1 )
    : T( std::move( arg0 ), std::move( arg1 ) ) { }
  template < typename Arg0, typename Arg1, typename Arg2 >
  Derived( Arg0 && arg0, Arg1 && arg1, Arg2 && arg2 )
    : T( std::move( arg0 ), std::move( arg1 ), std::move( arg2 ) ) { }
  template < typename Arg0, typename Arg1, typename Arg2, typename Arg3 >
  Derived( Arg0 && arg0, Arg1 && arg1, Arg2 && arg2, Arg3 && arg3 )
    : T( std::move( arg0 ), std::move( arg1 ), std::move( arg2 ), std::move( arg3 ) ) { }
  template < typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4 >
  Derived( Arg0 && arg0, Arg1 && arg1, Arg2 && arg2, Arg3 && arg3, Arg4 && arg4 )
    : T( std::move( arg0 ), std::move( arg1 ), std::move( arg2 ), std::move( arg3 ), std::move( arg4 ) ) { }
  template < typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5 >
  Derived( Arg0 && arg0, Arg1 && arg1, Arg2 && arg2, Arg3 && arg3, Arg4 && arg4, Arg5 && arg5 )
    : T( std::move( arg0 ), std::move( arg1 ), std::move( arg2 ), std::move( arg3 ), std::move( arg4 ), std::move( arg5 ) ) { }

  template < typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6 >
  Derived( Arg0 && arg0, Arg1 && arg1, Arg2 && arg2, Arg3 && arg3, Arg4 && arg4, Arg5 && arg5, Arg6 && arg6 )
    : T( std::move( arg0 ), std::move( arg1 ), std::move( arg2 ), std::move( arg3 ), std::move( arg4 ), std::move( arg5 ), std::move( arg6 ) ) { }

  template < typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7 >
  Derived( Arg0 && arg0, Arg1 && arg1, Arg2 && arg2, Arg3 && arg3, Arg4 && arg4, Arg5 && arg5, Arg6 && arg6, Arg7 && arg7 )
    : T( std::move( arg0 ), std::move( arg1 ), std::move( arg2 ), std::move( arg3 ), std::move( arg4 ), std::move( arg5 ), std::move( arg6 ), std::move( arg7 ) ) { }

  template < typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8 >
  Derived( Arg0 && arg0, Arg1 && arg1, Arg2 && arg2, Arg3 && arg3, Arg4 && arg4, Arg5 && arg5, Arg6 && arg6, Arg7 && arg7, Arg8 && arg8 )
    : T( std::move( arg0 ), std::move( arg1 ), std::move( arg2 ), std::move( arg3 ), std::move( arg4 ), std::move( arg5 ), std::move( arg6 ), std::move( arg7 ), std::move( arg8 ) ) { }

};

template < typename T >
inline shared_ptr<T> make_shared( ) {
  return std::make_shared<Derived<T>>( );
}
template < typename T, typename Arg0 >
inline shared_ptr<T> make_shared( const Arg0& arg0 ) {
  return std::make_shared<Derived<T>>( arg0 );
}
template < typename T, typename Arg0 >
inline shared_ptr<T> make_shared( Arg0 && arg0 ) {
  return std::make_shared<Derived<T>>( std::move( arg0 ) );
}
template < typename T, typename Arg0, typename Arg1 >
inline shared_ptr<T> make_shared( Arg0 && arg0, Arg1 && arg1 ) {
  return std::make_shared<Derived<T>>( std::move( arg0 ), std::move( arg1 ) );
}
template < typename T, typename Arg0, typename Arg1, typename Arg2 >
inline shared_ptr<T> make_shared( Arg0 && arg0, Arg1 && arg1, Arg2 && arg2 ) {
  return std::make_shared<Derived<T>>( std::move( arg0 ), std::move( arg1 ), std::move( arg2 ) );
}
template < typename T, typename Arg0, typename Arg1, typename Arg2, typename Arg3 >
inline shared_ptr<T> make_shared( Arg0 && arg0, Arg1 && arg1, Arg2 && arg2, Arg3 && arg3 ) {
  return std::make_shared<Derived<T>>( std::move( arg0 ), std::move( arg1 ), std::move( arg2 ), std::move( arg3 ) );
}
template < typename T, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4 >
inline shared_ptr<T> make_shared( Arg0 && arg0, Arg1 && arg1, Arg2 && arg2, Arg3 && arg3, Arg4 && arg4 ) {
  return std::make_shared<Derived<T>>( std::move( arg0 ), std::move( arg1 ), std::move( arg2 ), std::move( arg3 ), std::move( arg4 ) );
}
template < typename T, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5 >
inline shared_ptr<T> make_shared( Arg0 && arg0, Arg1 && arg1, Arg2 && arg2, Arg3 && arg3, Arg4 && arg4, Arg5 && arg5 ) {
  return std::make_shared<Derived<T>>( std::move( arg0 ), std::move( arg1 ), std::move( arg2 ), std::move( arg3 ), std::move( arg4 ), std::move( arg5 ) );
}

template < typename T, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5 , typename Arg6, typename Arg7  >
inline shared_ptr<T> make_shared( Arg0 && arg0, Arg1 && arg1, Arg2 && arg2, Arg3 && arg3, Arg4 && arg4, Arg5 && arg5, Arg6 && arg6, Arg7 && arg7 ) {
  return std::make_shared<Derived<T>>( std::move( arg0 ), std::move( arg1 ), std::move( arg2 ), std::move( arg3 ), std::move( arg4 ), std::move( arg5 ), std::move( arg6 ), std::move( arg7 )   );
}

template < typename T, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5 , typename Arg6, typename Arg7, typename Arg8   >
inline shared_ptr<T> make_shared( Arg0 && arg0, Arg1 && arg1, Arg2 && arg2, Arg3 && arg3, Arg4 && arg4, Arg5 && arg5, Arg6 && arg6, Arg7 && arg7, Arg8 && arg8 ) {
  return std::make_shared<Derived<T>>( std::move( arg0 ), std::move( arg1 ), std::move( arg2 ), std::move( arg3 ), std::move( arg4 ), std::move( arg5 ), std::move( arg6 ), std::move( arg7 ), std::move( arg8 )   );
}

#endif // THREE_HAS_VARIADIC_TEMPLATES

} // namespace three

#endif // THREE_UTILS_H