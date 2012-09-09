#ifndef THREE_UTILS_HPP
#define THREE_UTILS_HPP

#include <algorithm>
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

// Workaround for using make_shared with protected constructors

template < typename T, typename... Args >
inline std::shared_ptr<T> make_shared( ) {
	struct Derived : public T { };
	return std::make_shared<Derived>( );
}

template < typename T >
struct Derived : public T {
	template < typename... Args >
	Derived( Args&&... args ) : T ( std::move(args)... ) { }
	template < typename... Args >
	Derived( const Args&... args ) : T ( args... ) { }
};

template < typename T, typename... Args >
inline std::shared_ptr<T> make_shared( Args&&... args ) {
	return std::make_shared<Derived<T>>( std::move(args)... );
}

/*template < typename T, typename... Args >
inline std::shared_ptr<T> make_shared( const Args&... args ) {
	return std::make_shared<Derived<T>>( args... );
}*/

} // namespace three

#endif // THREE_UTILS_HPP