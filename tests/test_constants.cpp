#include <three/math/math.h>
#include <three/math/vector2.h>
#include <three/math/vector3.h>
#include <three/math/matrix4.h>

using namespace three;

auto x = 2.f;
auto y = 3.f;
auto z = 4.f;
auto w = 5.f;

auto negInf2 = Vector2( -Math::INF(), -Math::INF() );
auto posInf2 = Vector2( Math::INF(), Math::INF() );

auto zero2 = Vector2();
auto one2 = Vector2( 1, 1 );
auto two2 = Vector2( 2, 2 );

auto negInf3 = Vector3( -Math::INF(), -Math::INF(), -Math::INF() );
auto posInf3 = Vector3( Math::INF(), Math::INF(), Math::INF() );

auto zero3 = Vector3( 0, 0, 0 );
auto one3 = Vector3( 1, 1, 1 );
auto two3 = Vector3( 2, 2, 2 );

auto unit3 = Vector3( 2, 3, 4 );

bool matrixEquals4 ( const Matrix4& a, const Matrix4& b, float tolerance = 0.0001 ) {
	for( auto i = 0, il = 16; i < il; i ++ ) {
		auto delta = a.elements[i] - b.elements[i];
		if( delta > tolerance ) {
			return false;
		}
	}
	return true;
};
