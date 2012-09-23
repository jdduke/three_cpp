#ifndef THREE_UTILS_HPP
#define THREE_UTILS_HPP

#include <three/config.hpp>

#include <algorithm>
#include <unordered_map>
#include <memory>

namespace three {

template < typename C, typename T >
inline bool contains( const C& c, const T& elem ) {
	return std::find( c.begin(), c.end(), elem ) != c.end();
}

template < typename C, typename T >
inline bool erase( C& c, const T& elem ) {
	auto i = std::find( c.begin(), c.end(), elem );
	if ( i != c.end() ) {
		c.erase( i );
		return true;
	}
	return false;
}

template < typename C, typename T >
inline bool push_unique( C& c, T&& elem ) {
	if ( !contains( c, elem ) ) {
		c.push_back( std::move(elem) );
		return true;
	}
	return false;
}

template < int I >
struct Int2Type {
  enum { value = I };
};

// Workaround for using make_shared with protected constructors

#if THREE_HAS_VARIADIC_TEMPLATES

template < typename T >
struct Derived : public T {
	template < typename... Args >
	Derived( Args&&... args ) : T ( std::move(args)... ) { }
	template < typename... Args >
	Derived( const Args&... args ) : T ( args... ) { }
};

template < typename T, typename... Args >
inline std::shared_ptr<T> make_shared( ) {
	struct Derived : public T { };
	return std::make_shared<Derived>( );
}

template < typename T, typename... Args >
inline std::shared_ptr<T> make_shared( Args&&... args ) {
	return std::make_shared<Derived<T>>( std::move(args)... );
}

#else // THREE_HAS_VARIADIC_TEMPLATES

template < typename T >
struct Derived : public T {
	Derived( )
		: T ( ) { }
	template < typename Arg0 >
	Derived( Arg0&& arg0 )
		: T ( std::move(arg0) ) { }
	template < typename Arg0, typename Arg1 >
	Derived( Arg0&& arg0, Arg1&& arg1)
		: T ( std::move(arg0), std::move(arg1) ) { }
	template < typename Arg0, typename Arg1, typename Arg2 >
	Derived( Arg0&& arg0, Arg1&& arg1, Arg2&& arg2)
		: T ( std::move(arg0), std::move(arg1), std::move(arg2) ) { }
	template < typename Arg0, typename Arg1, typename Arg2, typename Arg3 >
	Derived( Arg0&& arg0, Arg1&& arg1, Arg2&& arg2, Arg3&& arg3)
		: T ( std::move(arg0), std::move(arg1), std::move(arg2), std::move(arg3) ) { }
	template < typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4 >
	Derived( Arg0&& arg0, Arg1&& arg1, Arg2&& arg2, Arg3&& arg3, Arg4&& arg4)
		: T ( std::move(arg0), std::move(arg1), std::move(arg2), std::move(arg3), std::move(arg4) ) { }
	template < typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5 >
	Derived( Arg0&& arg0, Arg1&& arg1, Arg2&& arg2, Arg3&& arg3, Arg4&& arg4, Arg5&& arg5)
		: T ( std::move(arg0), std::move(arg1), std::move(arg2), std::move(arg3), std::move(arg4), std::move(arg5) ) { }
};

template < typename T >
inline std::shared_ptr<T> make_shared( ) {
	return std::make_shared<Derived<T>>( );
}
template < typename T, typename Arg0 >
inline std::shared_ptr<T> make_shared( Arg0&& arg0 ) {
	return std::make_shared<Derived<T>>( std::move(arg0) );
}
template < typename T, typename Arg0, typename Arg1 >
inline std::shared_ptr<T> make_shared( Arg0&& arg0, Arg1&& arg1 ) {
	return std::make_shared<Derived<T>>( std::move(arg0), std::move(arg1) );
}
template < typename T, typename Arg0, typename Arg1, typename Arg2 >
inline std::shared_ptr<T> make_shared( Arg0&& arg0, Arg1&& arg1, Arg2&& arg2) {
	return std::make_shared<Derived<T>>( std::move(arg0), std::move(arg1), std::move(arg2) );
}
template < typename T, typename Arg0, typename Arg1, typename Arg2, typename Arg3 >
inline std::shared_ptr<T> make_shared( Arg0&& arg0, Arg1&& arg1, Arg2&& arg2, Arg3&& arg3 ) {
	return std::make_shared<Derived<T>>( std::move(arg0), std::move(arg1), std::move(arg2), std::move(arg3) );
}
template < typename T, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4 >
inline std::shared_ptr<T> make_shared( Arg0&& arg0, Arg1&& arg1, Arg2&& arg2, Arg3&& arg3, Arg4&& arg4 ) {
	return std::make_shared<Derived<T>>( std::move(arg0), std::move(arg1), std::move(arg2), std::move(arg3), std::move(arg4) );
}
template < typename T, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5 >
inline std::shared_ptr<T> make_shared( Arg0&& arg0, Arg1&& arg1, Arg2&& arg2, Arg3&& arg3, Arg4&& arg4, Arg5&& arg5 ) {
	return std::make_shared<Derived<T>>( std::move(arg0), std::move(arg1), std::move(arg2), std::move(arg3), std::move(arg4), std::move(arg5) );
}

#endif // THREE_HAS_VARIADIC_TEMPLATES

} // namespace three

#endif // THREE_UTILS_HPP