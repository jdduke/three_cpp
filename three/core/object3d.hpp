#ifndef THREE_OBJECT3D_HPP
#define THREE_OBJECT3D_HPP

#include <three/common.hpp>

#include <memory>

namespace three {

class Object3D {
public:

	typedef std::shared_ptr<Object3D> Ptr;

	int id;

	std::string name;
	//properties = {};

	Object3D* parent;
	std::vector<Object3D::Ptr> children;

	Vector3 up;

	Vector3 position;
	Vector3 rotation;
	Order eulerOrder;
	Vector3 scale;

	void* renderDepth = nullptr;

	bool rotationAutoUpdate = true;

	Matrix4 matrix;
	Matrix4 matrixWorld;
	Matrix4 matrixRotationWorld;

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

	Object3D::Ptr create() {
		return make_shared<Object3D>();
	}

	void applyMatrix ( const Matrix4& m ) {

		matrix.multiply( m, matrix );

		scale.getScaleFromMatrix( matrix );

		auto mat = Matrix4().extractRotation( matrix );
		rotation.setEulerFromRotationMatrix( mat, eulerOrder );

		position.getPositionFromMatrix( matrix );

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

			rotation.setEulerFromRotationMatrix( matrix, eulerOrder );

		}

	}

	void add ( Ptr& object ) {

		if ( !object )
			return;

		if ( object.get() == this ) {

			console.warn( 'THREE.Object3D.add: An object can\'t be added as a child of itself.' );
			return;

		}

		if ( object.parent != nullptr ) {

			object.parent->remove( object );

		}

		object.parent = this;
		children.push( object );

		// add to scene

		auto scene = this;

		while ( scene.parent != nullptr ) {

			scene = scene.parent;

		}

		if ( scene != nullptr )  {

			scene.__addObject( object );

		}

	}

	void remove ( Ptr& object ) {

		auto index = std::find( children, object );

		if ( index != children.end() ) {

			object.parent = nullptr;
			children.erase( index );

			// remove from scene

			auto scene = this;

			while ( scene.parent !== undefined ) {

				scene = scene.parent;

			}

			if ( scene != nullptr ) {

				scene.__removeObject( object );

			}

		}

	}

	Ptr getChildByName ( const std::string& name, bool recursive ) {


		foreach (const auto& child : children ) {

			if ( child.name == name ) {

				return child;

			}

			if ( recursive ) {

				auto recursive_child = child.getChildByName( name, recursive );

				if ( recursive_child ) {

					return recursive_child;

				}

			}

		}

		return Object3D::Ptr;

	}

	void updateMatrix () {

		matrix.setPosition( position );

		if ( useQuaternion === true )  {

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

				matrixWorld.multiply( parent.matrixWorld, matrix );

			} else {

				matrixWorld.copy( matrix );

			}

			matrixWorldNeedsUpdate = false;

			force = true;

		}

		// update children

		for ( const auto& chidl : children ) {

			child.updateMatrixWorld( force );

		}

	}

	Vector3 worldToLocal ( const Vector3& vector ) {

		return Matrix4().getInverse( matrixWorld ).multiplyVector3( vector );

	},

	Vector3 localToWorld ( const Vector3& vector ) {

		return matrixWorld.multiplyVector3( vector );

	}

/*
	clone: function () {

		// TODO

	}
*/

protected:
	virtual void __addObject(Ptr& object) { }
	virtual void __removeObject(Ptr& object) { }

	virtual THREE::Type getType() const { return THREE::Object3D; }

	virtual void visit()( Visitor& v ) { v( *this ); }


private:

	Object3D ()
	: id ( Object3DCount++ ),
	parent ( nullptr ),
	up ( 0, 1, 0 ),
	eulerOrder ( XYZ ),
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


	static int Object3DCount;

};

int Object3D::Object3DCount = 0;

} // namespace three

#endif // THREE_OBJECT3D_HPP