#pragma once
#include <cmath>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <cassert>
#include <cstdio>

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
 * Class for four dimensional vector.
  * There are four ways of accessing vector components.
 * Let's have <code>Vector4f v</code>, you can either:
 * <ul>
 * 	<li>access as position in projective space (x,y,z,w) &mdash; <code>v.x = v.y = v.z = v.w = 1;</code></li>
 * 	<li>access as texture coordinate (s,t,u,v) &mdash; <code>v.s = v.t = v.u = v.v = 1;</code></li>
 * 	<li>access as color (r,g,b,a) &mdash; <code>v.r = v.g = v.b = v.a = 1;</code></li>
 * 	<li>access via operator[] &mdash; <code>v[0] = v[1] = v[2] = v[3] = 1;</code></li>
 * </ul>
 */
template<class T>
class Vector4
{
public:

	union
	{
		/**
		 * First element of vector, alias for R-coordinate.
		 * For color notation.
		 */
		T r
			/**
			 * First element of vector, alias for X-coordinate.
			 */;
		T x;
	};

	union
	{
		/**
		 * Second element of vector, alias for G-coordinate.
		 * For color notation.
		 */
		T g;
		/**
		 * Second element of vector, alias for Y-coordinate.
		 */
		T y;
	};

	union
	{
		/**
		 * Third element of vector, alias for B-coordinate.
		 * For color notation.
		 */
		T b;
		/**
		 * Third element of vector, alias for Z-coordinate.
		 */
		T z;
	};

	union
	{
		/**
		 * Fourth element of vector, alias for A-coordinate.
		 * For color notation. This represnt aplha chanell
		 */
		T a;
		/**
		 * First element of vector, alias for W-coordinate.
		 * @note For vectors (such as normals) should be set to 0.0
		 * For vertices should be set to 1.0
		 */
		T w;
	};

	//----------------[ constructors ]--------------------------
	/**
	 * Creates and sets to (0,0,0,0)
	 */
	Vector4()
		: x(0), y(0), z(0), w(0)
	{
	}

	/**
	 * Creates and sets to (x,y,z,z)
	 * @param nx initial x-coordinate value (R)
	 * @param ny initial y-coordinate value (G)
	 * @param nz initial z-coordinate value (B)
	 * @param nw initial w-coordinate value (Alpha)
	 */
	Vector4(T nx, T ny, T nz, T nw)
		: x(nx), y(ny), z(nz), w(nw)
	{
	}

	/**
	 * Copy constructor.
	 * @param src Source of data for new created Vector4 instance.
	 */
	Vector4(const Vector4<T>& src)
		: x(src.x), y(src.y), z(src.z), w(src.w)
	{
	}

	/**
	 * Copy casting constructor.
	 * @param src Source of data for new created Vector4 instance.
	 */
	template<class FromT>
	Vector4(const Vector4<FromT>& src)
		: x(static_cast<T>(src.x)), y(static_cast<T>(src.y)), z(static_cast<T>(src.z)), w(static_cast<T>(src.w))
	{
	}

	Vector4(olc::Pixel color) : r(color.r), g(color.g), b(color.b), a(color.a) {}
	//std::string& operator() { return toString(); }


	//----------------[ access operators ]-------------------
	/**
	 * Copy operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector4<T> operator=(const Vector4<T>& rhs)
	{
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		w = rhs.w;
		return *this;
	}

	/**
	 * Copy casting operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	template<class FromT>
	Vector4<T> operator=(const Vector4<FromT>& rhs)
	{
		x = static_cast<T>(rhs.x);
		y = static_cast<T>(rhs.y);
		z = static_cast<T>(rhs.z);
		w = static_cast<T>(rhs.w);
		return *this;
	}

	/**
	 * Array access operator
	 * @param n Array index
	 * @return For n = 0, reference to x coordinate, n = 1
	 * reference to y coordinate, n = 2 reference to z,
	 * else reference to w coordinate.
	 */
	T& operator[](int n)
	{
		assert(n >= 0 && n <= 3);
		if (0 == n)
			return x;
		else if (1 == n)
			return y;
		else if (2 == n)
			return z;
		else
			return w;
	}

	/**
	 * Array access operator
	 * @param n Array index
	 * @return For n = 0, reference to x coordinate, n = 1
	 * reference to y coordinate, n = 2 reference to z,
	 * else reference to w coordinate.
	 */
	const T& operator[](int n) const
	{
		assert(n >= 0 && n <= 3);
		if (0 == n)
			return x;
		else if (1 == n)
			return y;
		else if (2 == n)
			return z;
		else
			return w;
	}

	//---------------[ vector aritmetic operator ]--------------
	/**
	 * Addition operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector4<T> operator+(const Vector4<T>& rhs) const
	{
		return Vector4<T>(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
	}

	/**
	 * Subtraction operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector4<T> operator-(const Vector4<T>& rhs) const
	{
		return Vector4<T>(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
	}

	/**
	 * Multiplication operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector4<T> operator*(const Vector4<T> rhs) const
	{
		return Vector4<T>(x * rhs.x, y * rhs.y, z * rhs.z, w * rhs.w);
	}

	/**
	 * Division operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector4<T> operator/(const Vector4<T>& rhs) const
	{
		return Vector4<T>(x / rhs.x, y / rhs.y, z / rhs.z, w / rhs.w);
	}

	/**
	 * Addition operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector4<T>& operator+=(const Vector4<T>& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		w += rhs.w;
		return *this;
	}

	/**
	 * Subtraction operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector4<T>& operator-=(const Vector4<T>& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		w -= rhs.w;
		return *this;
	}

	/**
	 * Multiplication operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector4<T>& operator*=(const Vector4<T>& rhs)
	{
		x *= rhs.x;
		y *= rhs.y;
		z *= rhs.z;
		w *= rhs.w;
		return *this;
	}

	/**
	 * Division operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector4<T>& operator/=(const Vector4<T>& rhs)
	{
		x /= rhs.x;
		y /= rhs.y;
		z /= rhs.z;
		w /= rhs.w;
		return *this;
	}

	//--------------[ equiality operator ]------------------------
	/**
	 * Equality test operator
	 * @param rhs Right hand side argument of binary operator.
	 * @note Test of equality is based of threshold EPSILON value. To be two
	 * values equal, must satisfy this condition | lhs.x - rhs.y | < EPSILON,
	 * same for y-coordinate, z-coordinate, and w-coordinate.
	 */
	bool operator==(const Vector4<T>& rhs) const
	{
		return fabsf(x - rhs.x) < EPSILON && fabsf(y - rhs.y) < EPSILON && fabsf(z - rhs.z) < EPSILON
			&& fabsf(w - rhs.w) < EPSILON;
	}

	/**
	 * Inequality test operator
	 * @param rhs Right hand side argument of binary operator.
	 * @return not (lhs == rhs) :-P
	 */
	bool operator!=(const Vector4<T>& rhs) const
	{
		return !(*this == rhs);
	}

	//-------------[ unary operations ]--------------------------
	/**
	 * Unary negate operator
	 * @return negated vector
	 */
	Vector4<T> operator-() const
	{
		return Vector4<T>(-x, -y, -z, -w);
	}

	//--------------[ scalar vector operator ]--------------------

	/**
	 * Addition operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector4<T> operator+(T rhs) const
	{
		return Vector4<T>(x + rhs, y + rhs, z + rhs, w + rhs);
	}

	/**
	 * Subtraction operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector4<T> operator-(T rhs) const
	{
		return Vector4<T>(x - rhs, y - rhs, z - rhs, w - rhs);
	}

	/**
	 * Multiplication operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector4<T> operator*(T rhs) const
	{
		return Vector4<T>(x * rhs, y * rhs, z * rhs, w * rhs);
	}

	/**
	 * Division operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector4<T> operator/(T rhs) const
	{
		return Vector4<T>(x / rhs, y / rhs, z / rhs, w / rhs);
	}

	/**
	 * Addition operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector4<T>& operator+=(T rhs)
	{
		x += rhs;
		y += rhs;
		z += rhs;
		w += rhs;
		return *this;
	}

	/**
	 * Subtraction operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector4<T>& operator-=(T rhs)
	{
		x -= rhs;
		y -= rhs;
		z -= rhs;
		w -= rhs;
		return *this;
	}

	/**
	 * Multiplication operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector4<T>& operator*=(T rhs)
	{
		x *= rhs;
		y *= rhs;
		z *= rhs;
		w *= rhs;
		return *this;
	}

	/**
	 * Division operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector4<T>& operator/=(T rhs)
	{
		x /= rhs;
		y /= rhs;
		z /= rhs;
		w /= rhs;
		return *this;
	}

	//-------------[ size operations ]---------------------------
	/**
	 * Get length of vector.
	 * @return lenght of vector
	 */
	T len() const
	{
		return (T)std::sqrt(float(x * x + y * y + z * z + w * w));
	}

	/**
	 * Normalize vector
	 */
	void normalize()
	{
		T s = len();
		x /= s;
		y /= s;
		z /= s;
		w /= s;
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
		return x * x + y * y + z * z + w * w;
	}

	//--------------[ misc. operations ]-----------------------
	/**
	 * Linear interpolation of two vectors
	 * @param fact Factor of interpolation. For translation from position
	 * of this vector to vector r, values of factor goes from 0.0 to 1.0.
	 * @param r Second Vector for interpolation
	 * @note However values of fact parameter are reasonable only in interval
	 * [0.0 , 1.0], you can pass also values outside of this interval and you
	 * can get result (extrapolation?)
	 */
	Vector4<T> lerp(T fact, const Vector4<T>& r) const
	{
		return (*this) + (r - (*this)) * fact;
	}

	Vector4<T> floor() const { return Vector4<T>(std::floor(x), std::floor(y), std::floor(z), std::floor(w)); }
	Vector4<T> ceil() const { return Vector4<T>(std::ceil(x), std::ceil(y), std::ceil(z), std::ceil(w)); }
	Vector4<T> max(const Vector4<T>& v) const { return Vector4<T>(std::max(x, v.x), std::max(y, v.y), std::max(z, v.z), std::max(w, v.w)); }
	Vector4<T> min(const Vector4<T>& v) const { return Vector4<T>(std::min(x, v.x), std::min(y, v.y), std::min(z, v.z), std::min(w, v.w)); }


	//-------------[ conversion ]-----------------------------

	void Print(void)
	{
		printf("(%5.5f,%5.5f,%5.5f)", x, y, z);
	}

	/**
	 * Conversion to pointer operator
	 * @return Pointer to internally stored (in management of class Vector4<T>)
	 * used for passing Vector4<T> values to gl*4[fd] functions.
	 */
	operator T* ()
	{
		return (T*)this;
	}

	/**
	 * Conversion to pointer operator
	 * @return Constant Pointer to internally stored (in management of class Vector4<T>)
	 * used for passing Vector4<T> values to gl*4[fd] functions.
	 */
	operator const T* () const
	{
		return (const T*)this;
	}

	//-------------[ output operator ]------------------------
	/**
	 * Output to stream operator
	 * @param lhs Left hand side argument of operator (commonly ostream instance).
	 * @param rhs Right hand side argument of operator.
	 * @return Left hand side argument - the ostream object passed to operator.
	 */
	friend std::ostream& operator<<(std::ostream& lhs, const Vector4<T>& rhs)
	{
		lhs << "[" << rhs.x << "," << rhs.y << "," << rhs.z << "," << rhs.w << "]";
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
typedef Vector4<float> Vector4f;
/// Three dimensional Vector of doubles
typedef Vector4<double> Vector4d;
/// Three dimensional Vector of ints
typedef Vector4<int> Vector4i;