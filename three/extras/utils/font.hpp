#ifndef THREE_FONT_HPP
#define THREE_FONT_HPP

#include <three/fwd.hpp>

#include <three/math/uv.hpp>
#include <three/textures/texture.hpp>
#include <three/utils/noncopyable.hpp>

#include <array>
#include <memory>

namespace three {

class Font : NonCopyable {
public:

  typedef std::shared_ptr<Font> Ptr;

  static Ptr create( const std::string& ttf,
                                float fontSize     = 30,
                                int textureWidth   = 512,
                                int textureHeight  = 512,
                                int firstCharacter = 32,
                                int countCharacter = 96 );

  void generate( const std::string& text,
                            std::vector<Vertex>& vertices,
                            std::vector<Face>& faces,
                            std::vector<std::array<UV,4>>& faceUvs );

  const Texture::Ptr& texture() const;

  ~Font();

protected:

  Font();
  bool initialize( const std::string& ttf,
                              float fontSize,
                              int textureWidth,
                              int textureHeight,
                              int firstCharacter,
                              int countCharacter );

private:
  struct Impl;
  std::unique_ptr<Impl> impl;

};

} // namespace three

#if defined(THREE_HEADER_ONLY)
# include <three/extras/utils/impl/font.ipp>
#endif // defined(THREE_HEADER_ONLY)

#endif // THREE_FONT_HPP