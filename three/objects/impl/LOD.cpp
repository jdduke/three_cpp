#ifndef THREE_LOOD_CPP
#define THREE_LOOD_CPP

#include <three/objects/LOD.h>

namespace three {

LOD& LOD::addLevel( const Object3D::Ptr& object, float distance ) {

	distance = Math::abs(distance);

	for(auto it = objects.begin(); it != objects.end(); it++) {

		if( distance < (*it).distance ) {

			objects.insert(it, DistanceObject( distance, object ) );

			break;
		}

	}
	
	add( object );

	return *this;
}

Object3D::Ptr LOD::getObjectForDistance( float distance ) const {

	for ( size_t i = 1, l = objects.size(); i < l; i ++ ) {

		if ( distance < objects[ i ].distance ) {

			return objects[ i - 1 ].object;

		}

	}
    
    return nullptr;
    
}

void LOD::update( const Camera::Ptr& camera ) const {

	auto v1 = Vector3();
	auto v2 = Vector3();

	if ( objects.size() > 1 ) {

		v1.setFromMatrixPosition( camera->matrixWorld );
		v2.setFromMatrixPosition( matrixWorld );

		float distance = v1.distanceTo( v2 );

		objects[ 0 ].object->visible = true;

		size_t i, l;

		for ( i = 1, l = objects.size(); i < l; i ++ ) {

			if ( distance >= objects[ i ].distance ) {

				objects[ i - 1 ].object->visible = false;
				objects[ i     ].object->visible = true;

			} else {

				break;

			}

		}

		for( ; i < l; i ++ ) {

			objects[ i ].object->visible = false;

		}

	}

}
    
}

#endif // THREE_LOOD_CPP