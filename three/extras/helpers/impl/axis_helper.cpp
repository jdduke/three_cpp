#ifndef THREE_EXTRAS_AXIS_HELPER_CPP
#define THREE_EXTRAS_AXIS_HELPER_CPP

#include <three/extras/helpers/axis_helper.h>
#include <three/utils/conversion.h>

namespace three {

AxisHelper::Ptr AxisHelper::create( float sizeIn ) {
  return make_shared<AxisHelper>( sizeIn );
}

AxisHelper::AxisHelper( float sizeIn )
  : Line( nullptr, nullptr, THREE::LinePieces ), size( sizeIn ) {

  geometry = Geometry::create();

  geometry->vertices = toVector(
    Vector3(), Vector3( size, 0, 0 ),
    Vector3(), Vector3( 0, size, 0 ),
    Vector3(), Vector3( 0, 0, size )
  );

  geometry->colors = toVector(
    Color( 0xff0000 ), Color( 0xffaa00 ),
    Color( 0x00ff00 ), Color( 0xaaff00 ),
    Color( 0x0000ff ), Color( 0x00aaff )
  );

  material = LineBasicMaterial::create(
    Material::Parameters().add( "vertexColors", THREE::VertexColors )
  );
}

} // end namespace three

#endif // THREE_EXTRAS_AXIS_HELPER_CPP