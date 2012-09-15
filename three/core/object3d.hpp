#ifndef THREE_OBJECT3D_HPP
#define THREE_OBJECT3D_HPP

#include <three/common.hpp>
#include <three/utils.hpp>

#include <three/core/vector3.hpp>
#include <three/core/quaternion.hpp>
#include <three/core/matrix4.hpp>

#include <three/extras/noncopyable.hpp>

#include <algorithm>
#include <memory>
#include <string>
#include <vector>

namespace three {

class Object3D : NonCopyable {
public:

	typedef std::shared_ptr<Object3D> Ptr;

	/////////////////////////////////////////////////////////////////////////

	virtual THREE::Type type() const { return THREE::Object3D; }

	virtual void visit( Visitor& v ) = 0;
	virtual void visit( ConstVisitor& v ) const = 0;

	virtual ~Object3D() { }

	/////////////////////////////////////////////////////////////////////////

	int id;

	std::string name;
	//properties = {};

	Object3D* parent;
	std::vector<Object3D::Ptr> children;

	Vector3 up;

	Vector3 position;
	Vector3 rotation;
	THREE::Order eulerOrder;
	Vector3 scale;

	void* renderDepth;

	bool rotationAutoUpdate;

	Matrix4 matrix;
	Matrix4 matrixWorld;
	mutable Matrix4 matrixRotationWorld;

	bool matrixAutoUpdate;
	bool matrixWorldNeedsUpdate;

	Quaternion quaternion;
	bool useQuaternion;

	float boundRadius;
	float boundRadiusScale;

	bool visible;

	bool castShadow;
	bool receiveShadow;

	bool frustumCulled;

	/////////////////////////////////////////////////////////////////////////

	void applyMatrix ( const Matrix4& m ) {

		matrix.multiply( m, matrix );

		scale = matrix.getScale();

		auto mat = Matrix4().extractRotation( matrix );
		rotation = mat.getEulerRotation( eulerOrder );

		position = matrix.getPosition();

	}

	void translate ( float distance, Vector3 axis ) {

		matrix.rotateAxis( axis );
		position.addSelf( axis.multiplyScalar( distance ) );

	}

	void translateX ( float distance ) {

		translate( distance, Vector3( 1, 0, 0 ) );

	}

	void translateY ( float distance ) {

		translate( distance, Vector3( 0, 1, 0 ) );

	}

	void translateZ ( float distance ) {

		translate( distance, Vector3( 0, 0, 1 ) );

	}

	void lookAt ( const Vector3& vector ) {

		// TODO: Add hierarchy support.

		matrix.lookAt( vector, position, up );

		if ( rotationAutoUpdate ) {

			rotation = matrix.getEulerRotation( eulerOrder );

		}

	}

	void add ( Ptr& object ) {

		if ( !object )
			return;

		if ( object.get() == this ) {

			console().warn( "Three.Object3D.add: An object can't be added as a child of itself." );
			return;

		}

		if ( object->parent != nullptr ) {

			object->parent->remove( object );

		}

		object->parent = this;
		children.push_back( object );

		// add to scene

		auto scene = this;

		while ( scene->parent != nullptr ) {

			scene = scene->parent;

		}

		if ( scene != nullptr )  {

			scene->__addObject( object );

		}

	}

	void remove ( Ptr& object ) {

		auto index = std::find( children.begin(), children.end(), object );

		if ( index != children.end() ) {

			object->parent = nullptr;
			children.erase( index );

			// remove from scene

			auto scene = this;

			while ( scene->parent != nullptr ) {

				scene = scene->parent;

			}

			if ( scene != nullptr ) {

				scene->__removeObject( object );

			}

		}

	}

	Ptr getChildByName ( const std::string& name, bool recursive ) {

		for ( auto& child : children ) {

			if ( child->name == name ) {

				return child;

			}

			if ( recursive ) {

				auto recursive_child = child->getChildByName( name, recursive );

				if ( recursive_child ) {

					return recursive_child;

				}

			}

		}

		return Object3D::Ptr();

	}

	void updateMatrix () {

		matrix.setPosition( position );

		if ( useQuaternion )  {

			matrix.setRotationFromQuaternion( quaternion );

		} else {

			matrix.setRotationFromEuler( rotation, eulerOrder );

		}

		if ( scale.x != 1 || scale.y != 1 || scale.z != 1 ) {

			matrix.scale( scale );
			boundRadiusScale = std::max( scale.x, std::max( scale.y, scale.z ) );

		}

		matrixWorldNeedsUpdate = true;

	}

	void updateMatrixWorld ( bool force = false ) {

		if ( matrixAutoUpdate ) updateMatrix();

		if ( matrixWorldNeedsUpdate || force ) {

			if ( parent != nullptr ) {

				matrixWorld.multiply( parent->matrixWorld, matrix );

			} else {

				matrixWorld.copy( matrix );

			}

			matrixWorldNeedsUpdate = false;

			force = true;

		}

		// update children

		for ( auto& child : children ) {

			child->updateMatrixWorld( force );

		}

	}

	Vector3 worldToLocal ( const Vector3& vector ) {

		return Matrix4().getInverse( matrixWorld ).multiplyVector3( vector );

	}

	Vector3 localToWorld ( const Vector3& vector ) {

		return matrixWorld.multiplyVector3( vector );

	}

/*
	clone: function () {

		// TODO

	}
*/

protected:

	Object3D ()
	: id ( Object3DCount()++ ),
	parent ( nullptr ),
	up ( 0, 1, 0 ),
	eulerOrder ( THREE::XYZ ),
	scale ( 1, 1, 1 ),
	renderDepth ( nullptr ),
	rotationAutoUpdate ( true ),
	matrixAutoUpdate ( true ),
	matrixWorldNeedsUpdate ( true ),
	useQuaternion ( false ),
	boundRadius ( 0.0f ),
	boundRadiusScale ( 1.0f ),
	visible ( true ),
	castShadow ( false ),
	receiveShadow ( false ),
	frustumCulled ( true ) { }

	virtual void __addObject(Ptr& object) { }

	virtual void __removeObject(Ptr& object) { }

private:

	static int& Object3DCount() {
		static int sObject3DCount = 0;
		return sObject3DCount;
	}

};

} // namespace three

#endif // THREE_OBJECT3D_HPP