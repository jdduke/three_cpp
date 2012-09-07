#ifndef THREE_PROJECTOR_HPP
#define THREE_PROJECTOR_HPP

#include <three/common.hpp>

#include <three/core/math.hpp>
#include <three/core/frustum.hpp>
#include <three/core/matrix4.hpp>
#include <three/core/vector3.hpp>
#include <three/core/vector4.hpp>

namespace three {

class Projector {
public:

	template < typename Renderable >
	struct Renderables {
		Renderables () : renderable ( nullptr ), count ( 0 ) { }

		Renderable& next() {
			if ( count == pool.size() ) {
				pool.push_back( Renderable() );
				++count;
				return pool.back();
			} else {
			return pool[ count++ ];
		}
		Renderable& current() {
			return pool[ count ];
		}
		void reset() {
			count = 0;
		}

		std::vector<Renderable> pool;
		int count;
	};

	Renderables<RenderableObject> _objects;
	Renderables<RenderableVertex> _vertices;
	Renderables<RenderableFace3> _face3s;
	Renderables<RenderableFace4> _face4s;
	Renderables<RenderableLine> _lines;
	Renderables<RenderableParticle> _particles;

	struct RenderData {
		// TODO: Fill these with pointers AFTER creating with the pool
		std::vector<RenderableObject> objects;
		std::vector<RenderableObject> sprites;
		std::vector<Object3D*> lights;
		std::vector<Renderable*> elements;
	} _renderData;

	Matrix4 _viewProjectionMatrix;
	Matrix4 _modelViewProjectionMatrix;

	Frustum _frustum;

	Projector () {

	}


	Vector3& projectVector ( Vector3& vector, const Camera& camera ) {

		camera.matrixWorldInverse.getInverse( camera.matrixWorld );

		_viewProjectionMatrix.multiply( camera.projectionMatrix, camera.matrixWorldInverse );
		_viewProjectionMatrix.multiplyVector3( vector );

		return vector;

	}

	Vector3& unprojectVector ( Vector3& vector, const Camera& camera ) {

		camera.projectionMatrixInverse.getInverse( camera.projectionMatrix );

		_viewProjectionMatrix.multiply( camera.matrixWorld, camera.projectionMatrixInverse );
		_viewProjectionMatrix.multiplyVector3( vector );

		return vector;

	}

	Ray pickingRay ( Vector3 vector, const Camera& camera ) {

		// set two vectors with opposing z values
		vector.z = -1.0f;
		auto end = Vector3( vector.x, vector.y, 1.0 );

		unprojectVector( vector, camera );
		unprojectVector( end, camera );

		// find direction from vector to end
		end.subSelf( vector ).normalize();

		return Ray( vector, end );

	}

	struct PainterSort {
		template <typename T>
		operator()(const T& a, const T& b) {
			return b.z < a.z;
		}
	};

	RenderData& projectGraph( Object3D& root, bool sort ) {

		_objects.reset();

		_renderData.objects.clear();
		_renderData.sprites.clear();
		_renderData.lights.clear();

		Vector3 _vector3;

		std::function<void(Object3D&)> projectObject = [this] ( Object3D& object ) {

			if ( !object.visible ) return;

			if ( ( object.getType() == THREE::Mesh || object.getType() == THREE::Line ) &&
			     ( !object.frustumCulled || _frustum.contains( object ) ) ) {

				_vector3.copy( object.matrixWorld.getPosition() );
				_viewProjectionMatrix.multiplyVector3( _vector3 );

				auto& renderable = _objects.next();
				renderable.object = &object;
				renderable.z = _vector3.z;

				_renderData.objects.push_back( renderable );

			} else if ( object.getType() == THREE::Sprite || object.getType() == THREE::Particle ) {

				_vector3.copy( object.matrixWorld.getPosition() );
				_viewProjectionMatrix.multiplyVector3( _vector3 );

				auto& renderable = _objects.next();
				renderable.object = &object;
				renderable.z = _vector3.z;

				_renderData.sprites.push_back( renderable );

			} else if ( object.getType() == THREE::Light ) {

				_renderData.lights.push_back( &object );

			}

			foreach ( auto& child : object.children ) {

				projectObject( child );

			}

		};

		projectObject( root );

		if ( sort )
			std::sort(_renderData.objects.begin(), _renderData.objects.end(), PainterSort() );

		return _renderData;

	}

	struct SceneVisitor() {
		Projector& p
		Matrix4& modelMatrix;

		SceneVisitor (Projector& p, Matrix4& model) : p ( p ), modelMatrix ( model ) { }

		virtual void operator() ( const Particle& p) {

		}

		virtual void operator() ( const Mesh& object ) {
			const auto& geometry          = object.geometry;
			const auto& geometryMaterials = object.geometry.materials;
			const auto& vertices          = geometry.vertices;
			const auto& faces             = geometry.faces;
			const auto& faceVertexUvs     = geometry.faceVertexUvs;

			auto rotationMatrix = object.matrixRotationWorld.extractRotation( modelMatrix );

			isFaceMaterial = object.material->getType() == THREE::MeshFaceMaterial;

			for ( const auto& v : vertices ) {

				auto& vertex = p._vertices.next();
				vertex.positionWorld.copy( vertices[ v ] );

				modelMatrix.multiplyVector3( vertex.positionWorld );

				vertex.positionScreen.copy( vertex.positionWorld );
				_viewProjectionMatrix.multiplyVector4( vertex.positionScreen );

				vertex.positionScreen.x /= vertex.positionScreen.w;
				vertex.positionScreen.y /= vertex.positionScreen.w;

				vertex.visible = vertex.positionScreen.z > near && vertex.positionScreen.z < far;

			}

			for ( const auto& face : faces ) {

				auto material = isFaceMaterial ? geometryMaterials[ face.materialIndex ] : object.material;

				if ( material == nullptr ) continue;

				auto side = material.side;

				if ( face instanceof THREE::Face3 ) {

					const auto& v1 = _vertexPool[ face.a ];
					const auto& v2 = _vertexPool[ face.b ];
					const auto& v3 = _vertexPool[ face.c ];

					if ( v1.visible && v2.visible && v3.visible ) {

						visible = ( ( v3.positionScreen.x - v1.positionScreen.x ) * ( v2.positionScreen.y - v1.positionScreen.y ) -
						            ( v3.positionScreen.y - v1.positionScreen.y ) * ( v2.positionScreen.x - v1.positionScreen.x ) ) < 0;

						if ( side == THREE::DoubleSide || visible == ( side == THREE::FrontSide ) ) {

							auto& face = _face3s.next();

							face.v1.copy( v1 );
							face.v2.copy( v2 );
							face.v3.copy( v3 );

						} else {

							continue;

						}

					} else {

						continue;

					}

				} /*else if ( face instanceof THREE::Face4 ) {

					v1 = _vertexPool[ face.a ];
					v2 = _vertexPool[ face.b ];
					v3 = _vertexPool[ face.c ];
					v4 = _vertexPool[ face.d ];

					if ( v1.visible === true && v2.visible === true && v3.visible === true && v4.visible === true ) {

						visible = ( v4.positionScreen.x - v1.positionScreen.x ) * ( v2.positionScreen.y - v1.positionScreen.y ) -
							( v4.positionScreen.y - v1.positionScreen.y ) * ( v2.positionScreen.x - v1.positionScreen.x ) < 0 ||
							( v2.positionScreen.x - v3.positionScreen.x ) * ( v4.positionScreen.y - v3.positionScreen.y ) -
							( v2.positionScreen.y - v3.positionScreen.y ) * ( v4.positionScreen.x - v3.positionScreen.x ) < 0;


						if ( side === THREE::DoubleSide || visible === ( side === THREE::FrontSide ) ) {

							_face = getNextFace4InPool();

							_face.v1.copy( v1 );
							_face.v2.copy( v2 );
							_face.v3.copy( v3 );
							_face.v4.copy( v4 );

						} else {

							continue;

						}

					} else {

						continue;

					}

				}*/

				auto& _face = _face3s.current()
				_face.normalWorld.copy( face.normal );

				if ( !visible && ( side == THREE::BackSide || side == THREE::DoubleSide ) ) _face.normalWorld.negate();
				rotationMatrix.multiplyVector3( _face.normalWorld );

				_face.centroidWorld.copy( face.centroid );
				modelMatrix.multiplyVector3( _face.centroidWorld );

				_face.centroidScreen.copy( _face.centroidWorld );
				p._viewProjectionMatrix.multiplyVector3( _face.centroidScreen );

				auto& faceVertexNormals = face.vertexNormals;

				for ( auto n = 0, nl = faceVertexNormals.size(); n < nl; n++ ) {

					auto& normal = _face.vertexNormalsWorld[ n ];
					normal.copy( faceVertexNormals[ n ] );

					if ( !visible && ( side == THREE::BackSide || side == THREE::DoubleSide ) ) normal.negate();

					rotationMatrix.multiplyVector3( normal );

				}

				for ( auto c = 0, cl = faceVertexUvs.size(); c < cl; c ++ ) {

					uvs = faceVertexUvs[ c ][ f ];

					if ( uvs == nullptr ) continue;

					for ( auto u = 0, ul = uvs.size(); u < ul; u ++ ) {

						_face.uvs[ c ][ u ] = uvs[ u ];

					}

				}

				_face.material = material;

				_face.z = _face.centroidScreen.z;

				_renderData.elements.push_back( _face );

			}
		}

		virtual void operator() ( const Line& object ) {

			p._modelViewProjectionMatrix.multiply( _viewProjectionMatrix, modelMatrix );

			auto& vertices = object.geometry.vertices;

			auto& v1 = _vertices.next();
			v1.positionScreen.copy( vertices[ 0 ] );
			p._modelViewProjectionMatrix.multiplyVector4( v1.positionScreen );

			// Handle LineStrip and LinePieces
			auto step = object.type == THREE::LinePieces ? 2 : 1;

			for ( v = 1, vl = vertices.size(); v < vl; v ++ ) {

				auto& v1 = _vertices.next();
				v1.positionScreen.copy( vertices[ v ] );
				p._modelViewProjectionMatrix.multiplyVector4( v1.positionScreen );

				if ( ( v + 1 ) % step > 0 ) continue;

				auto& v2 = _vertices.pool[ _vertices.count - 2 ];

				auto _clippedVertex1PositionScreen = v1.positionScreen;
				auto _clippedVertex2PositionScreen = v2.positionScreen;

				if ( clipLine( _clippedVertex1PositionScreen, _clippedVertex2PositionScreen ) === true ) {

					// Perform the perspective divide
					_clippedVertex1PositionScreen.multiplyScalar( 1.f / _clippedVertex1PositionScreen.w );
					_clippedVertex2PositionScreen.multiplyScalar( 1.f / _clippedVertex2PositionScreen.w );

					auto& _line = _lines.next();
					_line.v1.positionScreen.copy( _clippedVertex1PositionScreen );
					_line.v2.positionScreen.copy( _clippedVertex2PositionScreen );

					_line.z = Math.max( _clippedVertex1PositionScreen.z, _clippedVertex2PositionScreen.z );

					_line.material = object.material;

					_renderData.elements.push_back( _line );

				}
			}
		}
	};

	RenderData& projectScene ( Scene& scene, Camera& camera, bool sort ) {

		var near = camera.near, far = camera.far, visible = false,
		o, ol, v, vl, f, fl, n, nl, c, cl, u, ul, object,
		modelMatrix, rotationMatrix,
		geometry, geometryMaterials, vertices, vertex, vertexPositionScreen,
		faces, face, faceVertexNormals, normal, faceVertexUvs, uvs,
		v1, v2, v3, v4, isFaceMaterial, material, side;

		_face3s.reset();
		_face4s.reset();
		_lines.reset();
		_particles.reset();

		_renderData.elements.clear();

		scene.updateMatrixWorld();

		if ( camera.parent == nullptr ) camera.updateMatrixWorld();

		camera.matrixWorldInverse.getInverse( camera.matrixWorld );

		_viewProjectionMatrix.multiply( camera.projectionMatrix, camera.matrixWorldInverse );

		_frustum.setFromMatrix( _viewProjectionMatrix );

		_renderData = projectGraph( scene, false );

		for ( auto& renderObject : _renderData.objects )

			auto& object = *renderObject.object;

			modelMatrix = object.matrixWorld;

			_vertices.reset();

			object.visit( SceneVisitor(*this, modelMatrix) );

		}

		Vector4 _vector4;

		for ( auto& renderSprite : _renderData.sprites ) {

			auto& object = *renderSprite.object;

			const auto& modelMatrix = object.matrixWorld;

			if ( object.getType() == THREE::Particle ) {

				_vector4.set( modelMatrix.elements[12], modelMatrix.elements[13], modelMatrix.elements[14], 1 );
				_viewProjectionMatrix.multiplyVector4( _vector4 );

				_vector4.z /= _vector4.w;

				if ( _vector4.z > 0 && _vector4.z < 1 ) {

					auto& _particle = _particles.next();
					_particle.object = &object;
					_particle.x = _vector4.x / _vector4.w;
					_particle.y = _vector4.y / _vector4.w;
					_particle.z = _vector4.z;

					_particle.rotation = object.rotation.z;

					_particle.scale.x = object.scale.x * Math.abs( _particle.x - ( _vector4.x + camera.projectionMatrix.elements[0] ) / ( _vector4.w + camera.projectionMatrix.elements[12] ) );
					_particle.scale.y = object.scale.y * Math.abs( _particle.y - ( _vector4.y + camera.projectionMatrix.elements[5] ) / ( _vector4.w + camera.projectionMatrix.elements[13] ) );

					_particle.material = object.material;

					_renderData.elements.push_back( _particle );

				}

			}

		}

		if ( sort )
			std::sort(_renderData.elements.begin(), _renderData.elements.end(), PainterSort() );

		return _renderData;

	}

	//

	bool clipLine( const Vector4& s1, const Vector4& s2 ) {

		float alpha1 = 0, alpha2 = 1,

		// Calculate the boundary coordinate of each vertex for the near and far clip planes,
		// Z = -1 and Z = +1, respectively.
		auto bc1near =  s1.z + s1.w,
		auto bc2near =  s2.z + s2.w,
		auto bc1far =  - s1.z + s1.w,
		auto bc2far =  - s2.z + s2.w;

		if ( bc1near >= 0 && bc2near >= 0 && bc1far >= 0 && bc2far >= 0 ) {

			// Both vertices lie entirely within all clip planes.
			return true;

		} else if ( ( bc1near < 0 && bc2near < 0) || (bc1far < 0 && bc2far < 0 ) ) {

			// Both vertices lie entirely outside one of the clip planes.
			return false;

		} else {

			// The line segment spans at least one clip plane.

			if ( bc1near < 0 ) {

				// v1 lies outside the near plane, v2 inside
				alpha1 = Math.max( alpha1, bc1near / ( bc1near - bc2near ) );

			} else if ( bc2near < 0 ) {

				// v2 lies outside the near plane, v1 inside
				alpha2 = Math.min( alpha2, bc1near / ( bc1near - bc2near ) );

			}

			if ( bc1far < 0 ) {

				// v1 lies outside the far plane, v2 inside
				alpha1 = Math.max( alpha1, bc1far / ( bc1far - bc2far ) );

			} else if ( bc2far < 0 ) {

				// v2 lies outside the far plane, v2 inside
				alpha2 = Math.min( alpha2, bc1far / ( bc1far - bc2far ) );

			}

			if ( alpha2 < alpha1 ) {

				// The line segment spans two boundaries, but is outside both of them.
				// (This can't happen when we're only clipping against just near/far but good
				//  to leave the check here for future usage if other clip planes are added.)
				return false;

			} else {

				// Update the s1 and s2 vertices to match the clipped line segment.
				s1.lerpSelf( s2, alpha1 );
				s2.lerpSelf( s1, 1 - alpha2 );

				return true;

			}

		}

	}

};

} // namespace three

#endif // THREE_PROJECTOR_HPP