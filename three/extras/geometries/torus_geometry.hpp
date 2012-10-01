#ifndef THREE_TORUS_GEOMETRY_HPP
#define THREE_TORUS_GEOMETRY_HPP

#include <three/core/geometry.hpp>
#include <three/core/face3.hpp>
#include <three/core/face4.hpp>
#include <three/core/math.hpp>

namespace three {

class TorusGeometry : public Geometry {
public:

  typedef std::shared_ptr<TorusGeometry> Ptr;

  static Ptr create( float radius = 50,
                     float tube = 40,
                     int segmentsR = 8,
                     int segmentsT = 6,
                     float arc = Math::PI() * 2 ) {

    auto geometry = make_shared<TorusGeometry>();
    geometry->initialize( radius,
                          tube,
                          segmentsR,
                          segmentsT,
                          arc );
    return geometry;
  }

protected:

  TorusGeometry() : Geometry() { }

  void initialize( float radius,
                   float tube,
                   int segmentsR,
                   int segmentsT,
                   float arc ) {

    Vector3 center;

    std::vector<UV> tmpUvs;
    std::vector<Vector3> tmpNormals;

    for ( int j = 0; j <= segmentsR; j++ ) {

      for ( int i = 0; i <= segmentsT; i++ ) {

        const auto u = (float)i / segmentsT * arc;
        const auto v = (float)j / segmentsR * Math::PI() * 2;

        center.x = radius * Math::cos( u );
        center.y = radius * Math::sin( u );

        Vector3 vertex;
        vertex.x = ( radius + tube * Math::cos( v ) ) * Math::cos( u );
        vertex.y = ( radius + tube * Math::cos( v ) ) * Math::sin( u );
        vertex.z = tube * Math::sin( v );

        vertices.push_back( vertex );

        tmpUvs.push_back( UV( (float)i / segmentsT, (float)j / segmentsR ) );
        tmpNormals.push_back( vertex.clone().subSelf( center ).normalize() );

      }
    }


    for ( int j = 1; j <= segmentsR; j ++ ) {

      for ( int i = 1; i <= segmentsT; i ++ ) {

        const auto a = ( segmentsT + 1 ) * j + i - 1;
        const auto b = ( segmentsT + 1 ) * ( j - 1 ) + i - 1;
        const auto c = ( segmentsT + 1 ) * ( j - 1 ) + i;
        const auto d = ( segmentsT + 1 ) * j + i;

        Face4 face( a, b, c, d, tmpNormals[ a ], tmpNormals[ b ], tmpNormals[ c ], tmpNormals[ d ] );
        face.normal.addSelf( tmpNormals[ a ] );
        face.normal.addSelf( tmpNormals[ b ] );
        face.normal.addSelf( tmpNormals[ c ] );
        face.normal.addSelf( tmpNormals[ d ] );
        face.normal.normalize();

        faces.push_back( face );

        faceVertexUvs[ 0 ].push_back( toArray( tmpUvs[ a ], tmpUvs[ b ], tmpUvs[ c ], tmpUvs[ d ] ) );
      }

    }

    computeCentroids();

  }

};

} // namespace three

#endif // THREE_TORUS_GEOMETRY_HPP