#ifndef THREE_GEOMETRY_UTILS_HPP
#define THREE_GEOMETRY_UTILS_HPP

#include <three/math/face3.hpp>
#include <three/math/face4.hpp>

namespace three {

class GeometryUtils {
public:

  static void merge( Geometry& geometry1, Object3D& object2 ) {

    if ( object2.type() == THREE::Mesh ) {

      if ( object2.matrixAutoUpdate )
        object2.updateMatrix();

      Matrix4 matrix, matrixRotation;

      matrix = object2.matrix;
      matrixRotation.extractRotation( matrix );//, object2.scale );

      merge( geometry1, *object2.geometry, &matrix, &matrixRotation );

    } else {

      merge( geometry1, *object2.geometry );

    }

  }

  static void merge( Geometry& geometry1,
                     Geometry& geometry2,
                     const Matrix4* matrix = nullptr,
                     const Matrix4* matrixRotation = nullptr ) {

    auto vertexOffset = (int)geometry1.vertices.size();

    auto& vertices1 = geometry1.vertices;
    auto& vertices2 = geometry2.vertices;
    auto& faces1 = geometry1.faces;
    auto& faces2 = geometry2.faces;
    auto& uvs1 = geometry1.faceVertexUvs[ 0 ];
    auto& uvs2 = geometry2.faceVertexUvs[ 0 ];

    std::unordered_map<int, int> geo1MaterialsMap;

    for ( int i = 0; i < (int)geometry1.materials.size(); i ++ ) {
      const auto& id = geometry1.materials[ i ]->id;
      geo1MaterialsMap[ id ] = i;
    }

    // vertices
    concat( vertices1, vertices2 );

    if ( matrix ) {
      std::for_each( vertices1.begin() + vertexOffset, vertices1.end(),
        [&matrix] ( Vertex& v ) { matrix->multiplyVector3( v ); }
      );
    }

    // faces

    for ( size_t i = 0, il = faces2.size(); i < il; i ++ ) {

      const auto& face = faces2[ i ];

      Face faceCopy( 0, 0, 0 );//, normal, color,
      const auto& faceVertexNormals = face.vertexNormals;
      const auto& faceVertexColors = face.vertexColors;

      if ( face.type() == THREE::Face3 ) {
        faceCopy = Face3( face.a + vertexOffset, face.b + vertexOffset, face.c + vertexOffset );
      } else if ( face.type() == THREE::Face4 ) {
        faceCopy = Face4( face.a + vertexOffset, face.b + vertexOffset, face.c + vertexOffset, face.d + vertexOffset );
      }

      faceCopy.normal.copy( face.normal );

      if ( matrixRotation ) matrixRotation->multiplyVector3( faceCopy.normal );

      for ( size_t j = 0, jl = faceVertexNormals.size(); j < jl; j ++ ) {

        auto normal = faceVertexNormals[ j ].clone();

        if ( matrixRotation ) matrixRotation->multiplyVector3( normal );

        faceCopy.vertexNormals[ j ] = normal;

      }

      faceCopy.color.copy( face.color );

      faceCopy.vertexColors = faceVertexColors;

      if ( face.materialIndex != -1 ) {

        auto material2 = geometry2.materials[ face.materialIndex ];
        auto materialId2 = material2->id;
        int materialIndex = 0;

        auto materialIndexIt = geo1MaterialsMap.find( materialId2 );
        if ( materialIndexIt == geo1MaterialsMap.end() ) {

          materialIndex = (int)geometry1.materials.size();
          geo1MaterialsMap[ materialId2 ] = materialIndex;

          geometry1.materials.push_back( material2 );

        } else {
          materialIndex = materialIndexIt->second;
        }

        faceCopy.materialIndex = materialIndex;

      }

      faceCopy.centroid.copy( face.centroid );
      if ( matrix ) matrix->multiplyVector3( faceCopy.centroid );

      faces1.push_back( faceCopy );

    }

    // uvs

    concat( uvs1, uvs2 );

  }

}; // GeometryUtils

} // namespace three

#endif // THREE_GEOMETRY_UTILS_HPP



