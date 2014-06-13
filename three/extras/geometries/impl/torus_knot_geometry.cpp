#ifndef THREE_TORUS_KNOT_GEOMETRY_CPP
#define THREE_TORUS_KNOT_GEOMETRY_CPP

#include <three/core/face3.h>

#include <three/extras/geometries/torus_knot_geometry.h>

#include <three/utils/conversion.h>

namespace three {

TorusKnotGeometry::Ptr TorusKnotGeometry::create( float radius,
                   float tube,
                   size_t radialSegments,
                   size_t tubularSegments,
                   float p,
                   float q,
                   float heightScale ) {

  auto torusKnotGeometry = make_shared<TorusKnotGeometry>();

  torusKnotGeometry->initialize( radius, tube, radialSegments, tubularSegments, p, q, heightScale );

  return torusKnotGeometry;
}

TorusKnotGeometry::TorusKnotGeometry() {}

void TorusKnotGeometry::initialize( float radius,
                 float tube,
                 size_t radialSegments,
                 size_t tubularSegments,
                 float p,
                 float q,
                 float heightScale ) {

	std::vector<std::vector<int>> grid;
  grid.resize( radialSegments );

	auto tang = Vector3();
	auto n = Vector3();
	auto bitan = Vector3();

	for ( size_t i = 0; i < radialSegments; ++ i ) {


		auto u = (float)i / (float)radialSegments * 2.f * p * Math::PI();
		Vector3 p1 = getPos( u, q, p, radius, heightScale );
		Vector3 p2 = getPos( u + 0.01f, q, p, radius, heightScale );

		tang.subVectors( p2, p1 );
		n.addVectors( p2, p1 );

		bitan.crossVectors( tang, n );
		n.crossVectors( bitan, tang );
		bitan.normalize();
		n.normalize();

		for ( size_t j = 0; j < tubularSegments; ++ j ) {

			auto v = (float)j / (float)tubularSegments * 2.f * Math::PI();
			auto cx = - tube * Math::cos( v ); // TODO: Hack: Negating it so it faces outside.
			auto cy = tube * Math::sin( v );

			auto pos = Vector3();
			pos.x = p1.x + cx * n.x + cy * bitan.x;
			pos.y = p1.y + cx * n.y + cy * bitan.y;
			pos.z = p1.z + cx * n.z + cy * bitan.z;

      this->vertices.push_back( pos );
			grid[ i ].push_back( this->vertices.size() - 1 );

		}

	}

	for ( size_t i = 0; i < radialSegments; ++ i ) {

		for ( size_t j = 0; j < tubularSegments; ++ j ) {

			auto ip = ( i + 1 ) % radialSegments;
			auto jp = ( j + 1 ) % tubularSegments;

			auto a = grid[ i ][ j ];
			auto b = grid[ ip ][ j ];
			auto c = grid[ ip ][ jp ];
			auto d = grid[ i ][ jp ];

			auto uva = Vector2( (float)i / (float)radialSegments, (float)j / (float)tubularSegments );
			auto uvb = Vector2( (float)( i + 1 ) / (float)radialSegments, (float)j / (float)tubularSegments );
			auto uvc = Vector2( (float)( i + 1 ) / (float)radialSegments, (float)( j + 1 ) / (float)tubularSegments );
			auto uvd = Vector2( (float)i / (float)radialSegments, (float)( j + 1 ) / (float)tubularSegments );

			this->faces.push_back( Face3( a, b, d ) );
			this->faceVertexUvs[ 0 ].push_back( toArray( uva, uvb, uvd ) );

			this->faces.push_back( Face3( b, c, d ) );
			this->faceVertexUvs[ 0 ].push_back( toArray( uvb.clone(), uvc, uvd.clone() ) );

		}
	}

	this->computeCentroids();
	this->computeFaceNormals();
	this->computeVertexNormals();

}

Vector3 TorusKnotGeometry::getPos( float u, float in_q, float in_p, float radius, float heightScale ) const {

  auto cu = Math::cos( u );
  auto su = Math::sin( u );
  auto quOverP = in_q / in_p * u;
  auto cs = Math::cos( quOverP );

  auto tx = radius * ( 2 + cs ) * 0.5 * cu;
  auto ty = radius * ( 2 + cs ) * su * 0.5;
  auto tz = heightScale * radius * Math::sin( quOverP ) * 0.5;

  return Vector3( tx, ty, tz );

}

} // namespace three

#endif // THREE_TORUS_KNOT_GEOMETRY_CPP