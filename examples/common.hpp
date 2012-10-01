#ifndef THREE_EXAMPLES_COMMON_HPP
#define THREE_EXAMPLES_COMMON_HPP

#include <three/config.hpp>

#include <three/extras/anim.hpp>
#include <three/extras/glew.hpp>
#include <three/extras/sdl.hpp>

#ifndef THREE_DATA_DIR
#define THREE_DATA_DIR "."
#endif

namespace three_examples {

inline const std::string& threeDataDir() {
  static std::string sThreeDataDir(THREE_DATA_DIR);
  return sThreeDataDir;
}

inline std::string threeDataPath( const std::string& relativePath ) {
  return threeDataDir() + "/" + relativePath;
}

} // namespace three_examples

#endif // THREE_EXAMPLES_COMMON_HPP