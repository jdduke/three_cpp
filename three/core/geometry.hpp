#ifndef THREE_GEOMETRY_HPP
#define THREE_GEOMETRY_HPP

#include <three/common.hpp>

#include <three/core/math.hpp>
#include <three/core/color.hpp>
#include <three/core/vector3.hpp>
#include <three/core/vertex.hpp>
#include <three/core/uv.hpp>

#include <three/materials/material.hpp>

#include <array>
#include <unordered_map>
#include <tuple>

namespace three {

template <class T>
inline void hash_combine(std::size_t& seed, const T& v)
{
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
}

struct PointHash {
    std::size_t operator() ( const std::tuple<int,int,int>& e ) {
        std::size_t seed = 0;
        hash_combine( seed, std::get<0>(e) );
        hash_combine( seed, std::get<1>(e) );
        hash_combine( seed, std::get<2>(e) );
        return seed;
    }
};

struct MorphTarget {
    std::string name;
    std::vector<Vertex> vertices;
};

struct Box {
    float x[2];
    float y[2];
    float z[2];

    void bound( const Vertex& vertex ) {
       if ( vertex.position.x < x[ 0 ] ) {
            x[ 0 ] = vertex.position.x;
        } else if ( vertex.position.x > x[ 1 ] ) {
            x[ 1 ] = vertex.position.x;
        }

        if ( vertex.position.y < y[ 0 ] ) {
            y[ 0 ] = vertex.position.y;
        } else if ( vertex.position.y > y[ 1 ] ) {
            y[ 1 ] = vertex.position.y;
        }

        if ( vertex.position.z < z[ 0 ] ) {
            z[ 0 ] = vertex.position.z;
        } else if ( vertex.position.z > z[ 1 ] ) {
            z[ 1 ] = vertex.position.z;
        }
    }
};

struct Sphere {
    float radius;
};

class Geometry {
public:

    typedef std::shared_ptr<Geometry> Ptr;

    Ptr create() { return std::make_shared<Geometry>(); }

    /////////////////////////////////////////////////////////////////////////

    int id;

    std::string name;

    std::vector<Vertex> vertices;

    std::vector<Color> colors; // one-to-one vertex colors, used in ParticleSystem, Line and Ribbon

    std::vector<Material::Ptr> materials;

    std::vector<Face3> faces;

    std::vector<std::vector<UV>> faceUvs;
    std::vector<std::vector<std::array<UV,4>>> faceVertexUvs;

    std::vector<MorphTarget> morphTargets;
    std::vector<Color> morphColors;
    std::vector<Vector3> morphNormals;

    std::vector<float> skinWeights;
    std::vector<int> skinIndices;

    Box    boundingBox;
    Sphere boundingSphere;

    bool hasTangents;

    bool dynamic;

    /////////////////////////////////////////////////////////////////////////

    void applyMatrix ( const Matrix4& matrix ) {

        Matrix4 matrixRotation;
        // BUGBUG
        //matrixRotation.extractRotation( matrix, Vector3( 1, 1, 1 ) );
        matrixRotation.extractRotation( matrix );

        for ( auto& vertex : vertices ) {

            matrix.multiplyVector3( vertex.position );

        }

        for ( auto& face : faces ) {

            matrixRotation.multiplyVector3( face.normal );

            for ( auto i = 0; i < face.size(); ++i ) {

                matrixRotation.multiplyVector3( face.vertexNormals[i] );

            }

            matrix.multiplyVector3( face.centroid );

        }

    }

    void computeCentroids () {

        for ( auto& face : faces ) {

            face.centroid.set( 0, 0, 0 );

            for ( auto i = 0; i < face.size(); ++i ) {

                face.centroid.addSelf ( vertices[ face.abcd[ i ] ].position );

            }

            face.centroid.divideScalar( face.size() );

        }

    }

    void computeFaceNormals () {

        for ( auto& face : faces ) {

            auto& vA = vertices[ face.a ];
            auto& vB = vertices[ face.b ];
            auto& vC = vertices[ face.c ];

            auto cb = sub( vC.position, vB.position );
            auto ab = sub( vA.position, vB.position );
            cb.crossSelf( ab );

            if ( !cb.isZero() ) {

                cb.normalize();

            }

            face.normal.copy( cb );

        }

    }

    void computeVertexNormals () {

        // create internal buffers for reuse when calling this method repeatedly
        // (otherwise memory allocation / deallocation every frame is big resource hog)

        if ( normals.size() == 0 ) {

            normals.resize ( vertices.size() );

        } else {

            std::fill ( normals.begin(), normals.end(), Vector3() );

        }

        for ( const auto& face : faces ) {

            for ( auto i = 0; i < face.size(); ++i ) {

                normals[ face.abcd[ i ] ].addSelf ( face.normal );

            }

        }

        for ( auto& normal : normals ) {

            normal.normalize();

        }

        for ( auto& face : faces ) {

            for ( auto i = 0; i < face.size(); ++i ) {
                face.vertexNormals[ i ].copy( normals[ face.abcd[ i ] ] );
            }

        }

    }

    void computeTangents () {

        // based on http://www.terathon.com/code/tangent.html
        // tangents go to vertices

        std::vector<Vector3> tan1 ( vertices.size() );
        std::vector<Vector3> tan2 ( vertices.size() );

        auto handleTriangle = [&,this] (const std::array<UV,4>& uv, int a, int b, int c, int ua, int ub, int uc ) {

            const auto& vA = vertices[ a ].position;
            const auto& vB = vertices[ b ].position;
            const auto& vC = vertices[ c ].position;

            const auto uvA = uv[ ua ];
            const auto uvB = uv[ ub ];
            const auto uvC = uv[ uc ];

            const auto x1 = vB.x - vA.x;
            const auto x2 = vC.x - vA.x;
            const auto y1 = vB.y - vA.y;
            const auto y2 = vC.y - vA.y;
            const auto z1 = vB.z - vA.z;
            const auto z2 = vC.z - vA.z;

            const auto s1 = uvB.u - uvA.u;
            const auto s2 = uvC.u - uvA.u;
            const auto t1 = uvB.v - uvA.v;
            const auto t2 = uvC.v - uvA.v;

            const auto r = 1.0 / ( s1 * t2 - s2 * t1 );
            Vector3 sdir( ( t2 * x1 - t1 * x2 ) * r,
                      ( t2 * y1 - t1 * y2 ) * r,
                      ( t2 * z1 - t1 * z2 ) * r );
            Vector3 tdir( ( s1 * x2 - s2 * x1 ) * r,
                      ( s1 * y2 - s2 * y1 ) * r,
                      ( s1 * z2 - s2 * z1 ) * r );

            tan1[ a ].addSelf( sdir );
            tan1[ b ].addSelf( sdir );
            tan1[ c ].addSelf( sdir );

            tan2[ a ].addSelf( tdir );
            tan2[ b ].addSelf( tdir );
            tan2[ c ].addSelf( tdir );

        };

        for ( int f = 0, fl = faces.size(); f < fl; f++ ) {

            auto& face = faces[ f ];
            auto& uv = faceVertexUvs[ 0 ][ f ]; // use UV layer 0 for tangents

            if ( face.type() == THREE::Face3 ) {

                handleTriangle( uv, face.a, face.b, face.c, 0, 1, 2 );

            } else if ( face.type() == THREE::Face4 ) {

                handleTriangle( uv, face.a, face.b, face.c, 0, 1, 2 );
                handleTriangle( uv, face.a, face.b, face.d, 0, 1, 3 );

            }

        }

        Vector3 tmp, tmp2, n;

        for ( auto& face : faces ) {

            for ( auto i = 0; i < face.size(); i++ ) {

                n.copy( face.vertexNormals[ i ] );

                auto vertexIndex = face.abcd[ i ];

                const auto& t = tan1[ vertexIndex ];

                // Gram-Schmidt orthogonalize

                tmp.copy( t );
                tmp.subSelf( n.multiplyScalar( n.dot( t ) ) ).normalize();

                // Calculate handedness

                tmp2.cross( face.vertexNormals[ i ], t );
                const auto test = tmp2.dot( tan2[ vertexIndex ] );
                const auto w = (test < 0.0) ? -1.0 : 1.0;

                face.vertexTangents[ i ] = Vector4( tmp.x, tmp.y, tmp.z, w );

            }

        }

        hasTangents = true;

    }

    void computeBoundingBox () {

        if ( vertices.size() > 0 ) {

            boundingBox = { { vertices[ 0 ].position.x, vertices[ 0 ].position.x },
                            { vertices[ 0 ].position.y, vertices[ 0 ].position.y },
                            { vertices[ 0 ].position.z, vertices[ 0 ].position.z } };

            for ( int v = 1, vl = vertices.size(); v < vl; v ++ ) {

                boundingBox.bound ( vertices [ v ] );

            }

        }

    }

    void computeBoundingSphere () {

        // var radius = this.boundingSphere === null ? 0 : this.boundingSphere.radius;

        auto radius = 0.f;

        for ( const auto& vertex : vertices ) {

            radius = Math::max( radius, vertex.position.length() );

        }

        boundingSphere.radius = radius;

    }

    /*
     * Checks for duplicate vertices with hashmap.
     * Duplicated vertices are removed
     * and faces' vertices are updated.
     */

    void mergeVertices () {

        typedef std::tuple<int,int,int> Key;
        std::unordered_map<Key,int,PointHash> verticesMap; // Hashmap for looking up vertice by position coordinates (and making sure they are unique)

        std::vector<Vertex> unique;
        std::vector<int> changes( vertices.size() );

        auto precisionPoints = 4; // number of decimal points, eg. 4 for epsilon of 0.0001
        auto precision = Math::pow( 10.f, precisionPoints );

        for ( int i = 0, il = vertices.size(); i < il; i ++ ) {

            const auto& v = vertices[ i ].position;
            auto key = std::make_tuple( (int)Math::round( v.x * precision ), (int)Math::round( v.y * precision ), (int)Math::round( v.z * precision ) );

            auto vertexIter = verticesMap.find( key );
            if ( vertexIter == verticesMap.end() ) {

                verticesMap[ key ] = i;
                unique.push_back( vertices[ i ] );
                changes[ i ] = unique.size() - 1;

            } else {

                //console.log('Duplicate vertex found. ', i, ' could be using ', verticesMap[key]);
                changes[ i ] = changes[ vertexIter->second ];

            }

        }


        // Start to patch face indices

        for ( auto& face : faces ) {

            for ( int i = 0; i < face.size(); ++i ) {

                face.abcd[ i ] = changes[ face.abcd[ i ] ];

            }

        }

        // Use unique set of vertices

        vertices = std::move( unique );

    }

    /////////////////////////////////////////////////////////////////////////

protected:

    Geometry()
    : id ( GeometryCount()++ ),
    boundingBox ( {{0,0}, {0,0}, {0,0}} ),
    boundingSphere ( { 0 } ),
    hasTangents ( false ),
    dynamic ( true ) { }

private:

    std::vector<Vector3> normals;

    static int& GeometryCount() {
        static int sGeometryCount = 0;
        return sGeometryCount;
    }

};

} // namespace three

#endif // THREE_GEOMETRY_HPP