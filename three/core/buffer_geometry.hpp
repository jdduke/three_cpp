#ifndef THREE_BUFFER_GEOMETRY_HPP
#define THREE_BUFFER_GEOMETRY_HPP

#include <three/common.hpp>

#include <three/core/geometry.hpp>

namespace three {

template < typename T >
inline bool contains( const std::map<std::string, T>& c, const char* elem ) {
    return c.find( std::string(elem) ) != c.end();
}

struct Offset {
    Offset() : start ( 0 ), count ( 0 ), index ( 0 ) { }
    int start, count, index;
};

class BufferGeometry : public Geometry {
public:

    typedef std::shared_ptr<BufferGeometry> Ptr;

    Ptr create() { return make_shared<BufferGeometry>(); }

    /////////////////////////////////////////////////////////////////////////

    std::vector<Offset> offsets;

    /////////////////////////////////////////////////////////////////////////

    void applyMatrix ( const Matrix4& matrix ) {

        if ( contains( attributes, "position" ) ) {

            matrix.multiplyVector3Array( attributes[ "position" ].array );
            verticesNeedUpdate = true;

        }

        if ( contains( attributes, "normal" ) ) {

            Matrix4 matrixRotation;
            matrixRotation.extractRotation( matrix );

            matrixRotation.multiplyVector3Array( attributes[ "normal" ].array );
            normalsNeedUpdate = true;

        }

    }

    void computeBoundingBox () {

        Box bb;

        if ( contains( attributes, "positions" ) ) {

            const auto& positions = attributes[ "position" ].array;

            if ( positions.size() > 2 ) {

                bb.min = bb.max = Vector3 ( positions[ 0 ], positions[ 1 ], positions[ 2 ] );

                for ( size_t i = 3, il = positions.size(); i < il; i += 3 ) {

                    boundingBox.bound( Vector3( positions[ i ], positions[ i + 1 ], positions[ i + 2 ] ) );

                }

            }

        }

        boundingBox = bb;

    }

    void computeBoundingSphere () {

        if ( contains( attributes, "positions" ) ) {

            float maxRadiusSq = 0;

            const auto& positions = attributes[ "position" ].array;

            for ( size_t i = 0, il = positions.size(); i < il; i += 3 ) {

                Vector3 pos ( positions[ i ], positions[ i + 1 ], positions[ i + 2 ] );
                auto radiusSq =  pos.lengthSq();
                if ( radiusSq > maxRadiusSq ) maxRadiusSq = radiusSq;

            }

            boundingSphere.radius = Math::sqrt( maxRadiusSq );

        }

    }

    void computeVertexNormals () {

        if ( contains( attributes, "positions" ) && contains( attributes, "index" ) ) {

            const auto& indices = attributes[ "index" ].array;
            const auto& positions = attributes[ "position" ].array;

            const auto nVertexElements = positions.size();

            if ( !contains( attributes, "normal" ) ) {

                attributes[ "normal" ] = GLCustomAttribute( THREE::V3, nVertexElements );

            } else {

                // reset existing normals to zero

                auto& normals = attributes[ "normal" ].array;
                for ( size_t i = 0, il = normals.size(); i < il; i ++ ) {

                    normals[ i ] = 0;

                }

            }

            auto& normals = attributes[ "normal" ].array;

            Vector3 pA, pB, pC, cb, ab;

            for ( size_t j = 0, jl = offsets.size(); j < jl; ++ j ) {

                const auto start = offsets[ j ].start;
                const auto count = offsets[ j ].count;
                const auto index = offsets[ j ].index;

                for ( auto i = start, il = start + count; i < il; i += 3 ) {

                    const auto vA = index + indices[ i ];
                    const auto vB = index + indices[ i + 1 ];
                    const auto vC = index + indices[ i + 2 ];

                    auto x = positions[ vA * 3 ];
                    auto y = positions[ vA * 3 + 1 ];
                    auto z = positions[ vA * 3 + 2 ];
                    pA.set( x, y, z );

                    x = positions[ vB * 3 ];
                    y = positions[ vB * 3 + 1 ];
                    z = positions[ vB * 3 + 2 ];
                    pB.set( x, y, z );

                    x = positions[ vC * 3 ];
                    y = positions[ vC * 3 + 1 ];
                    z = positions[ vC * 3 + 2 ];
                    pC.set( x, y, z );

                    cb.sub( pC, pB );
                    ab.sub( pA, pB );
                    cb.crossSelf( ab );

                    normals[ vA * 3 ] += cb.x;
                    normals[ vA * 3 + 1 ] += cb.y;
                    normals[ vA * 3 + 2 ] += cb.z;

                    normals[ vB * 3 ] += cb.x;
                    normals[ vB * 3 + 1 ] += cb.y;
                    normals[ vB * 3 + 2 ] += cb.z;

                    normals[ vC * 3 ] += cb.x;
                    normals[ vC * 3 + 1 ] += cb.y;
                    normals[ vC * 3 + 2 ] += cb.z;

                }

            }

            // normalize normals

            for ( size_t i = 0, il = normals.size(); i < il; i += 3 ) {

                Vector3 xyz ( normals[ i ], normals[ i + 1 ], normals[ i + 2 ] );

                auto n = 1.0f / xyz.length();

                normals[ i ] *= n;
                normals[ i + 1 ] *= n;
                normals[ i + 2 ] *= n;

            }

            normalsNeedUpdate = true;

        }

    }

    void computeTangents () {

        // based on http://www.terathon.com/code/tangent.html
        // (per vertex tangents)

        if ( !contains( attributes, "index" ) ||
             !contains( attributes, "position" ) ||
             !contains( attributes, "normal" ) ||
             !contains( attributes, "uv" ) ) {

            console().warn( "Missing required attributes (index, position, normal or uv) in BufferGeometry.computeTangents()" );
            return;

        }

        const auto& indices   = attributes[ "index" ].array;
        const auto& positions = attributes[ "position" ].array;
        const auto& normals   = attributes[ "normal" ].array;
        const auto& uvs       = attributes[ "uv" ].array;

        const auto nVertices = positions.size() / 3;

        if ( !contains( attributes, "tangent" ) ) {

             const auto nTangentElements = 4 * nVertices;

             attributes[ "tangent" ] = GLCustomAttribute( THREE::V4, nTangentElements );

        }

        auto& tangents = attributes[ "tangent" ].array;

        std::vector<Vector3> tan1( nVertices ), tan2( nVertices );

        float xA, yA, zA,
            xB, yB, zB,
            xC, yC, zC,

            uA, vA,
            uB, vB,
            uC, vC,

            x1, x2, y1, y2, z1, z2,
            s1, s2, t1, t2, r;

        Vector3 sdir, tdir;

        auto handleTriangle = [&]( size_t a, size_t b, size_t c ) {

            xA = positions[ a * 3 ];
            yA = positions[ a * 3 + 1 ];
            zA = positions[ a * 3 + 2 ];

            xB = positions[ b * 3 ];
            yB = positions[ b * 3 + 1 ];
            zB = positions[ b * 3 + 2 ];

            xC = positions[ c * 3 ];
            yC = positions[ c * 3 + 1 ];
            zC = positions[ c * 3 + 2 ];

            uA = uvs[ a * 2 ];
            vA = uvs[ a * 2 + 1 ];

            uB = uvs[ b * 2 ];
            vB = uvs[ b * 2 + 1 ];

            uC = uvs[ c * 2 ];
            vC = uvs[ c * 2 + 1 ];

            x1 = xB - xA;
            x2 = xC - xA;

            y1 = yB - yA;
            y2 = yC - yA;

            z1 = zB - zA;
            z2 = zC - zA;

            s1 = uB - uA;
            s2 = uC - uA;

            t1 = vB - vA;
            t2 = vC - vA;

            r = 1.0f / ( s1 * t2 - s2 * t1 );

            sdir.set(
                ( t2 * x1 - t1 * x2 ) * r,
                ( t2 * y1 - t1 * y2 ) * r,
                ( t2 * z1 - t1 * z2 ) * r
            );

            tdir.set(
                ( s1 * x2 - s2 * x1 ) * r,
                ( s1 * y2 - s2 * y1 ) * r,
                ( s1 * z2 - s2 * z1 ) * r
            );

            tan1[ a ].addSelf( sdir );
            tan1[ b ].addSelf( sdir );
            tan1[ c ].addSelf( sdir );

            tan2[ a ].addSelf( tdir );
            tan2[ b ].addSelf( tdir );
            tan2[ c ].addSelf( tdir );

        };

        for ( size_t j = 0, jl = offsets.size(); j < jl; ++ j ) {

            const auto start = offsets[ j ].start;
            const auto count = offsets[ j ].count;
            const auto index = offsets[ j ].index;

            for ( auto i = start, il = start + count; i < il; i += 3 ) {

                const auto iA = index + indices[ i ];
                const auto iB = index + indices[ i + 1 ];
                const auto iC = index + indices[ i + 2 ];

                handleTriangle( iA, iB, iC );

            }

        }

        Vector3 tmp, tmp2;
        Vector3 n, n2;

        auto handleVertex = [&]( size_t v ) {

            n.x = normals[ v * 3 ];
            n.y = normals[ v * 3 + 1 ];
            n.z = normals[ v * 3 + 2 ];

            n2.copy( n );

            const auto& t = tan1[ v ];

            // Gram-Schmidt orthogonalize

            tmp.copy( t );
            tmp.subSelf( n.multiplyScalar( n.dot( t ) ) ).normalize();

            // Calculate handedness

            tmp2.cross( n2, t );
            const auto test = tmp2.dot( tan2[ v ] );
            const auto w = ( test < 0.0f ) ? -1.0f : 1.0f;

            tangents[ v * 4 ]     = tmp.x;
            tangents[ v * 4 + 1 ] = tmp.y;
            tangents[ v * 4 + 2 ] = tmp.z;
            tangents[ v * 4 + 3 ] = w;

        };

        for ( size_t j = 0, jl = offsets.size(); j < jl; ++ j ) {

            const auto start = offsets[ j ].start;
            const auto count = offsets[ j ].count;
            const auto index = offsets[ j ].index;

            for ( auto i = start, il = start + count; i < il; i += 3 ) {

                const auto iA = index + indices[ i ];
                const auto iB = index + indices[ i + 1 ];
                const auto iC = index + indices[ i + 2 ];

                handleVertex( iA );
                handleVertex( iB );
                handleVertex( iC );

            }

        }

        hasTangents = true;
        tangentsNeedUpdate = true;

    }

protected:

    BufferGeometry()
    : Geometry() {
        dynamic = false;
        hasTangents = false;
        boundingBox = Box( Vector3(Math::INF), Vector3(-Math::INF) );
    }

};

} // namespace three

#endif // THREE_BUFFER_GEOMETRY_HPP