#ifndef THREE_FONT2_HPP
#define THREE_FONT2_HPP

#include <three/fwd.hpp>

#include <three/core/uv.hpp>
#include <three/core/vertex.hpp>
#include <three/textures/texture.hpp>
#include <three/utils/noncopyable.hpp>

#include <array>
#include <memory>

namespace three {

class Font : NonCopyable {
public:

  typedef std::shared_ptr<Font> Ptr;

  THREE_DECL static Ptr create( const std::string& ttf,
                                float fontSize     = 30,
                                int textureWidth   = 512,
                                int textureHeight  = 512,
                                int firstCharacter = 32,
                                int countCharacter = 96 );

  THREE_DECL void generate( const std::string& text,
                            std::vector<Vertex>& vertices,
                            std::vector<Face>& faces,
                            std::vector<std::array<UV,4>>& faceUvs );

  THREE_DECL const Texture::Ptr& texture() const;

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

} // namespace three

#if defined(THREE_HEADER_ONLY)
# include <three/extras/utils/impl/font2.ipp>
#endif // defined(THREE_HEADER_ONLY)

#endif // THREE_FONT2_HPP