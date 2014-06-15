#ifndef THREE_CONVERSION_UTILS_H
#define THREE_CONVERSION_UTILS_H

#include <three/config.h>

#include <array>
#include <sstream>
#include <string>

namespace three {

inline const std::string& threeDataDir() {
  static std::string sThreeDataDir(THREE_DATA_DIR);
  return sThreeDataDir;
}

inline std::string threeDataPath( const std::string& relativePath ) {
  auto path = threeDataDir();
  path += "/";
  path += relativePath;
  return path;
}

template < typename T >
inline std::array<T, 3> toArray( const T& t0, const T& t1, const T& t2 ) {
  std::array<T, 3> a = { t0, t1, t2 };
  return a;
}

template < typename T >
void* toOffset( T t ) {
  return reinterpret_cast<void*>( t );
}

inline int toInt( bool b ) {
  return b ? 1 : 0;
}

template < typename T, typename U >
inline std::string toString( const T& t, const U& u ) {
  std::stringstream ss;
  ss << t << u;
  return ss.str();
}

template < typename T, typename U >
inline std::string toString( const std::pair<T, U>& p ) {
  std::stringstream ss;
  ss << p.first << "_" << p.second;
  return ss.str();
}

#if THREE_HAS_VARIADIC_TEMPLATES
template < typename T, typename... Ts >
std::vector<T> toVector( const T& t0, const Ts&... ts ) {
	return std::vector<T> { t0, ts... };
}
#else
template < typename T >
std::vector<T> toVector( const T& t0 ) {
	return std::vector<T>( 1, t0 );
}
template < typename T >
std::vector<T> toVector( const T& t0, const T& t1 ) {
	std::vector<T> result;
	result.reserve(2);
	result.push_back(t0);
	result.push_back(t1);
	return result;
}
template < typename T >
std::vector<T> toVector( const T& t0, const T& t1, const T& t2 ) {
	std::vector<T> result;
	result.reserve(3);
	result.push_back(t0);
	result.push_back(t1);
	result.push_back(t2);
	return result;
}
template < typename T >
std::vector<T> toVector( const T& t0, const T& t1, const T& t2, const T& t3 ) {
	std::vector<T> result;
	result.reserve(4);
	result.push_back(t0);
	result.push_back(t1);
	result.push_back(t2);
	result.push_back(t3);
	return result;
}
template < typename T >
std::vector<T> toVector( const T& t0, const T& t1, const T& t2, const T& t3, const T& t4 ) {
	std::vector<T> result;
	result.reserve(5);
	result.push_back(t0);
	result.push_back(t1);
	result.push_back(t2);
	result.push_back(t3);
	result.push_back(t4);
	return result;
}
template < typename T >
std::vector<T> toVector( const T& t0, const T& t1, const T& t2, const T& t3, const T& t4, const T& t5 ) {
	std::vector<T> result;
	result.reserve(6);
	result.push_back(t0);
	result.push_back(t1);
	result.push_back(t2);
	result.push_back(t3);
	result.push_back(t4);
	result.push_back(t5);
	return result;
}
#endif

} // namespace three

#endif // THREE_CONVERSION_UTILS_H