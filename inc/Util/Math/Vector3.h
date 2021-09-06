#pragma once

//Disables Conversion from 'T' to 'xxx' possible loss of data warnings.
#pragma warning( disable : 4244 ) 

#include <math.h>
#include <sstream>
#include <cmath>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <cassert>
#include <cstdio>

#include <Util/Math/Vector2.h>
#include <Util/Color.h>

#ifndef M_PI
#define M_PI           3.14159265358979323846  /* pi */
#endif

#define DEG2RAD(x) ((x * M_PI) / 180.0)
//#define EPSILON (4.37114e-07)

#ifndef EPSILON
const double epsilon = 4.37114e-05;
#define EPSILON epsilon
#endif


/**
 * Class for three dimensional vector.
 * There are four ways of accessing vector components.
 * Let's have <code>Vector3f v</code>, you can either:
 * <ul>
 * 	<li>access as position (x,y,z) &mdash; <code>v.x = v.y = v.z = 1;</code></li>
 * 	<li>access as texture coordinate (s,t,u) &mdash; <code>v.s = v.t = v.u = 1;</code></li>
 * 	<li>access as color (r,g,b) &mdash; <code>v.r = v.g = v.b = 1;</code></li>
 * 	<li>access via operator[] &mdash; <code>v[0] = v[1] = v[2] = 1;</code></li>
 * </ul>
 */
template<class T>
class Vector3
{
public:
	//T x, y, z;
	union
	{
		/**
		 * First element of vector, alias for X-coordinate.
		 */
		T x;

		/**
		 * First element of vector, alias for S-coordinate.
		 * For textures notation.
		 */
		T s;

		/**
		 * First element of vector, alias for R-coordinate.
		 * For color notation.
		 */
		T r;
	};

	union
	{
		/**
		 * Second element of vector, alias for Y-coordinate.
		 */
		T y;
		/**
		 * Second element of vector, alias for T-coordinate.
		 * For textures notation.
		 */
		T t;
		/**
		 * Second element of vector, alias for G-coordinate.
		 * For color notation.
		 */
		T g;
	};

	union
	{
		/**
		 * Third element of vector, alias for Z-coordinate.
		 */
		T z;

		/**
		 * Third element of vector, alias for U-coordinate.
		 * For textures notation.
		 */
		T u;
		/**
		 * Third element of vector, alias for B-coordinate.
		 * For color notation.
		 */
		T b;
	};

	//----------------[ constructors ]--------------------------
	/**
	 * Creates and sets to (0,0,0)
	 */
	Vector3()
		: x(0), y(0), z(0)
	{
	}

	/**
	 * Creates and sets to (x,y,z)
	 * @param nx initial x-coordinate value
	 * @param ny initial y-coordinate value
	 * @param nz initial z-coordinate value
	 */
	Vector3(T nx, T ny, T nz)
		: x(nx), y(ny), z(nz)
	{
	}

	/**
	 * Copy constructor.
	 * @param src Source of data for new created Vector3 instance.
	 */
	Vector3(const Vector3<T>& src)
		: x(src.x), y(src.y), z(src.z)
	{
	}

	/**
	 * Copy casting constructor.
	 * @param src Source of data for new created Vector3 instance.
	 */
	template<class FromT>
	Vector3(const Vector3<FromT>& src)
		: x(static_cast<T>(src.x)), y(static_cast<T>(src.y)), z(static_cast<T>(src.z))
	{
	}

	//std::string& operator() { return toString(); }
	

	/**********************/
	/***    Setters     ***/
	/**********************/


	void setVector(T X, T Y, T Z) { x = X; y = Y; z = Z; }
	void setVector(Vector3 other) { x = other.x; y = other.y; z = other.z; }
	
	//----------------[ access operators ]-------------------
	/**
	 * Copy operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector3<T> operator=(const Vector3<T>& rhs)
	{
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		return *this;
	}

	/**
	 * Copy casting operator.
	 * @param rhs Right hand side argument of binary operator.
	 */
	template<class FromT>
	Vector3<T> operator=(const Vector3<FromT>& rhs)
	{
		x = static_cast<T>(rhs.x);
		y = static_cast<T>(rhs.y);
		z = static_cast<T>(rhs.z);
		return *this;
	}

	/**
	 * Array access operator
	 * @param n Array index
	 * @return For n = 0, reference to x coordinate, n = 1
	 * reference to y, else reference to z
	 * y coordinate.
	 */
	T& operator[](int n)
	{
		assert(n >= 0 && n <= 2);
		if (0 == n)
			return x;
		else if (1 == n)
			return y;
		else
			return z;
	}

	/**
	 * Constant array access operator
	 * @param n Array index
	 * @return For n = 0, reference to x coordinate, n = 1
	 * reference to y, else reference to z
	 * y coordinate.
	 */
	const T& operator[](int n) const
	{
		assert(n >= 0 && n <= 2);
		if (0 == n)
			return x;
		else if (1 == n)
			return y;
		else
			return z;
	}



	//---------------[ vector arithmetic operator ]--------------
	/**
	 * Addition operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector3<T> operator+(const Vector3<T>& rhs) const
	{
		return Vector3<T>(x + rhs.x, y + rhs.y, z + rhs.z);
	}

	/**
	 * Subtraction operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector3<T> operator-(const Vector3<T>& rhs) const
	{
		return Vector3<T>(x - rhs.x, y - rhs.y, z - rhs.z);
	}

	/**
	 * Multiplication operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector3<T> operator*(const Vector3<T>& rhs) const
	{
		return Vector3<T>(x * rhs.x, y * rhs.y, z * rhs.z);
	}

	/**
	 * Division operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector3<T> operator/(const Vector3<T>& rhs) const
	{
		return Vector3<T>(x / rhs.x, y / rhs.y, z / rhs.z);
	}

	/**
	 * Addition operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector3<T>& operator+=(const Vector3<T>& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}

	/**
	 * Subtraction operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector3<T>& operator-=(const Vector3<T>& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		return *this;
	}

	/**
	 * Multiplication operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector3<T>& operator*=(const Vector3<T>& rhs)
	{
		x *= rhs.x;
		y *= rhs.y;
		z *= rhs.z;
		return *this;
	}

	/**
	 * Division operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector3<T>& operator/=(const Vector3<T>& rhs)
	{
		x /= rhs.x;
		y /= rhs.y;
		z /= rhs.z;
		return *this;
	}



	float dist(const Vector3<T>& rhs) const
	{
		float dx(fabsf(float(x - rhs.x)));
		float dy(fabsf(float(y - rhs.y)));
		float dz(fabsf(float(z - rhs.z)));
		return sqrt(float(dx * dx + dy * dy + dz * dz));
	}

	/**
	 * Dot product of two vectors.
	 * @param rhs Right hand side argument of binary operator.
	 */
	T dot(const Vector3<T>& rhs) const
	{
		return x * rhs.x + y * rhs.y + z * rhs.z;
	}

	/**
	 * Cross product operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector3<T> cross(const Vector3<T>& rhs) const
	{
		return Vector3<T>(y * rhs.z - rhs.y * z, z * rhs.x - rhs.z * x, x * rhs.y - rhs.x * y);
	}

	//--------------[ scalar vector operator ]--------------------
	/**
	 * Addition operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector3<T> operator+(T rhs) const
	{
		return Vector3<T>(x + rhs, y + rhs, z + rhs);
	}

	/**
	 * Subtraction operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector3<T> operator-(T rhs) const
	{
		return Vector3<T>(x - rhs, y - rhs, z - rhs);
	}

	/**
	 * Multiplication operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector3<T> operator*(T rhs) const
	{
		return Vector3<T>(x * rhs, y * rhs, z * rhs);
	}

	/**
	 * Division operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector3<T> operator/(T rhs) const
	{
		return Vector3<T>(x / rhs, y / rhs, z / rhs);
	}

	/**
	 * Addition operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector3<T>& operator+=(T rhs)
	{
		x += rhs;
		y += rhs;
		z += rhs;
		return *this;
	}

	/**
	 * Subtraction operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector3<T>& operator-=(T rhs)
	{
		x -= rhs;
		y -= rhs;
		z -= rhs;
		return *this;
	}

	/**
	 * Multiplication operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector3<T>& operator*=(T rhs)
	{
		x *= rhs;
		y *= rhs;
		z *= rhs;
		return *this;
	}

	/**
	 * Division operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector3<T>& operator/=(T rhs)
	{
		x /= rhs;
		y /= rhs;
		z /= rhs;
		return *this;
	}

	//--------------[ Equality operator ]------------------------
	/**
	 * Equality test operator
	 * @param rhs Right hand side argument of binary operator.
	 * @note Test of equality is based of threshold EPSILON value. To be two
	 * values equal, must satisfy this condition | lhs.x - rhs.y | < EPSILON,
	 * same for y-coordinate, and z-coordinate.
	 */
	bool operator==(const Vector3<T>& rhs) const
	{
		return fabsf(x - rhs.x) < EPSILON && fabsf(y - rhs.y) < EPSILON && fabsf(z - rhs.z) < EPSILON;
	}

	/**
	 * Inequality test operator
	 * @param rhs Right hand side argument of binary operator.
	 * @return not (lhs == rhs) :-P
	 */
	bool operator!=(const Vector3<T>& rhs) const
	{
		return !(*this == rhs);
	}

	inline bool operator < (const Vector3<T>& o) const {
		if (x < o.x) return true;
		if (x > o.x) return false;
		if (y < o.y) return true;
		if (y > o.y) return false;
		if (z < o.z) return true;
		return false;
	}

	//-------------[ unary operations ]--------------------------
	/**
	 * Unary negate operator
	 * @return negated vector
	 */
	Vector3<T> operator-() const
	{
		return Vector3<T>(-x, -y, -z);
	}

	//-------------[ size operations ]---------------------------
	/**
	 * Get length of vector.
	 * @return lenght of vector
	 */
	T len() const
	{
		return (T)sqrtf(x * x + y * y + z * z);
	}

	/**
	 * Return square of length.
	 * @return length ^ 2
	 * @note This method is faster then length(). For comparison
	 * of length of two vector can be used just this value, instead
	 * of more expensive length() method.
	 */
	T lenSq() const
	{
		return x * x + y * y + z * z;
	}

	/**
	 * Normalize vector
	 */
	void norm()
	{
		T s = len();
		x /= s;
		y /= s;
		z /= s;
	}

	//------------[ other operations ]---------------------------
	/**
	 * Rotate vector around three axis.
	 * @param ax Angle (in degrees) to be rotated around X-axis.
	 * @param ay Angle (in degrees) to be rotated around Y-axis.
	 * @param az Angle (in degrees) to be rotated around Z-axis.
	 */
	void rot(T ax, T ay, T az)
	{
		T a = cosf(DEG2RAD(ax));
		T b = sinf(DEG2RAD(ax));
		T c = cosf(DEG2RAD(ay));
		T d = sinf(DEG2RAD(ay));
		T e = cosf(DEG2RAD(az));
		T f = sinf(DEG2RAD(az));
		T nx = c * e * x - c * f * y + d * z;
		T ny = (a * f + b * d * e) * x + (a * e - b * d * f) * y - b * c * z;
		T nz = (b * f - a * d * e) * x + (a * d * f + b * e) * y + a * c * z;
		x = nx;
		y = ny;
		z = nz;

	}

	/**
	 * Linear interpolation of two vectors
	 * @param fact Factor of interpolation. For translation from positon
	 * of this vector to vector r, values of factor goes from 0.0 to 1.0.
	 * @param r Second Vector for interpolation
	 * @note However values of fact parameter are reasonable only in interval
	 * [0.0 , 1.0], you can pass also values outside of this interval and you
	 * can get result (extrapolation?)
	 */
	Vector3<T> lerp(T fact, const Vector3<T>& r) const
	{
		return (*this) + (r - (*this)) * fact;
	}

	Vector3<T> floor() const { return Vector3<T>(std::floor(x), std::floor(y), std::floor(z)); }

	Vector3<T> ceil() const { return Vector3<T>(std::ceil(x), std::ceil(y), std::ceil(z)); }

	Vector3<T> max(const Vector3<T>& v) const { return Vector3<T>(std::max(x, v.x), std::max(y, v.y),std::max(z,v.z)); }

	Vector3<T> min(const Vector3<T>& v) const { return Vector3<T>(std::min(x, v.x), std::min(y, v.y), std::min(z,v.z)); }

	float dist(Vector3<T> other) {
		return sqrt(pow(other.x - x, 2) + pow(other.y - y, 2) +	pow(other.z - z, 2) * 1.0);
	}


	//-------------[ conversion ]-----------------------------

	/**
	 * Conversion to pointer operator
	 * @return Pointer to internally stored (in management of class Vector3<T>)
	 * used for passing Vector3<T> values to gl*3[fd] functions.
	 */
	operator T* ()
	{
		return (T*)this;
	}

	/**
	 * Conversion to pointer operator
	 * @return Constant Pointer to internally stored (in management of class Vector3<T>)
	 * used for passing Vector3<T> values to gl*3[fd] functions.
	 */
	operator const T* () const
	{
		return (const T*)this;
	}

	
	/**
	 * Conversion to OneLoneCoder::PGE Pixel operator
	 * @return a Pixel color value containing (x/r,y/g,z/b,w/a)
	 */
	operator Colorf() const { return Colorf(r, g, b, 255); }

	/**
	 * Conversion to dorkestEngine Vector2
	 * @return a Vector2 containing just the x,y
	 */
	//operator Vector2<T> const { return Vector2<T>(x, y); }


	//-------------[ output operator ]------------------------
	/**
	 * Output to stream operator
	 * @param lhs Left hand side argument of operator (commonly ostream instance).
	 * @param rhs Right hand side argument of operator.
	 * @return Left hand side argument - the ostream object passed to operator.
	 */
	friend std::ostream& operator<<(std::ostream& lhs, const Vector3<T> rhs)
	{
		lhs << "[" << rhs.x << "," << rhs.y << "," << rhs.z << "]";
		return lhs;
	}

	/**
	 * Gets string representation.
	 */
	std::string toString() const
	{
		std::ostringstream oss;
		oss << *this;
		return oss.str();
	}
	
	/**
	 * Gets string representation.
	 */
	std::string toStr() const
	{
		std::ostringstream oss;
		oss << *this;
		return oss.str();
	}
};

/// Three dimensional Vector of floats
typedef Vector3<float> Vector3f;
/// Three dimensional Vector of doubles
typedef Vector3<double> Vector3d;
/// Three dimensional Vector of ints
typedef Vector3<int> Vector3i;









