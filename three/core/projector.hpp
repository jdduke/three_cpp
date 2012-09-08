#ifndef THREE_PROJECTOR_HPP
#define THREE_PROJECTOR_HPP

#include <three/common.hpp>

#include <three/core/math.hpp>
#include <three/core/frustum.hpp>
#include <three/core/matrix4.hpp>
#include <three/core/ray.hpp>
#include <three/core/vector3.hpp>
#include <three/core/vector4.hpp>

#include <three/objects/line.hpp>
#include <three/objects/mesh.hpp>
#include <three/objects/particle.hpp>

#include <three/renderers/renderables/renderable_object.hpp>
#include <three/renderers/renderables/renderable_vertex.hpp>
#include <three/renderers/renderables/renderable_face.hpp>
#include <three/renderers/renderables/renderable_line.hpp>
#include <three/renderers/renderables/renderable_particle.hpp>

#include <three/scenes/scene.hpp>

#include <vector>

namespace three {

class Projector {
public:

	struct RenderData {
		// TODO: Fill these with pointers AFTER creating with the pool
		std::vector<RenderableObject> objects;
		std::vector<RenderableObject> sprites;
		std::vector<Object3D*>        lights;
		std::vector<Renderable*>      elements;
	};

protected:

	template < typename Renderable >
	struct Renderables {
		Renderables () : count ( 0 ) { }

		Renderable& next() {
			if ( count == pool.size() ) {
				pool.push_back( Renderable() );
				++count;
				return pool.back();
			} else {
				return pool[ count++ ];
			}
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

	Renderables<RenderableObject>   _objects;
	Renderables<RenderableVertex>   _vertices;
	Renderables<RenderableFace>     _faces;
	Renderables<RenderableLine>     _lines;
	Renderables<RenderableParticle> _particles;

	RenderData _renderData;

	Matrix4 _viewProjectionMatrix;
	Matrix4 _modelViewProjectionMatrix;

	Frustum _frustum;

public:

	Projector () { }

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
		bool operator()(const RenderableObject& a, const RenderableObject& b) {
			return b.z < a.z;
		}
		bool operator()(const Renderable* a, const Renderable* b) {
			return b->z < a->z;
		}
	};

	RenderData& projectGraph( Object3D& root, bool sort ) {

		_objects.reset();

		_renderData.objects.clear();
		_renderData.sprites.clear();
		_renderData.lights.clear();

		Vector3 _vector3;

		std::function<void(Object3D&)> projectObject = [&projectObject,this] ( Object3D& object ) {

			if ( !object.visible ) return;

			if ( ( object.type() == THREE::Mesh || object.type() == THREE::Line ) &&
			     ( !object.frustumCulled || _frustum.contains( object ) ) ) {

				Vector3 _vector3 = object.matrixWorld.getPosition();
				_viewProjectionMatrix.multiplyVector3( _vector3 );

				auto& renderable = _objects.next();
				renderable.object = &object;
				renderable.z = _vector3.z;

				_renderData.objects.push_back( renderable );

			} else if ( object.type() == THREE::Sprite || object.type() == THREE::Particle ) {

				Vector3 _vector3 = object.matrixWorld.getPosition();
				_viewProjectionMatrix.multiplyVector3( _vector3 );

				auto& renderable = _objects.next();
				renderable.object = &object;
				renderable.z = _vector3.z;

				_renderData.sprites.push_back( renderable );

			} else if ( object.type() == THREE::Light ) {

				_renderData.lights.push_back( &object );

			}

			for ( auto& child : object.children ) {

				projectObject( *child );

			}

		};

		projectObject( root );

		if ( sort )
			std::sort(_renderData.objects.begin(), _renderData.objects.end(), PainterSort() );

		return _renderData;

	}

	friend struct SceneVisitor;

	struct SceneVisitor : public ConstVisitor {
		Projector& p;
		Camera& c;
		Matrix4& modelMatrix;
		Matrix4& viewProjectionMatrix;

		SceneVisitor (Projector& p, Camera& c, Matrix4& model, Matrix4& viewProjection)
		: p ( p ), c ( c ), modelMatrix ( model ), viewProjectionMatrix ( viewProjection ) { }

		virtual void operator() ( const Particle& p) { }
		virtual void operator() ( const Mesh& object ) {
			const auto& geometry          = *object.geometry;
			const auto& geometryMaterials = geometry.materials;
			const auto& vertices          = geometry.vertices;
			const auto& faces             = geometry.faces;
			const auto& faceVertexUvs     = geometry.faceVertexUvs;

			auto rotationMatrix = object.matrixRotationWorld.extractRotation( modelMatrix );

			auto isFaceMaterial = object.material->type() == THREE::MeshFaceMaterial;

			for ( const auto& v : vertices ) {

				auto& vertex = p._vertices.next();
				vertex.positionWorld.copy( v.position );

				modelMatrix.multiplyVector3( vertex.positionWorld );

				vertex.positionScreen.copy( vertex.positionWorld );
				viewProjectionMatrix.multiplyVector4( vertex.positionScreen );

				vertex.positionScreen.x /= vertex.positionScreen.w;
				vertex.positionScreen.y /= vertex.positionScreen.w;

				vertex.visible = vertex.positionScreen.z > c.near && vertex.positionScreen.z < c.far;

			}

			for ( int f = 0; f < faces.size(); ++f ) {

				auto& face = faces[ f ];

				bool visible = false;

				auto material = isFaceMaterial ? geometryMaterials[ face.materialIndex ] : object.material;

				if ( material == nullptr ) continue;

				auto side = material->side;

				if ( face.type() == THREE::Face3 ) {

					const auto& v1 = p._vertices.pool[ face.a ];
					const auto& v2 = p._vertices.pool[ face.b ];
					const auto& v3 = p._vertices.pool[ face.c ];

					if ( v1.visible && v2.visible && v3.visible ) {

						const auto visible = ( ( v3.positionScreen.x - v1.positionScreen.x ) * ( v2.positionScreen.y - v1.positionScreen.y ) -
						                       ( v3.positionScreen.y - v1.positionScreen.y ) * ( v2.positionScreen.x - v1.positionScreen.x ) ) < 0;

						if ( side == THREE::DoubleSide || visible == ( side == THREE::FrontSide ) ) {

							auto& face = p._faces.next();

							face.v1.copy( v1 );
							face.v2.copy( v2 );
							face.v3.copy( v3 );

						} else {

							continue;

						}

					} else {

						continue;

					}

				} else if ( face.type() == THREE::Face4 ) {

					const auto& v1 = p._vertices.pool[ face.a ];
					const auto& v2 = p._vertices.pool[ face.b ];
					const auto& v3 = p._vertices.pool[ face.c ];
					const auto& v4 = p._vertices.pool[ face.d ];

					if ( v1.visible && v2.visible && v3.visible && v4.visible ) {

						const auto visible = ( v4.positionScreen.x - v1.positionScreen.x ) * ( v2.positionScreen.y - v1.positionScreen.y ) -
						                     ( v4.positionScreen.y - v1.positionScreen.y ) * ( v2.positionScreen.x - v1.positionScreen.x ) < 0 ||
						                     ( v2.positionScreen.x - v3.positionScreen.x ) * ( v4.positionScreen.y - v3.positionScreen.y ) -
						                     ( v2.positionScreen.y - v3.positionScreen.y ) * ( v4.positionScreen.x - v3.positionScreen.x ) < 0;


						if ( side == THREE::DoubleSide || visible == ( side == THREE::FrontSide ) ) {

							auto& _face = p._faces.next();

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

				}

				auto& _face = p._faces.current();
				_face.normalWorld.copy( face.normal );

				if ( !visible && ( side == THREE::BackSide || side == THREE::DoubleSide ) ) _face.normalWorld.negate();
				rotationMatrix.multiplyVector3( _face.normalWorld );

				_face.centroidWorld.copy( face.centroid );
				modelMatrix.multiplyVector3( _face.centroidWorld );

				_face.centroidScreen.copy( _face.centroidWorld );
				p._viewProjectionMatrix.multiplyVector3( _face.centroidScreen );

				auto& faceVertexNormals = face.vertexNormals;

				for ( auto n = 0, nl = face.size(); n < nl; n++ ) {

					auto& normal = _face.vertexNormalsWorld[ n ];
					normal.copy( faceVertexNormals[ n ] );

					if ( !visible && ( side == THREE::BackSide || side == THREE::DoubleSide ) ) normal.negate();

					rotationMatrix.multiplyVector3( normal );

				}

				for ( int c = 0, cl = face.size(); c < cl; c ++ ) {

					const auto& uvs = faceVertexUvs[ c ][ f ];

					for ( int u = 0, ul = uvs.size(); u < ul; u ++ ) {

						_face.uvs[ c ][ u ] = uvs[ u ];

					}

				}

				_face.material = material.get();

				_face.z = _face.centroidScreen.z;

				// TODO: FIX!!!
				p._renderData.elements.push_back( &_face );

			}
		}

		virtual void operator() ( const Line& object ) {

			p._modelViewProjectionMatrix.multiply( p._viewProjectionMatrix, modelMatrix );

			auto& vertices = object.geometry->vertices;

			auto& v1 = p._vertices.next();
			v1.positionScreen.copy( vertices[ 0 ].position );
			p._modelViewProjectionMatrix.multiplyVector4( v1.positionScreen );

			// Handle LineStrip and LinePieces
			auto step = object.lineType == THREE::LinePieces ? 2 : 1;

			for ( int v = 1, vl = vertices.size(); v < vl; v ++ ) {

				auto& v1 = p._vertices.next();
				v1.positionScreen.copy( vertices[ v ].position );
				p._modelViewProjectionMatrix.multiplyVector4( v1.positionScreen );

				if ( ( v + 1 ) % step > 0 ) continue;

				auto& v2 = p._vertices.pool[ p._vertices.count - 2 ];

				auto _clippedVertex1PositionScreen = v1.positionScreen;
				auto _clippedVertex2PositionScreen = v2.positionScreen;

				if ( clipLine( _clippedVertex1PositionScreen, _clippedVertex2PositionScreen ) ) {

					// Perform the perspective divide
					_clippedVertex1PositionScreen.multiplyScalar( 1.f / _clippedVertex1PositionScreen.w );
					_clippedVertex2PositionScreen.multiplyScalar( 1.f / _clippedVertex2PositionScreen.w );

					auto& _line = p._lines.next();
					_line.v1.positionScreen.copy( _clippedVertex1PositionScreen );
					_line.v2.positionScreen.copy( _clippedVertex2PositionScreen );

					_line.z = Math::max( _clippedVertex1PositionScreen.z, _clippedVertex2PositionScreen.z );

					_line.material = object.material.get();

					p._renderData.elements.push_back( &_line );

				}
			}
		}
	};

	RenderData& projectScene ( Scene& scene, Camera& camera, bool sort ) {

		_faces.reset();
		_lines.reset();
		_particles.reset();

		_renderData.elements.clear();

		scene.updateMatrixWorld();

		if ( camera.parent == nullptr ) camera.updateMatrixWorld();

		camera.matrixWorldInverse.getInverse( camera.matrixWorld );

		_viewProjectionMatrix.multiply( camera.projectionMatrix, camera.matrixWorldInverse );

		_frustum.setFromMatrix( _viewProjectionMatrix );

		_renderData = projectGraph( scene, false );

		for ( auto& renderObject : _renderData.objects ) {

			auto& object = *renderObject.object;

			auto& modelMatrix = object.matrixWorld;

			_vertices.reset();

			SceneVisitor visitor( *this, camera, modelMatrix, _viewProjectionMatrix );

			object.visit( visitor );

		}

		Vector4 _vector4;

		for ( auto& renderSprite : _renderData.sprites ) {

			auto& object = *renderSprite.object;

			struct SpriteVisitor : public Visitor {

				SpriteVisitor ( Projector& p, Camera& c )
				 : p ( p ), c ( c ) { }

				void operator() ( Particle& object ) {

					const auto& modelMatrix = object.matrixWorld;

					Vector4 _vector4 ( modelMatrix.elements[12], modelMatrix.elements[13], modelMatrix.elements[14], 1 );
					p._viewProjectionMatrix.multiplyVector4( _vector4 );

					_vector4.z /= _vector4.w;

					if ( _vector4.z > 0 && _vector4.z < 1 ) {

						auto& _particle = p._particles.next();
						_particle.object = &object;
						_particle.x = _vector4.x / _vector4.w;
						_particle.y = _vector4.y / _vector4.w;
						_particle.z = _vector4.z;

						_particle.rotation = object.rotation.z;

						_particle.scale.x = object.scale.x * Math::abs( _particle.x - ( _vector4.x + c.projectionMatrix.elements[0] ) / ( _vector4.w + c.projectionMatrix.elements[12] ) );
						_particle.scale.y = object.scale.y * Math::abs( _particle.y - ( _vector4.y + c.projectionMatrix.elements[5] ) / ( _vector4.w + c.projectionMatrix.elements[13] ) );

						_particle.material = object.material.get();

						p._renderData.elements.push_back( &_particle );

					}

				}

				Projector& p;
				Camera& c;

			} visitor ( *this, camera );

			object.visit ( visitor );

		}

		if ( sort ) {

			std::sort( _renderData.elements.begin(), _renderData.elements.end(), PainterSort() );

		}

		return _renderData;

	}

	//

	static bool clipLine( Vector4& s1, Vector4& s2 ) {

		float alpha1 = 0, alpha2 = 1;

		// Calculate the boundary coordinate of each vertex for the near and far clip planes,
		// Z = -1 and Z = +1, respectively.
		auto bc1near =  s1.z + s1.w;
		auto bc2near =  s2.z + s2.w;
		auto bc1far =  - s1.z + s1.w;
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
				alpha1 = Math::max( alpha1, bc1near / ( bc1near - bc2near ) );

			} else if ( bc2near < 0 ) {

				// v2 lies outside the near plane, v1 inside
				alpha2 = Math::min( alpha2, bc1near / ( bc1near - bc2near ) );

			}

			if ( bc1far < 0 ) {

				// v1 lies outside the far plane, v2 inside
				alpha1 = Math::max( alpha1, bc1far / ( bc1far - bc2far ) );

			} else if ( bc2far < 0 ) {

				// v2 lies outside the far plane, v2 inside
				alpha2 = Math::min( alpha2, bc1far / ( bc1far - bc2far ) );

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