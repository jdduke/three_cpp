#ifndef THREE_JSON_LOADER_H
#define THREE_JSON_LOADER_H

#include <three/loaders/loader.h>

#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>

#include <fstream>

namespace three {

typedef rapidjson::Document JSON;
typedef rapidjson::Value    JSONValue;

// TODO: Async support

inline bool readFile( const std::string& fileName, std::string& output ) {
  std::ifstream t( fileName );
  if ( !t )
    return false;

  t.seekg( 0, std::ios::end );
  size_t size = t.tellg();
  std::string buffer( size, ' ' );
  t.seekg( 0 );
  t.read( &buffer[0], size );
  output = std::move( buffer );

  return true;
}

class JSONLoader : public Loader {
public:

  typedef std::function<void( const Geometry::Ptr& )> Callback;

  JSONLoader( bool showStatus )
    : Loader( showStatus ) { }

  void load( const std::string& url,
             const Callback& callback,
             const std::string& texturePath ) {

    onLoadStart();

    loadJSON( *this, url, callback, texturePath );

  }

protected:

  static bool loadJSON( JSONLoader& context,
                        const std::string& url,
                        const Callback& callback,
                        const std::string& texturePath ) {

    std::string buffer;
    if ( !readFile( url, buffer ) ) {
      console().error() << "Three::JSONLoader: Couldn't load [" << url << "]";
      return false;
    }

    JSON json;
    if ( json.Parse<0>( buffer.c_str() ).HasParseError() ) {
      console().error() << "Three::JSONLoader: Invalid JSON faile [" << url << "]";
      return false;
    }

    context.createModel( json, callback, texturePath );

    context.onLoadComplete();
  }

  void createModel( JSON& json,
                    const Callback& callback,
                    const std::string& texturePath ) {

    auto geometry = Geometry::create();

    auto scale = json.HasMember( "scale" ) ? ( float )json["scale"].GetDouble() : 1.0f;

    std::vector<Material::Ptr> materials;

    parseMaterials( json["materials"], materials );

    initMaterials( *geometry, materials, texturePath );

    parseModel( scale, json, *geometry );

    parseSkin( json, *geometry );
    parseMorphing( scale, json, *geometry );

    geometry->computeCentroids();
    geometry->computeFaceNormals();

    if ( hasNormals( *geometry ) ) geometry->computeTangents();

    callback( geometry );

  }

  static void parseMaterials( JSONValue& json, std::vector<Material::Ptr>& materials ) {

#ifdef TODO_PARSE_MATERIALS

#endif // TODO_PARSE_MATERIALS

  }

  static void parseModel( float scale, JSON& json, Geometry& geometry ) {

#ifdef TODO_PARSE_MODEL

    auto isBitSet = []( int value, int position ) {
      return value & ( 1 << position );
    };

    auto& faces    = json.faces;
    auto& vertices = json.vertices;
    auto& normals  = json.normals;
    auto& colors   = json.colors;

    int nUvLayers = 0;

    // disregard empty arrays
    for ( int i = 0; i < json.uvs.size(); i++ ) {
      if ( json.uvs[ i ].size() ) nUvLayers ++;
    }

    geometry.faceUvs.resize( nUvLayers );
    geometry.faceVertexUvs.resize( nUvLayers );
    for ( i = 0; i < nUvLayers; i++ ) {
      geometry.faceUvs[ i ].clear();
      geometry.faceVertexUvs[ i ].clear();
    }


    int offset = 0;
    int zLength = ( int )vertices.size();

    while ( offset < zLength ) {
      Vertex vertex;
      vertex.position.x = vertices[ offset ++ ] * scale;
      vertex.position.y = vertices[ offset ++ ] * scale;
      vertex.position.z = vertices[ offset ++ ] * scale;

      geometry.vertices.push_back( vertex );
    }

    offset = 0;
    zLength = faces.length;

    while ( offset < zLength ) {

      const int type = faces[ offset ++ ];

      int nVertices = 0;

      const auto isQuad              = isBitSet( type, 0 );
      const auto hasMaterial         = isBitSet( type, 1 );
      const auto hasFaceUv           = isBitSet( type, 2 );
      const auto hasFaceVertexUv     = isBitSet( type, 3 );
      const auto hasFaceNormal       = isBitSet( type, 4 );
      const auto hasFaceVertexNormal = isBitSet( type, 5 );
      const auto hasFaceColor        = isBitSet( type, 6 );
      const auto hasFaceVertexColor  = isBitSet( type, 7 );

      //console.log("type", type, "bits", isQuad, hasMaterial, hasFaceUv, hasFaceVertexUv, hasFaceNormal, hasFaceVertexNormal, hasFaceColor, hasFaceVertexColor);

      Face face( 0, 0, 0 );

      if ( isQuad ) {

        face = Face4( 0, 0, 0, 0 );

        face.a = faces[ offset ++ ];
        face.b = faces[ offset ++ ];
        face.c = faces[ offset ++ ];
        face.d = faces[ offset ++ ];

        nVertices = 4;

      } else {

        face.a = faces[ offset ++ ];
        face.b = faces[ offset ++ ];
        face.c = faces[ offset ++ ];

        nVertices = 3;

      }

      if ( hasMaterial ) {
        const auto materialIndex = faces[ offset ++ ];
        face.materialIndex = materialIndex;
      }

      // to get face <=> uv index correspondence

      const auto fi = geometry.faces.size();

      if ( hasFaceUv ) {
        for ( int i = 0; i < nUvLayers; i++ ) {
          const auto uvLayer = json.uvs[ i ];
          const auto uvIndex = faces[ offset ++ ];

          const auto u = uvLayer[ uvIndex * 2 ];
          const auto v = uvLayer[ uvIndex * 2 + 1 ];

          geometry.faceUvs[ i ].push_back( UV( u, v ) );
        }
      }

      if ( hasFaceVertexUv ) {
        for ( i = 0; i < nUvLayers; i++ ) {

          const auto uvLayer = json.uvs[ i ];
          std::vector<UV> uvs( nVertices );

          for ( int j = 0; j < nVertices; j ++ ) {

            const auto uvIndex = faces[ offset ++ ];

            const auto u = uvLayer[ uvIndex * 2 ];
            const auto v = uvLayer[ uvIndex * 2 + 1 ];

            uvs[ j ] = UV( u, v );
          }

          geometry.faceVertexUvs[ i ].push_back( std::move( uvs ) );
        }
      }

      if ( hasFaceNormal ) {
        const auto normalIndex = faces[ offset ++ ] * 3;

        Vector3 normal;
        normal.x = normals[ normalIndex ++ ];
        normal.y = normals[ normalIndex ++ ];
        normal.z = normals[ normalIndex ];

        face.normal = normal;
      }

      if ( hasFaceVertexNormal ) {

        for ( i = 0; i < nVertices; i++ ) {
          const auto normalIndex = faces[ offset ++ ] * 3;

          Vector3 normal;
          normal.x = normals[ normalIndex ++ ];
          normal.y = normals[ normalIndex ++ ];
          normal.z = normals[ normalIndex ];

          face.vertexNormals[ i ] = normal;
        }

      }

      if ( hasFaceColor ) {
        const auto colorIndex = faces[ offset ++ ];
        face.color = Color( colors[ colorIndex ] );
      }

      if ( hasFaceVertexColor ) {
        for ( i = 0; i < nVertices; i++ ) {
          const auto colorIndex = faces[ offset ++ ];
          face.vertexColors[ i ] = Color( colors[ colorIndex ] );
        }
      }

      geometry.faces.push_back( face );
    }

#endif // TODO_PARSE_MODEL

  }

  static void parseSkin( JSON& json, Geometry& geometry ) {

#ifdef TODO_PARSE_SKIN
    if ( json.skinWeights ) {

      for ( size_t i = 0, l = json.skinWeights.size(); i < l; i += 2 ) {

        const auto x = json.skinWeights[ i     ];
        const auto y = json.skinWeights[ i + 1 ];
        const auto z = 0.f;
        const auto w = 0.f;

        geometry.skinWeights.push_back( Vector4( x, y, z, w ) );

      }

    }

    if ( json.skinIndices ) {

      for ( i = 0, l = json.skinIndices.length; i < l; i += 2 ) {

        a = json.skinIndices[ i     ];
        b = json.skinIndices[ i + 1 ];
        c = 0;
        d = 0;

        geometry.skinIndices.push( new THREE.Vector4( a, b, c, d ) );

      }

    }

    geometry.bones = json.bones;
    geometry.animation = json.animation;
#endif // TODO_PARSE_SKIN

  }

  static void parseMorphing( float scale, JSON& json, Geometry& geometry ) {

#ifdef TODO_PARSE_MORPHING

    if ( json.morphTargets != = undefined ) {

      var i, l, v, vl, dstVertices, srcVertices;

      for ( i = 0, l = json.morphTargets.length; i < l; i ++ ) {

        geometry.morphTargets[ i ] = {};
        geometry.morphTargets[ i ].name = json.morphTargets[ i ].name;
        geometry.morphTargets[ i ].vertices = [];

        dstVertices = geometry.morphTargets[ i ].vertices;
        srcVertices = json.morphTargets [ i ].vertices;

        for ( v = 0, vl = srcVertices.length; v < vl; v += 3 ) {

          var vertex = new THREE.Vector3();
          vertex.x = srcVertices[ v ] * scale;
          vertex.y = srcVertices[ v + 1 ] * scale;
          vertex.z = srcVertices[ v + 2 ] * scale;

          dstVertices.push( vertex );

        }

      }

    }

    if ( json.morphColors != = undefined ) {

      var i, l, c, cl, dstColors, srcColors, color;

      for ( i = 0, l = json.morphColors.length; i < l; i++ ) {

        geometry.morphColors[ i ] = {};
        geometry.morphColors[ i ].name = json.morphColors[ i ].name;
        geometry.morphColors[ i ].colors = [];

        dstColors = geometry.morphColors[ i ].colors;
        srcColors = json.morphColors [ i ].colors;

        for ( c = 0, cl = srcColors.length; c < cl; c += 3 ) {

          color = new THREE.Color( 0xffaa00 );
          color.setRGB( srcColors[ c ], srcColors[ c + 1 ], srcColors[ c + 2 ] );
          dstColors.push( color );

        }

      }

    }

#endif // TODO_PARSE_MORPHING

  }

};

} // namespace three

#endif // THREE_JSON_LOADER_H
