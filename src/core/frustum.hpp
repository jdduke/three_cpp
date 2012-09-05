#ifndef _FRUSTUM_HPP_
#define _FRUSTUM_HPP_

#include <array>

#include "vector4.hpp"

namespace three {

class Frustum {
public:

    void setFromMatrix ( const Matrix4& m ) {

        auto planes = this.planes;

        const auto me = m.elements;
        auto me0 = me[0], me1 = me[1], me2 = me[2], me3 = me[3];
        auto me4 = me[4], me5 = me[5], me6 = me[6], me7 = me[7];
        auto me8 = me[8], me9 = me[9], me10 = me[10], me11 = me[11];
        auto me12 = me[12], me13 = me[13], me14 = me[14], me15 = me[15];

        planes[ 0 ].set ( me3 - me0, me7 - me4, me11 - me8, me15 - me12 );
        planes[ 1 ].set ( me3 + me0, me7 + me4, me11 + me8, me15 + me12 );
        planes[ 2 ].set ( me3 + me1, me7 + me5, me11 + me9, me15 + me13 );
        planes[ 3 ].set ( me3 - me1, me7 - me5, me11 - me9, me15 - me13 );
        planes[ 4 ].set ( me3 - me2, me7 - me6, me11 - me10, me15 - me14 );
        planes[ 5 ].set ( me3 + me2, me7 + me6, me11 + me10, me15 + me14 );

        for ( const auto & plane : planes ) {

            plane.divideScalar ( std::sqrt ( plane.x * plane.x + plane.y * plane.y + plane.z * plane.z ) );

        }

    }

    bool contains ( Object object ) {

        auto distance = 0.0f;
        const auto& matrix = object.matrixWorld;
        const auto& me = matrix.elements;
        auto radius = - object.geometry.boundingSphere.radius * matrix.getMaxScaleOnAxis();

        for ( var i = 0; i < 6; i ++ ) {

            distance = planes[ i ].x * me[12] + planes[ i ].y * me[13] + planes[ i ].z * me[14] + planes[ i ].w;
            if ( distance <= radius ) return false;

        }

        return true;

    }

    std::array<Vector4, 6> planes;
};

}

#endif