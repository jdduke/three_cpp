#ifndef THREE_FONT_HPP
#define THREE_FONT_HPP

#include <three/fwd.hpp>

#include <three/utils/noncopyable.hpp>

#include <memory>

namespace three {
namespace utils {

class Font : NonCopyable {
public:

  typedef std::shared_ptr<Font> Ptr;

  THREE_DECL static Ptr create( const std::string& ttf,
                                float fontSize     = 30,
                                int textureWidth   = 512,
                                int textureHeight  = 512,
                                int firstCharacter = 32,
                                int countCharacter = 96);

  THREE_DECL void render( const char* text,
                          float x, float y,
                          float w, float h,
                          const Color& color );

  THREE_DECL ~Font();

protected:

  THREE_DECL Font();
  THREE_DECL bool initialize( const std::string& ttf,
                              float fontSize,
                              int textureWidth,
                              int textureHeight,
                              int firstCharacter,
                              int countCharacter );

private:
  struct Impl;
  std::unique_ptr<Impl> impl;

};

} // namespace utils
} // namespace three

#if defined(THREE_HEADER_ONLY)
# include <three/extras/utils/impl/font.ipp>
#endif // defined(THREE_HEADER_ONLY)

#endif // THREE_FONT_HPP