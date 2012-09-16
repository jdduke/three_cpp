#ifndef THREE_INTERFACES_HPP
#define THREE_INTERFACES_HPP

namespace three {

class IGeometry {
public:
    virtual void applyMatrix( const Matrix4& ) = 0;
    virtual void computeBoundingBox()          = 0;
    virtual void computeBoundingSphere()       = 0;
    virtual void computeVertexNormals()        = 0;
    virtual void computeTangents()             = 0;
    virtual ~IGeometry() { }
};

}

#endif // THREE_INTERFACES_HPP