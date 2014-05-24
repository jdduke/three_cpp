#ifndef THREE_GEOMETRY_CPP
#define THREE_GEOMETRY_CPP

#include <three/common.h>

#include <three/core/geometry.h>
#include <three/core/face.h>
#include <three/math/vector3.h>
#include <three/math/vector4.h>
#include <three/math/matrix3.h>

namespace three {

template <class T>
inline void hash_combine( std::size_t& seed, const T& v ) {
  std::hash<T> hasher;
  seed ^= hasher( v ) + 0x9e3779b9 + ( seed << 6 ) + ( seed >> 2 );
}

struct PointHash {
  std::size_t operator()( const std::tuple<int, int, int>& e ) const {
    std::size_t seed = 0;
    hash_combine( seed, std::get<0>( e ) );
    hash_combine( seed, std::get<1>( e ) );
    hash_combine( seed, std::get<2>( e ) );
    return seed;
  }
};

void Geometry::applyMatrix( Matrix4& matrix ) {

  auto normalMatrix = Matrix3().getNormalMatrix( matrix );

  for ( size_t i = 0, il = vertices.size(); i < il; i ++ ) {

    auto& vertex = vertices[ i ];
    vertex.applyMatrix4( matrix );

  }

  for ( size_t i = 0, il = faces.size(); i < il; i ++ ) {

    auto& face = faces[ i ];
    face.normal.applyMatrix3( normalMatrix ).normalize();

    for ( size_t j = 0, jl = face.vertexNormals.size(); j < jl; j ++ ) {

      face.vertexNormals[ j ].applyMatrix3( normalMatrix ).normalize();

    }

    face.centroid.applyMatrix4( matrix );

  }

  computeBoundingBox();

  computeBoundingSphere();

}

void Geometry::computeCentroids() {

    for ( size_t f = 0, fl = faces.size(); f < fl; f ++ ) {

      auto& face = faces[ f ];
      face.centroid.set( 0, 0, 0 );

      face.centroid.add( vertices[ face.a ] );
      face.centroid.add( vertices[ face.b ] );
      face.centroid.add( vertices[ face.c ] );
      face.centroid.divideScalar( 3 );

    }

}

void Geometry::computeFaceNormals() {

  Vector3 cb, ab;
  
  for ( auto& face : faces ) {

    auto& vA = vertices[ face.a ];
    auto& vB = vertices[ face.b ];
    auto& vC = vertices[ face.c ];

    cb.subVectors( vC, vB );
    ab.subVectors( vA, vB );
    cb.cross( ab );

    cb.normalize();

    face.normal.copy( cb );

  }

}

void Geometry::computeVertexNormals( bool areaWeighted ) {

  // create internal buffers for reuse when calling this method repeatedly
  // (otherwise memory allocation / deallocation every frame is big resource hog)

  THREE_REVIEW("EA: Efficiency/memory/performance")
  vertices.resize( vertices.size() );
  std::fill( vertices.begin(), vertices.end(), Vector3() );


  if( areaWeighted ) {

    // vertex normals weighted by triangle areas
    // http://www.iquilezles.org/www/articles/normals/normals.htm

      auto cb = Vector3(), ab = Vector3();
      // TODO: unused vars?
      //,db = Vector3(), dc = Vector3(), bc = Vector3();

    for ( const auto& face : faces ) {

      auto& vA = vertices[ face.a ];
      auto& vB = vertices[ face.b ];
      auto& vC = vertices[ face.c ];

      cb.subVectors( vC, vB );
      ab.subVectors( vA, vB );
      cb.cross( ab );

      vertices[ face.a ].add( cb );
      vertices[ face.b ].add( cb );
      vertices[ face.c ].add( cb );

    }

  } else {

    for ( const auto& face : faces ) {

      for ( auto i = 0; i < face.size(); ++i ) {

        vertices[ face.abc[ i ] ].add( face.normal );

      }

    }

  }


  for ( auto& vertice : vertices ) {

    vertice.normalize();

  }

  for ( auto& face : faces ) {

    face.vertexNormals[ 0 ].copy( vertices[ face.abc[ 0 ] ] );
    face.vertexNormals[ 1 ].copy( vertices[ face.abc[ 1 ] ] );
    face.vertexNormals[ 2 ].copy( vertices[ face.abc[ 2 ] ] );

  }

}

void Geometry::computeMorphNormals() {

  __originalFaceNormal.resize( faces.size() );
  __originalVertexNormals.resize( faces.size() );

  // save original normals
  // - create temp variables on first access
  //   otherwise just copy (for faster repeated calls)

  for ( size_t f = 0, fl = faces.size(); f < fl; f ++ ) {

    auto& face = faces[ f ];

    __originalFaceNormal[ f ].copy(face.normal);

    std::fill(__originalVertexNormals.begin(), __originalVertexNormals.end(), face.vertexNormals );

  }

  // use temp geometry to compute face and vertex normals for each morph

  auto tmpGeo = Geometry::create();
  tmpGeo->faces = faces;

  morphNormals.resize( morphTargets.size() );

  for ( size_t i = 0, il = morphTargets.size(); i < il; i ++ ) {

    auto& morphNormalsIndex = morphNormals[ i ];

    // set vertices to morph target

    tmpGeo->vertices = morphTargets[ i ].vertices;

    // compute morph normals

    tmpGeo->computeFaceNormals();
    tmpGeo->computeVertexNormals();

    // store morph normals

    for ( size_t f = 0, fl = faces.size(); f < fl; f ++ ) {

      auto& face = faces[ f ];

      auto& faceNormal = morphNormalsIndex.faceNormals[f];
      auto& vertexNormals = morphNormalsIndex.vertexNormals[f];

      faceNormal.copy( face.normal );

      vertexNormals.a.copy( face.vertexNormals[ 0 ] );
      vertexNormals.b.copy( face.vertexNormals[ 1 ] );
      vertexNormals.c.copy( face.vertexNormals[ 2 ] );

    }

  }

  // restore original normals

  for ( size_t f = 0, fl = faces.size(); f < fl; f ++ ) {

    auto& face = faces[ f ];

    face.normal = __originalFaceNormal[ f ];
    face.vertexNormals = __originalVertexNormals[ f ];

  }
}

void Geometry::computeTangents() {

  // based on http://www.terathon.com/code/tangent.html
  // tangents go to vertices

  std::vector<Vector3> tan1( vertices.size() );
  std::vector<Vector3> tan2( vertices.size() );

  static auto handleTriangle = [&, this]( const std::array<Vector2, 3>& uv, int a, int b, int c, int ua, int ub, int uc ) {

    const auto& vA = vertices[ a ];
    const auto& vB = vertices[ b ];
    const auto& vC = vertices[ c ];

    const auto uvA = uv[ ua ];
    const auto uvB = uv[ ub ];
    const auto uvC = uv[ uc ];

    const auto x1 = vB.x - vA.x;
    const auto x2 = vC.x - vA.x;
    const auto y1 = vB.y - vA.y;
    const auto y2 = vC.y - vA.y;
    const auto z1 = vB.z - vA.z;
    const auto z2 = vC.z - vA.z;

    const auto s1 = uvB.x - uvA.x;
    const auto s2 = uvC.x - uvA.x;
    const auto t1 = uvB.y - uvA.y;
    const auto t2 = uvC.y - uvA.y;

    const auto r = 1.0f / ( s1 * t2 - s2 * t1 );
    Vector3 sdir( ( t2 * x1 - t1 * x2 ) * r,
                  ( t2 * y1 - t1 * y2 ) * r,
                  ( t2 * z1 - t1 * z2 ) * r );
    Vector3 tdir( ( s1 * x2 - s2 * x1 ) * r,
                  ( s1 * y2 - s2 * y1 ) * r,
                  ( s1 * z2 - s2 * z1 ) * r );

    tan1[ a ].add( sdir );
    tan1[ b ].add( sdir );
    tan1[ c ].add( sdir );

    tan2[ a ].add( tdir );
    tan2[ b ].add( tdir );
    tan2[ c ].add( tdir );

  };

  for ( size_t f = 0, fl = faces.size(); f < fl; f++ ) {

    auto& face = faces[ f ];
    auto& uv = faceVertexUvs[ 0 ][ f ]; // use UV layer 0 for tangents

    THREE_ASSERT( face.type() == enums::Face3 );

    handleTriangle( uv, face.a, face.b, face.c, 0, 1, 2 );
  }

  Vector3 tmp, tmp2, n;

  for ( auto& face : faces ) {

    for ( auto i = 0; i < face.size(); i++ ) {

      n.copy( face.vertexNormals[ i ] );

      auto vertexIndex = face.abc[ i ];

      const auto& t = tan1[ vertexIndex ];

      // Gram-Schmidt orthogonalize

      tmp.copy( t );
      tmp.sub( n.multiplyScalar( n.dot( t ) ) ).normalize();

      // Calculate handedness

      tmp2.crossVectors( face.vertexNormals[ i ], t );
      const auto test = tmp2.dot( tan2[ vertexIndex ] );
      const auto w = ( test < 0.0f ) ? -1.0f : 1.0f;

      face.vertexTangents[ i ] = Vector4( tmp.x, tmp.y, tmp.z, w );

    }

  }

  hasTangents = true;

}

void Geometry::computeBoundingBox() {

  if ( !boundingBox ) {

    boundingBox = Box3();

  }

  boundingBox->setFromPoints( vertices );

}

void Geometry::computeBoundingSphere() {

  if ( !boundingSphere ) {

    boundingSphere = Sphere();

  }

  boundingSphere->setFromPoints( vertices );

}

/*
 * Checks for duplicate vertices with hashmap.
 * Duplicated vertices are removed
 * and faces' vertices are updated.
 */

void Geometry::mergeVertices() {

  typedef std::tuple<int, int, int> Key;
  std::unordered_map<Key, int, PointHash> verticesMap; // Hashmap for looking up vertice by position coordinates (and making sure they are unique)

  std::vector<Vertex> unique;
  std::vector<int> changes( vertices.size() );

  auto precisionPoints = 4; // number of decimal points, eg. 4 for epsilon of 0.0001
  auto precision = Math::pow( 10.f, precisionPoints );

  for ( size_t i = 0, il = vertices.size(); i < il; i ++ ) {

    const auto& v = vertices[ i ];
    auto key = std::make_tuple( ( int )Math::round( v.x * precision ), ( int )Math::round( v.y * precision ), ( int )Math::round( v.z * precision ) );

    auto vertexIter = verticesMap.find( key );
    if ( vertexIter == verticesMap.end() ) {

      verticesMap[ key ] = ( int )i;
      unique.push_back( vertices[ i ] );
      changes[ i ] = ( int )unique.size() - 1;

    } else {

      //console.log('Duplicate vertex found. ', i, ' could be using ', verticesMap[key]);
      changes[ i ] = changes[ vertexIter->second ];

    }

  }


  // Start to patch face indices

  for ( auto& face : faces ) {
    for ( int i = 0; i < face.size(); ++i ) {
      face.abc[ i ] = changes[ face.abc[ i ] ];
    }
  }

  // Use unique set of vertices

  vertices = std::move( unique );

}

Geometry::Geometry()
  : id( GeometryCount()++ ),
    uuid( Math::generateUUID() ),
    faceVertexUvs( 2 ),
    hasTangents( false ),
    dynamic( true ),
    verticesNeedUpdate( false ),
    morphTargetsNeedUpdate( false ),
    elementsNeedUpdate( false ),
    uvsNeedUpdate( false ),
    normalsNeedUpdate( false ),
    tangentsNeedUpdate( false ),
    colorsNeedUpdate( false ) { }

Geometry::~Geometry() { }

} // namespace three

#endif // THREE_GEOMETRY_CPP