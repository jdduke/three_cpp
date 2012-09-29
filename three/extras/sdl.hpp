#ifndef THREE_SDL_HPP
#define THREE_SDL_HPP

#include <three/renderers/renderer_parameters.hpp>

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

namespace three {
namespace sdl {

inline bool init( RendererParameters& parameters ) {

  if ( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
    console().error() << "Error initializing SDL: " << SDL_GetError();
    return false;
  }

  console().log() << "SDL initialized";

  if ( SDL_SetVideoMode( parameters.width, parameters.height, parameters.precision, SDL_OPENGL ) == NULL ) {
    console().error() << "Error setting SDL video mode: " << SDL_GetError();
    return false;
  }

  console().log() << "SDL video initialized";

  SDL_WM_SetCaption( "Three.cpp", NULL );

  return true;
}

inline bool swapBuffers() {
  SDL_GL_SwapBuffers();
  return true;
}

std::pair<Image, THREE::PixelFormat> inline loadImage( const std::string& path ) {

  SDL_Surface* surface = IMG_Load( path.c_str() );
  if( nullptr == surface ) {
    console().error() << "three::sdl::loadImage: Unable to load " << path;
    return std::pair<Image, THREE::PixelFormat>();
  }

  const auto width = surface->w;
  const auto height = surface->h;
  if ( !Math::isPowerOfTwo( width ) ) {
    console().warn() << "three::sdl::LoadImage: " << path << " width is not a power of two.";
  }
  if ( !Math::isPowerOfTwo( height ) ) {
    console().warn() << "three::sdl::LoadImage: " << path << " width is not a power of two.";
  }

  THREE::PixelFormat format = THREE::RGBFormat;
  const auto nOfColors = surface->format->BytesPerPixel;
  if (nOfColors == 4) {
    if (surface->format->Rmask == 0x000000ff)
      format = THREE::RGBAFormat;
    else
      format = THREE::BGRAFormat;
  } else if (nOfColors == 3) {
    if (surface->format->Rmask == 0x000000ff)
      format = THREE::RGBFormat;
    else
      format = THREE::BGRFormat;
  } else {
    console().warn() << "three::sdl::loadImage: The specified image " << path << " is not truecolor, and is not guaranteed to work";
  }

  const auto bytes = surface->pitch * height;
  Image image(std::vector<unsigned char>((unsigned char*)surface->pixels,
                                         (unsigned char*)surface->pixels + bytes),
              width,
              height);

  SDL_FreeSurface( surface );

  return std::make_pair( std::move(image), format );

#ifdef TODO_SDL_IMAGE_SCALING

  #if SDL_BYTEORDER == SDL_BIG_ENDIAN
    const auto rmask = 0xff000000;
    const auto gmask = 0x00ff0000;
    const auto bmask = 0x0000ff00;
    const auto amask = 0x000000ff;
#else
    const auto rmask = 0x000000ff;
    const auto gmask = 0x0000ff00;
    const auto bmask = 0x00ff0000;
    const auto amask = 0xff000000;
#endif

  const auto widthPow = Math::upperPowerOfTwo( width );
  const auto heightPow = Math::upperPowerOfTwo( height );

  // Create new empty surface.
  SDL_Surface* newSurface = SDL_CreateRGBSurface( SDL_SRCALPHA,
                                                  widthPow, heightPow, 32,
                                                  rmask, bmask, gmask, amask );

  // Fill sprite with alpha.
  auto alpha = SDL_MapRGBA( format, 0, 0, 0, amask );
  SDL_Rect rect;
  rect.x = 0;
  rect.y = 0;
  rect.h = heightPow;
  rect.w = widthPow;
  int ret = SDL_FillRect( newSurface, &rect, alpha);
  surface->flags &= !SDL_SRCALPHA;

  SDL_SetAlpha( newSurface, SDL_SRCALPHA, SDL_ALPHA_TRANSPARENT );

  // Copy image data to our new surface.
  ret = SDL_BlitSurface( surface, 0, newSurface, 0 );
#endif

}

} // namespace sdl
} // namespace three

#endif // THREE_SDL_HPP