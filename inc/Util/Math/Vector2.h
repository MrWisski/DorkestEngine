#pragma once

//Disables Conversion from 'T' to 'xxx' possible loss of data warnings.
#pragma warning( disable : 4244 ) 

#include <cmath>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <cassert>
#include <cstdio>
#include <olcPixelGameEngine.h>

#include "Vector3.h"

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
 * Class for two dimensional vector.
 * There are three ways of accessing vector components.
 * Let's have <code>Vector2f v</code>, you can either:
 * <ul>
 * 	<li>access as position(x,y) &mdash; <code>v.x = v.y = 3;</code></li>
 * 	<li>access as texture coordinate (s,t) &mdash; <code>v.s = v.t = 3;</code></li>
 * 	<li>access via operator[] &mdash; <code>v[0] = v[1] = 3;</code></li>
 * </ul>
 */
template<class T>
class Vector2
{
public:
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
	};

	//----------------[ constructors ]--------------------------
	/**
	 * Creates and sets to (0,0)
	 */
	Vector2()
			: x(0), y(0)
	{
	}

	/**
	 * Creates and sets to (x,y)
	 * @param nx initial x-coordinate value
	 * @param ny initial y-coordinate value
	 */
	Vector2(T nx, T ny)
			: x(nx), y(ny)
	{
	}

	/**
	 * Creates and sets to an olc::vi2d
	 */
	Vector2(olc::vi2d n) : x(static_cast<T>(n.x)), y(static_cast<T>(n.y)) {}

	/**
	 * Creates and sets to an olc::vf2d
	 */
	Vector2(olc::vf2d n) : x(static_cast<T>(n.x)), y(static_cast<T>(n.y)) {}
	/**
	 * Creates and sets to an olc::vd2d
	 */
	Vector2(olc::vd2d n) : x(static_cast<T>(n.x)), y(static_cast<T>(n.y)){}


	/**
	 * Copy constructor.
	 * @param src Source of data for new created instance.
	 */
	Vector2(const Vector2<T>& src)
			: x(src.x), y(src.y)
	{
	}

	/**
	 * Copy casting constructor.
	 * @param src Source of data for new created instance.
	 */
	template<class FromT>
	Vector2(const Vector2<FromT>& src)
			: x(static_cast<T>(src.x)), y(static_cast<T>(src.y))
	{
	}

	//----------------[ access operators ]-------------------
	/**
	 * Copy casting operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	template<class FromT>
	Vector2<T>& operator=(const Vector2<FromT>& rhs)
	{
		x = static_cast<T>(rhs.x);
		y = static_cast<T>(rhs.y);
		return *this;
	}

	/**
	 * Copy operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector2<T>& operator=(const Vector2<T>& rhs)
	{
		x = rhs.x;
		y = rhs.y;
		return *this;
	}

	/**
	 * Array access operator
	 * @param n Array index
	 * @return For n = 0, reference to x coordinate, else reference to y 
	 * y coordinate.
	 */
	T& operator[](int n)
	{
		assert(n >= 0 && n <= 1);
		if (0 == n)
			return x;
		else
			return y;
	}

	/**
	 * Constant array access operator
	 * @param n Array index
	 * @return For n = 0, reference to x coordinate, else reference to y
	 * y coordinate.
	 */
	const T& operator[](int n) const
	{
		assert(n >= 0 && n <= 1);
		if (0 == n)
			return x;
		else
			return y;
	}

	//---------------[ vector aritmetic operator ]--------------
	/**
	 * Addition operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector2<T> operator+(const Vector2<T>& rhs) const
	{
		return Vector2<T>(x + rhs.x, y + rhs.y);
	}

	/**
	 * Subtraction operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector2<T> operator-(const Vector2<T>& rhs) const
	{
		return Vector2<T>(x - rhs.x, y - rhs.y);
	}

	/**
	 * Multiplication operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector2<T> operator*(const Vector2<T>& rhs) const
	{
		return Vector2<T>(x * rhs.x, y * rhs.y);
	}

	/**
	 * Division operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector2<T> operator/(const Vector2<T>& rhs) const
	{
		return Vector2<T>(x / rhs.x, y / rhs.y);
	}

	/**
	 * Addition operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector2<T>& operator+=(const Vector2<T>& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		return *this;
	}

	/**
	 * Substraction operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector2<T>& operator-=(const Vector2<T>& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}

	/**
	 * Multiplication operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector2<T>& operator*=(const Vector2<T>& rhs)
	{
		x *= rhs.x;
		y *= rhs.y;
		return *this;
	}

	/**
	 * Division operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector2<T>& operator/=(const Vector2<T>& rhs)
	{
		x /= rhs.x;
		y /= rhs.y;
		return *this;
	}

	Vector2<T> floor() const { return Vector2<T>(std::floor(x), std::floor(y)); }
	Vector2<T> ceil() const { return Vector2<T>(std::ceil(x), std::ceil(y)); }
	Vector2<T> max(const Vector2<T>& v) const { return Vector2<T>(std::max(x, v.x), std::max(y, v.y)); }
	Vector2<T> min(const Vector2<T>& v) const { return Vector2<T>(std::min(x, v.x), std::min(y, v.y)); }

	//--------------[ scalar vector operator ]--------------------
	/**
	 * Addition operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector2<T> operator+(T rhs) const
	{
		return Vector2<T>(x + rhs, y + rhs);
	}

	/**
	 * Subtraction operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector2<T> operator-(T rhs) const
	{
		return Vector2<T>(x - rhs, y - rhs);
	}

	/**
	 * Multiplication operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector2<T> operator*(T rhs) const
	{
		return Vector2<T>(x * rhs, y * rhs);
	}

	/**
	 * Division operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector2<T> operator/(T rhs) const
	{
		return Vector2<T>(x / rhs, y / rhs);
	}

	/**
	 * Addition operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector2<T>& operator+=(T rhs)
	{
		x += rhs;
		y += rhs;
		return *this;
	}

	/**
	 * Subtraction operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector2<T>& operator-=(T rhs)
	{
		x -= rhs;
		y -= rhs;
		return *this;
	}

	/**
	 * Multiplication operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector2<T>& operator*=(T rhs)
	{
		x *= rhs;
		y *= rhs;
		return *this;
	}

	/**
	 * Division operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector2<T>& operator/=(T rhs)
	{
		x /= rhs;
		y /= rhs;
		return *this;
	}

	//--------------[ equality operator ]------------------------
	/**
	 * Equality test operator
	 * @param rhs Right hand side argument of binary operator.
	 * @note Test of equality is based of threshold EPSILON value. To be two
	 * values equal, must satisfy this condition | lhs.x - rhs.y | < EPSILON,
	 * same for y-coordinate.
	 */
	bool operator==(const Vector2<T>& rhs) const
	{
		return (std::abs(x - rhs.x) < EPSILON) && (std::abs(y - rhs.y) < EPSILON);
	}

	/**
	 * Inequality test operator
	 * @param rhs Right hand side argument of binary operator.
	 * @return not (lhs == rhs) :-P
	 */
	bool operator!=(const Vector2<T>& rhs) const
	{
		return !(*this == rhs);
	}

	//-------------[ unary operations ]--------------------------
	/**
	 * Unary negate operator
	 * @return negated vector
	 */
	Vector2<T> operator-() const
	{
		return Vector2<T>(-x, -y);
	}

	//-------------[ size operations ]---------------------------
	/**
	 * Get length of vector.
	 * @return lenght of vector
	 */
	T len() const
	{
		return (T) std::sqrt(float(x * x + y * y));
	}

	/**
	 * Normalize vector
	 */
	void norm()
	{
		T s = len();
		x /= s;
		y /= s;
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
		return x * x + y * y;
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
	Vector2<T> lerp(T fact, const Vector2<T>& r) const
	{
		return (*this) + (r - (*this)) * fact;
	}

	//-------------[ conversion ]-----------------------------
	/**
	 * Conversion to pointer operator
	 * @return Pointer to internally stored (in management of class Vector2<T>)
	 * used for passing Vector2<T> values to gl*2[fd] functions.
	 */
	operator T*()
	{
		return (T*) this;
	}
	/**
	 * Conversion to pointer operator
	 * @return Constant Pointer to internally stored (in management of class Vector2<T>)
	 * used for passing Vector2<T> values to gl*2[fd] functions.
	 */
	operator const T*() const
	{
		return (const T*) this;
	}

	/**
	 * Conversion to OneLoneCoder::PGE Vector2 operator
	 * @return a vi2d containing (x,y)
	 */
	operator olc::vi2d() const { return olc::vi2d(x, y); }

	/**
	 * Conversion to OneLoneCoder::PGE Vector2 operator
	 * @return a vi2d containing (x,y)
	 */
	operator olc::vf2d() const { return olc::vf2d(x, y); }

	/**
	 * Conversion to OneLoneCoder::PGE Vector2 operator
	 * @return a vi2d containing (x,y)
	 */
	operator olc::vd2d() const { return olc::vd2d(x, y); }

	/**
	 * Conversion to dorkestEngine Vector3
	 * @return a Vector3 containing x,y,0
	 */
	//operator Vector3<T>() const { return Vector3<T>(x, y, 0); }


	//-------------[ output operator ]------------------------
	/**
	 * Output to stream operator
	 * @param lhs Left hand side argument of operator (commonly ostream instance). 
	 * @param rhs Right hand side argument of operator.
	 * @return Left hand side argument - the ostream object passed to operator.
	 */
	friend std::ostream& operator<<(std::ostream& lhs, const Vector2<T>& rhs)
	{
		lhs << "[" << std::to_string(rhs.x) << "," << std::to_string(rhs.y) << "]";
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

//--------------------------------------
// Typedef shortcuts for 2D vector
//-------------------------------------
/// Two dimensional Vector of floats
typedef class Vector2<float> Vector2f;
/// Two dimensional Vector of doubles
typedef class Vector2<double> Vector2d;
/// Two dimensional Vector of ints
typedef class Vector2<int> Vector2i;