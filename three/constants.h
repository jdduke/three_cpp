#ifndef THREE_CONSTANTS_H
#define THREE_CONSTANTS_H

namespace three {

typedef unsigned Buffer;

namespace enums {

enum Side {
  NoSide     = -1,
  FrontSide  = 0,
  BackSide   = 1,
  DoubleSide = 2
};

enum Dir {
  CW  = 0,
  CCW = 1
};

enum Shading {
  NoShading     = 0,
  FlatShading   = 1,
  SmoothShading = 2
};

enum Colors {
  NoColors     = 0,
  FaceColors   = 1,
  VertexColors = 2
};

enum Blending {
  NoBlending          = 0,
  NormalBlending      = 1,
  AdditiveBlending    = 2,
  SubtractiveBlending = 3,
  MultiplyBlending    = 4,
  CustomBlending      = 5
};

enum BlendEquation {
  AddEquation = 100,
  SubtractEquation = 101,
  ReverseSubtractEquation = 102
};

enum BlendFactor {
  ZeroFactor             = 200,
  OneFactor              = 201,
  SrcColorFactor         = 202,
  OneMinusSrcColorFactor = 203,
  SrcAlphaFactor         = 204,
  OneMinusSrcAlphaFactor = 205,
  DstAlphaFactor         = 206,
  OneMinusDstAlphaFactor = 207,
  DstColorFactor         = 208,
  OneMinusDstColorFactor = 209,
  SrcAlphaSaturateFactor = 210
};

enum TextureConstant {
  MultiplyOperation = 0,
  MixOperation = 1
};

enum Wrapping {
  RepeatWrapping         = 1000,
  ClampToEdgeWrapping    = 1001,
  MirroredRepeatWrapping = 1002
};

enum Filter {
  NearestFilter              = 1003,
  NearestMipMapNearestFilter = 1004,
  NearestMipMapLinearFilter  = 1005,
  LinearFilter               = 1006,
  LinearMipMapNearestFilter  = 1007,
  LinearMipMapLinearFilter   = 1008
};

enum DataType {
  UnsignedByteType  = 1009,
  ByteType          = 1010,
  ShortType         = 1011,
  UnsignedShortType = 1012,
  IntType           = 1013,
  UnsignedIntType   = 1014,
  FloatType         = 1015
};

enum PixelType {
  //UnsignedByteType    = 1009,
  UnsignedShort4444Type = 1016,
  UnsignedShort5551Type = 1017,
  UnsignedShort565Type  = 1018
};

enum PixelFormat {
  AlphaFormat          = 1019,
  RGBFormat            = 1020,
  RGBAFormat           = 1021,
  LuminanceFormat      = 1022,
  LuminanceAlphaFormat = 1023,
  // JD - Added
  BGRFormat            = 1024,
  BGRAFormat           = 1025,

  RGB_S3TC_DXT1_Format  = 2001,
  RGBA_S3TC_DXT1_Format = 2002,
  RGBA_S3TC_DXT3_Format = 2003,
  RGBA_S3TC_DXT5_Format = 2004
};

enum EulerRotationOrder {
  XYZ = 0,
  YZX,
  ZXY,
  XZY,
  YXZ,
  ZYX
};



enum Index {
  X = 0,
  Y,
  Z,
  W
};

enum Type {
  Object3D = 0,
  ImmediateRenderObject,
  Bone,
  Camera,
  Light,
  AmbientLight,
  AreaLight,
  PointLight,
  DirectionalLight,
  HemisphereLight,
  SpotLight,
  Scene,
  Particle,
  ParticleSystem,
  Sprite,
  LensFlare,
  Mesh,
  SkinnedMesh,
  Ribbon,
  Line,
  Vector3
};

enum GeometryType {
  Geometry = 0,
  BufferGeometry
};

enum LineType {
  LineStrip = 0,
  LinePieces = 1
};

enum LineEndType {
  Round = 0,
};

enum FaceType {
  Face3 = 0,
  Face4
};

enum LightType {
  Ambient = 0,
  Directional,
  Point,
  Spot
};

enum MaterialType {
  Material = 0,
  MeshBasicMaterial,
  MeshDepthMaterial,
  MeshFaceMaterial,
  MeshLambertMaterial,
  MeshPhongMaterial,
  LineBasicMaterial,
  MeshNormalMaterial,
  ParticleBasicMaterial,
  ShaderMaterial
};

enum RenderType {
  Opaque = 0,
  Transparent,
  Override
};

enum ShaderType {
  ShaderVertex = 0,
  ShaderFragment,
};

enum FogType {
  Fog = 0,
  FogExp2
};

enum TextureType {
  Texture = 0,
  DataTexture,
  GLRenderTarget,
  GLRenderTargetCube
};

enum PrecisionType {
  PrecisionHigh = 32,
  PrecisionMedium = 24,
  PrecisionLow = 8
};

enum UniformType {
  c = 0,
  i,
  iv,
  iv1,
  f,
  fv,
  fv1,
  v2,
  v3,
  v4,
  v2v,
  v3v,
  v4v,
  t,
  tv,
  m4,
  m4v,
  INVALID_UNIFORM,
  INVALID_ATTRIBUTE = INVALID_UNIFORM
};

typedef UniformType AttributeType;

// TODO: Implement?
typedef void ( *Mapping ) ( void );
inline void CubeReflectionMapping() {}
inline void CubeRefractionMapping() {}
inline void LatitudeReflectionMapping() {}
inline void LatitudeRefractionMapping() {}
inline void SphericalReflectionMapping() {}
inline void SphericalRefractionMapping() {}
inline void UVMapping() {}

} // namespace THREE

} // namespace three

#endif // THREE_CONSTANTS_H
