#ifndef THREE_RECTANGLE_HPP
#define THREE_RECTANGLE_HPP

#include <three/common.hpp>

namespace three {

class Rectangle {
public:

	Rectangle () { set ( 0, 0, 0, 0 ); bEmpty = true; }

	Rectangle ( const Rectangle& ) = default;
	Rectangle& operator= ( const Rectangle& ) = default;

	float getX () const      { return left; }
	float getY () const      { return top; }
	float getWidth () const  { return width; }
	float getHeight () const { return height; }
	float getLeft () const   { return left; }
	float getTop () const    { return top; }
	float getRight () const  { return right; }
	float getBottom () const { return bottom; }

	bool isEmpty () const { return bEmpty; }

	Rectangle& set ( float leftIn, float topIn, float rightIn, float bottomIn ) {

		bEmpty = false;

		left = leftIn; top = topIn;
		right = rightIn; bottom = bottomIn;

		return resize();

	}

	Rectangle& addPoint ( float x, float y ) {

		if ( isEmpty() ) {

			bEmpty = false;
			left = x; top = y;
			right = x; bottom = y;

			resize();

		} else {

			left = left < x ? left : x; // Math.min( left, x );
			top = top < y ? top : y; // Math.min( top, y );
			right = right > x ? right : x; // Math.max( right, x );
			bottom = bottom > y ? bottom : y; // Math.max( bottom, y );

			resize();
		}

		return *this;

	}

	Rectangle& add3Points ( float x1, float y1, float x2, float y2, float x3, float y3 ) {

		if ( isEmpty() ) {

			bEmpty = false;
			left = x1 < x2 ? ( x1 < x3 ? x1 : x3 ) : ( x2 < x3 ? x2 : x3 );
			top = y1 < y2 ? ( y1 < y3 ? y1 : y3 ) : ( y2 < y3 ? y2 : y3 );
			right = x1 > x2 ? ( x1 > x3 ? x1 : x3 ) : ( x2 > x3 ? x2 : x3 );
			bottom = y1 > y2 ? ( y1 > y3 ? y1 : y3 ) : ( y2 > y3 ? y2 : y3 );

			return resize();

		} else {

			left = x1 < x2 ? ( x1 < x3 ? ( x1 < left ? x1 : left ) : ( x3 < left ? x3 : left ) ) : ( x2 < x3 ? ( x2 < left ? x2 : left ) : ( x3 < left ? x3 : left ) );
			top = y1 < y2 ? ( y1 < y3 ? ( y1 < top ? y1 : top ) : ( y3 < top ? y3 : top ) ) : ( y2 < y3 ? ( y2 < top ? y2 : top ) : ( y3 < top ? y3 : top ) );
			right = x1 > x2 ? ( x1 > x3 ? ( x1 > right ? x1 : right ) : ( x3 > right ? x3 : right ) ) : ( x2 > x3 ? ( x2 > right ? x2 : right ) : ( x3 > right ? x3 : right ) );
			bottom = y1 > y2 ? ( y1 > y3 ? ( y1 > bottom ? y1 : bottom ) : ( y3 > bottom ? y3 : bottom ) ) : ( y2 > y3 ? ( y2 > bottom ? y2 : bottom ) : ( y3 > bottom ? y3 : bottom ) );

			return resize();

		};

	}

	Rectangle& addRectangle ( const Rectangle& r ) {

		if ( isEmpty() ) {

			bEmpty = false;
			left = r.getLeft(); top = r.getTop();
			right = r.getRight(); bottom = r.getBottom();

			return resize();

		} else {

			left = left < r.getLeft() ? left : r.getLeft(); // Math.min(left, r.getLeft() );
			top = top < r.getTop() ? top : r.getTop(); // Math.min(top, r.getTop() );
			right = right > r.getRight() ? right : r.getRight(); // Math.max(right, r.getRight() );
			bottom = bottom > r.getBottom() ? bottom : r.getBottom(); // Math.max(bottom, r.getBottom() );

			return resize();

		}

	}

	Rectangle& inflate ( float v ) {

		left -= v; top -= v;
		right += v; bottom += v;

		return resize();

	}

	Rectangle& minSelf ( const Rectangle& r ) {

		left = left > r.getLeft() ? left : r.getLeft(); // Math.max( left, r.getLeft() );
		top = top > r.getTop() ? top : r.getTop(); // Math.max( top, r.getTop() );
		right = right < r.getRight() ? right : r.getRight(); // Math.min( right, r.getRight() );
		bottom = bottom < r.getBottom() ? bottom : r.getBottom(); // Math.min( bottom, r.getBottom() );

		return resize();

	}

	bool intersects ( const Rectangle& r ) const {

		// http://gamemath.com/2011/09/detecting-whether-two-boxes-overlap/

		if ( right < r.getLeft() ) return false;
		if ( left > r.getRight() ) return false;
		if ( bottom < r.getTop() ) return false;
		if ( top > r.getBottom() ) return false;

		return true;

	}

	Rectangle& empty () {

		bEmpty = true;

		left = 0; top = 0;
		right = 0; bottom = 0;

		return resize();

	}

private:

	Rectangle&  resize() {

		width  = right  - left;
		height = bottom - top;

		return *this;

	}

	float left, top, right, bottom, width, height;
	bool bEmpty;

};

} // namespace three

#endif // THREE_RECTANGLE_HPP