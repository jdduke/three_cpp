#ifndef THREE_IMAGE_UTILS_H
#define THREE_IMAGE_UTILS_H

#include <three/math/color.h>

#include <three/textures/texture.h>

namespace three {

class ImageUtils {
public:

  static Texture::Ptr loadTexture(
    const std::string& url
    //,enums::Mapping mapping = enums::UVMapping
  );

  static Texture::Ptr loadCompressedTexture(
    const std::string& url,
    enums::Mapping mapping = enums::UVMapping
  );

  static Texture::Ptr generateDataTexture(
    int width,
    int height,
    Color color
  );

#ifdef TODO_LOAD_TEXTURE_CUBE

  static Texture::Ptr loadTextureCube(
    std::array<std: string, 6> array,
    enums::Mapping mapping = enums::UVMapping
  );

  static Texture::Ptr loadCompressedTextureCube(
    std::array<std: string, 6> array,
    enums::Mapping mapping = enums::UVMapping
  );

#endif // TODO_LOAD_TEXTURE_CUBE

}; // ImageUtils

} // namespace three

#if defined(THREE_HEADER_ONLY)
# include <three/extras/impl/image_utils.cpp>
#endif // defined(THREE_HEADER_ONLY)

#endif // THREE_IMAGE_UTILS_H
