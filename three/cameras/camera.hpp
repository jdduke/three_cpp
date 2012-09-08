#ifndef THREE_CAMERA_HPP
#define THREE_CAMERA_HPP

#include <three/common.hpp>

#include <three/core/object3d.hpp>
#include <three/core/matrix4.hpp>

namespace three {

class Camera : public Object3D {
public:

	typedef std::shared_ptr<Camera> Ptr;

	static Ptr create() { return std::make_shared<Camera>(); }

	/////////////////////////////////////////////////////////////////////////

	Matrix4 matrixWorldInverse;
	Matrix4 projectionMatrix;
	Matrix4 projectionMatrixInverse;

	/////////////////////////////////////////////////////////////////////////

	void lookAt ( const Vector3& vector ) {

		matrix.lookAt( position, vector, up );

		if ( rotationAutoUpdate ) {

			rotation = matrix.getEulerRotation ( eulerOrder );

		}

	}

	/////////////////////////////////////////////////////////////////////////

protected:

	Camera ()
	: Object3D() { }

	/////////////////////////////////////////////////////////////////////////

	virtual THREE::Type getType() const { return THREE::Camera; }

	virtual void visit( Visitor& v ) { v( *this ); }

};

} // namespace three

#endif // THREE_CAMERA_HPP