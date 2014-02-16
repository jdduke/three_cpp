#ifndef THREE_BOX3_HPP
#define THREE_BOX3_HPP

#include <three/common.hpp>

#include <three/math/math.hpp>

namespace three {

  class Box3 {
  public:

    union {
      struct { Vector3 min, max; };
      Vector3 minmax[2];
    };

    Box3() {
      min = Vector3( Math::INF(), Math::INF(), Math::INF() );
      max = Vector3( -Math::INF(), -Math::INF(), -Math::INF() );
    }
    Box3( Vector3 minIn, Vector3 maxIn ) : min( minIn ), max( maxIn ) { }
    
    explicit Box3( Vector3* values ) : min( values[0] ), max( values[1] ) { }

    Box3& set( const Vector3& minIn, const Vector3& maxIn ) {
      min.copy(minIn);
      max.copy(maxIn);
      return *this;
    }

    Box3& addPoint ( const Vector3& point ) {

      if ( point.x < min.x ) {
        min.x = point.x;
      } else if ( point.x > max.x ) {
        max.x = point.x;
      }

      if ( point.y < min.y ) {
        min.y = point.y;
      } else if ( point.y > max.y ) {
        max.y = point.y;
      }

      if ( point.z < min.z ) {
        min.z = point.z;
      } else if ( point.z > max.z ) {
        max.z = point.z;
      }

      return *this;
    }

    Box3& setFromPoints(const std::vector<Vector3>& points ) {
      if ( points.size() == 0 ) {
        makeEmpty();
        return *this;
      }

      min.copy( *points.begin() );
      max.copy( *points.begin() );

      for ( auto it = ++points.begin(); it != points.end(); it++ ) {
        addPoint(*it);
      }

      return *this;
    }

    Box3& setFromCenterAndSize( const Vector3& center, const Vector3& size ) {
      auto v1 = Vector3();
      auto halfSize = v1.copy( size ).multiplyScalar( 0.5 );
      min.copy( center ).sub( halfSize );
      max.copy( center ).add( halfSize );
      return *this;
    }

    Box3& setFromObject (const Object3D& object) {

      // Computes the world-axis-aligned bounding box of an object (including its children),
      // accounting for both the object's, and childrens', world transforms

      auto v1 = new Vector3();
      
      object.updateMatrixWorld( true );

      makeEmpty();

      auto scope = this;

      object.traverse( [&]( const Object3D& node ) {

        if ( node.geometry && node.geometry.vertices ) {

          auto vertices = node.geometry.vertices;

          for ( auto it = vertices.begin(); it != vertices.end(); it++ ) {
            v1.copy( *it );
            v1.applyMatrix4( node.matrixWorld );
            scope.expandByPoint( v1 );
          }

        }

      });

      return *this;
    }

    Box3& copy( const Box3& b ) {
      min.copy(b.min);
      max.copy(b.max);
      return *this;
    }

    Box3& makeEmpty() {
      min.x = min.y = min.z = Math::INF();
      max.x = max.y = max.z = -Math::INF();
      return *this;
    }

    bool empty() const {
      return ( max.x < min.x ) || ( max.y < min.y ) || ( max.z < min.z );
    }

    Vector3 center() const {
      return Vector3().addVectors( min, max ).multiplyScalar( 0.5 );
    }

    Vector3 center( const Vector3& target ) const {
      auto result = target;
      return result.addVectors( min, max ).multiplyScalar( 0.5 );
    }

    Vector3 size() const {
      return Vector3().subVectors( max, min );
    }

    Vector3 size( const Vector3& target ) const {
      auto result = target;
      return result.subVectors( min, max );
    }

    Box3& expandByPoint( const Vector3& point ) {
      min.min( point );
      max.max( point );
      return *this;
    }

    Box3& expandByVector( const Vector3& vector ) {
      min.sub( vector );
      max.add( vector );
      return *this;
    }

    Box3& expandByScalar( float scalar ) {
      min.addScalar( -scalar );
      max.addScalar( scalar );
      return *this;
    }

    bool containsPoint ( const Vector3& point ) const {
      if ( point.x < min.x || point.x > max.x ||
         point.y < min.y || point.y > max.y ||
         point.z < min.z || point.z > max.z ) {
        return false;
      }

      return true;
    }

    bool containsBox( const Box3& box ) const {
      if ( ( min.x <= box.min.x ) && ( box.max.x <= max.x ) &&
       ( min.y <= box.min.y ) && ( box.max.y <= max.y ) &&
       ( min.z <= box.min.z ) && ( box.max.z <= max.z ) ) {
        return true;
      }
      return false;
    }

    Vector3 getParameter( const Vector3& point ) const {
      // @todo mem check
      return getParameter(point, Vector3());
    }

    Vector3 getParameter( const Vector3& point, const Vector3& target ) const {
      auto result = target;
      auto divX = ( max.x - min.x );
      auto divY = ( max.y - min.y );
      auto divZ = ( max.z - min.z );
      return result.set(
        ( point.x - min.x ) / divX == 0 ? NEAR_ZERO_FLOAT : divX,
        ( point.y - min.y ) / divX == 0 ? NEAR_ZERO_FLOAT : divY,
        ( point.z - min.z ) / divZ == 0 ? NEAR_ZERO_FLOAT : divZ
      );
    }

    bool isIntersectionBox( const Box3& box ) const {
      // using 6 splitting planes to rule out intersections.
      if ( box.max.x < min.x || box.min.x > max.x ||
           box.max.y < min.y || box.min.y > max.y ||
           box.max.z < min.z || box.min.z > max.z ) {
        return false;
      }
      return true;
    }

    Vector3 clampPoint( const Vector3& point ) const {
    // @todo mem check
      return clampPoint(point, Vector3());
    }

    Vector3 clampPoint( const Vector3& point, const Vector3& target ) const {
      auto result = target;
      return result.copy( point ).clamp( min, max );
    }

    float distanceToPoint( const Vector3& point ) const {
      auto v1 = point;
      auto clampedPoint = v1.copy( point ).clamp( min, max );
      return clampedPoint.sub( point ).length();
    }

    Sphere getBoundingSphere() const {
      return getBoundingSphere(Sphere());
    }

    Sphere getBoundingSphere(const Sphere& target) const {
      auto v1 = Vector3();
      auto result = target;

      result.center = center();
      result.radius = size( v1 ).length() * 0.5;
      return result;
    }

    Box3& intersect( const Box3& box ) {
      min.max( box.min );
      max.min( box.max );
      return *this;
    }

    Box3& unionBox( const Box3& box ) {
      min.min( box.min );
      max.max( box.max );
      return *this;
    }

    Box3& applyMatrix4(const Matrix4& matrix)  {

      auto points = [
        Vector3(),
        Vector3(),
        Vector3(),
        Vector3(),
        Vector3(),
        Vector3(),
        Vector3(),
        Vector3()
      ];

      // NOTE: I am using a binary pattern to specify all 2^3 combinations below
      points[0].set( min.x, min.y, min.z ).applyMatrix4( matrix ); // 000
      points[1].set( min.x, min.y, max.z ).applyMatrix4( matrix ); // 001
      points[2].set( min.x, max.y, min.z ).applyMatrix4( matrix ); // 010
      points[3].set( min.x, max.y, max.z ).applyMatrix4( matrix ); // 011
      points[4].set( max.x, min.y, min.z ).applyMatrix4( matrix ); // 100
      points[5].set( max.x, min.y, max.z ).applyMatrix4( matrix ); // 101
      points[6].set( max.x, max.y, min.z ).applyMatrix4( matrix ); // 110
      points[7].set( max.x, max.y, max.z ).applyMatrix4( matrix );  // 111

      makeEmpty();
      setFromPoints( points );

      return *this;
    }

    Box3& translate( float& offset ) {
      min.addScalar( offset );
      max.addScalar( offset );
      return *this;
    }

    bool equals ( const Box3& box ) const {
      return box.min.equals( min ) && box.max.equals( max );
    }

    Box3 clone() const {
      return *this;
    }

  };

  static_assert( sizeof( Box3 ) == sizeof( Vector3 ) * 2, "Invalid Box3 storage size" );

} // namespace three

#endif // THREE_BOX3_HPP