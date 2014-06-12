#ifndef THREE_EXTRAS_ARROW_HELPER_CPP
#define THREE_EXTRAS_ARROW_HELPER_CPP

#include <three/common.h>

#include <three/math/math.h>

#include <three/extras/helpers/arrow_helper.h>

#include <three/materials/mesh_basic_material.h>
#include <three/materials/line_basic_material.h>

namespace three {

void ArrowHelper::setDirection ( const Vector3& dir ) {

  auto axis = Vector3();
  float radians;

  // dir is assumed to be normalized

  if ( dir.y > 0.99999 ) {

    this->quaternion().set( 0, 0, 0, 1 );

  } else if ( dir.y < -0.99999 ) {

  this->quaternion().set( 1, 0, 0, 0 );

  } else {

    axis.set( dir.z, 0, - dir.x ).normalize();

    radians = Math::acos( dir.y );

    this->quaternion().setFromAxisAngle( axis, radians );

  }

}

void ArrowHelper::setLength ( float length ) {

  auto headLength = length * 0.2;
  auto headWidth = headLength * 0.2;

  setLength( length, headLength, headWidth );
}

void ArrowHelper::setLength ( float length, float headLength ) {

  auto headWidth = headLength * 0.2;

  setLength( length, headLength, headWidth );
}

void ArrowHelper::setLength ( float length, float headLength, float headWidth ) {

  this->line->scale().set( 1, length, 1 );
  this->line->updateMatrix();

  this->cone->scale().set( headWidth, headLength, headWidth );
  this->cone->position().y = length;
  this->cone->updateMatrix();

}

void ArrowHelper::setColor ( int hex ) {

  this->line->material->color.set( hex );
  this->cone->material->color.set( hex );

};

ArrowHelper::ArrowHelper( const Vector3& dir, 
             const Vector3& origin, 
             float length , 
             int hex, 
             float headLength, 
             float headWidth ) {

  this->position() = origin;

  auto lineGeometry = Geometry::create();
  lineGeometry->vertices.push_back( Vector3( 0, 0, 0 ) );
  lineGeometry->vertices.push_back( Vector3( 0, 1, 0 ) );

    this->line = Line::create( lineGeometry, LineBasicMaterial::create( Material::Parameters().add( "color", hex ) ) );
  this->line->matrixAutoUpdate = false;
  this->add( this->line );

  auto coneGeometry = CylinderGeometry::create( 0, 0.5, 1, 5, 1 );
  coneGeometry->applyMatrix( Matrix4().makeTranslation( 0, - 0.5, 0 ) );

  this->cone = Mesh::create( coneGeometry, MeshBasicMaterial::create( Material::Parameters().add( "color", hex ) ) );
  this->cone->matrixAutoUpdate = false;
  this->add( this->cone );

  this->setDirection( dir );
  this->setLength( length, headLength, headWidth );

}

} // end namespace three

#endif // THREE_EXTRAS_ARROW_HELPER_CPP