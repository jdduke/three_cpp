#ifndef THREE_CONVERSION_UTILS_HPP
#define THREE_CONVERSION_UTILS_HPP

#include <three/config.hpp>

#include <array>
#include <sstream>
#include <string>

namespace three {

inline const std::string& threeDataDir()
{
    static std::string sThreeDataDir(THREE_DATA_DIR);
    return sThreeDataDir;
}

inline std::string threeDataPath(const std::string& relativePath)
{
    auto path = threeDataDir();
    path += "/";
    path += relativePath;
    return path;
}

/////////////////////////////////////////////////////////////////////////

template <typename T>
inline std::array<T, 4> toArray(const T& t0, const T& t1, const T& t2, const T& t3 = T())
{
    std::array<T, 4> a = { t0, t1, t2, t3 };
    return a;
}

template <typename T>
void* toOffset(T t) { return reinterpret_cast<void*>(t); }

inline int toInt(bool b) { return b ? 1 : 0; }

template <typename T, typename U>
inline std::string toString(const T& t, const U& u)
{
    std::stringstream ss;
    ss << t << u;
    return ss.str();
}

template <typename T, typename U>
inline std::string toString(const std::pair<T, U>& p)
{
    std::stringstream ss;
    ss << p.first << "_" << p.second;
    return ss.str();
}

} // namespace three

#endif // THREE_UTILS_HPP