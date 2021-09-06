#pragma once

#include <cmath>
#include <Util/Log.h>

#ifndef EPSILON
const double epsilon = 4.37114e-05;
#define EPSILON epsilon
#endif
#include <cstdarg>

class Colorf
{
public:

	float r, g, b, a;
	





	//----------------[ constructors ]--------------------------
	/**
	 * Creates and sets to (0,0,0,0)
	 */
	Colorf()
		: r(0.0f), g(0.0f), b(0.0f), a(0.0f)
	{
	}


	Colorf(int r, int g, int b, int a) {
		//debugss << " IN : " << r << " " << g << " " << b << " " << a;
		this->r = r / 255.0f;
		this->g = g / 255.0f;
		this->b = b / 255.0f;
		this->a = a / 255.0f;
		//debugss << " OUT : " << this->r << " " << this->g << " " << this->b << " " << this->a;
	}

	Colorf(float r, float g, float b, float a)
	{
		if (r < 0 || r > 1) { this->r = 0; error("Invalid value for red color component - set to zero. Must be between 0.0f and 1.0f."); }
		else this->r = r;
		if (g < 0 || g > 1) { this->g = 0; error("Invalid value for green color component - set to zero. Must be between 0.0f and 1.0f."); }
		else this->g = g;
		if (b < 0 || b > 1) { this->b = 0; error("Invalid value for blue color component - set to zero. Must be between 0.0f and 1.0f."); }
		else this->b = b;
		if (a < 0 || a > 1) { this->a = 0; error("Invalid value for red color component - set to zero. Must be between 0.0f and 1.0f."); }
		else this->a = a;
	}

	/**
	 * Copy constructor.
	 * @param src Source of data for new created Colorf instance.
	 */
	Colorf(const Colorf& src)
		: r(src.r), g(src.g), b(src.b), a(src.a)
	{
	}

	/// <summary>
	/// Initializes to a random color value. DOES NOT call srand.
	/// </summary>
	void setRand() {
		
		r = (float)(rand() % 255) /255.0f;
		g = (float)(rand() % 255) / 255.0f;
		b = (float)(rand() % 255) / 255.0f;
		a = 1.0f;
		debugss << r << ", " << g << ", " << b << ", " << a << ".";
	}

	/// <summary>
	/// Creates dorkestColorf from
	/// </summary>
	/// <param name="color"></param>
	//Colorf(Colorf color) : r(color.r / 255), g(color.g / 255), b(color.b / 255), a(color.a / 255) {}

	operator Colorf() { return Colorf(r * 255, g * 255, b * 255, a * 255); }

	operator std::string() { return toString(); }


	//----------------[ access operators ]-------------------
	/**
	 * Copy operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Colorf operator=(const Colorf& rhs)
	{
		r = rhs.r;
		g = rhs.g;
		b = rhs.b;
		a = rhs.a;
		return *this;
	}

	/**
	 * Array access operator
	 * @param n Array index
	 * @return For n = 0, reference to r coordinate, n = 1
	 * reference to g coordinate, n = 2 reference to b,
	 * else reference to a coordinate.
	 */
	float& operator[](int n)
	{
		assert(n >= 0 && n <= 3);
		if (0 == n)
			return r;
		else if (1 == n)
			return g;
		else if (2 == n)
			return b;
		else
			return a;
	}

	/**
	 * Array access operator
	 * @param n Array index
	 * @return For n = 0, reference to r coordinate, n = 1
	 * reference to g coordinate, n = 2 reference to b,
	 * else reference to a coordinate.
	 */
	const float& operator[](int n) const
	{
		assert(n >= 0 && n <= 3);
		if (0 == n)
			return r;
		else if (1 == n)
			return g;
		else if (2 == n)
			return b;
		else
			return a;
	}

	//---------------[ color aritmetic operator ]--------------

	/// <summary>
	/// Additively mixes 2 colors together.
	/// </summary>
	/// <param name="rhs"></param>
	/// <returns></returns>
	Colorf operator+(const Colorf& rhs) const
	{
		return Colorf(
			std::fminf(r + rhs.r, 1.0f),
			std::fminf(g + rhs.g, 1.0f),
			std::fminf(b + rhs.b, 1.0f),
			std::fminf(a + rhs.a, 1.0f)
		);
	}


	Colorf operator-(const Colorf& rhs) const
	{
		return Colorf(
			std::fmaxf(std::fminf(r - rhs.r, 1.0f),0.0f),
			std::fmaxf(std::fminf(g - rhs.g, 1.0f), 0.0f),
			std::fmaxf(std::fminf(b - rhs.b, 1.0f), 0.0f),
			a);
	}

	Colorf operator*(const Colorf rhs) const
	{
		return Colorf(
			std::fmaxf(std::fminf(r * rhs.r, 1.0f), 0.0f),
			std::fmaxf(std::fminf(g * rhs.g, 1.0f), 0.0f),
			std::fmaxf(std::fminf(b * rhs.b, 1.0f), 0.0f),
			a);
	}


	Colorf operator/(const Colorf& rhs) const
	{
		float r = (this->r / 2) + (rhs.r / 2);
		float g = (this->g / 2) + (rhs.g / 2);
		float b = (this->b / 2) + (rhs.b / 2);
		return Colorf(r, g, b, 1.0f);

/*		return Colorf(
			std::fmaxf(std::fminf(std::fminf(r,rhs.r) / std::fmaxf(r, rhs.r), 1.0f), 0.0f),
			std::fmaxf(std::fminf(std::fminf(g, rhs.g) / std::fmaxf(g, rhs.g), 1.0f), 0.0f),
			std::fmaxf(std::fminf(std::fminf(b, rhs.b) / std::fmaxf(b, rhs.b), 1.0f), 0.0f),
			a);
*/
	}
	/*
	Colorf& operator+=(const Colorf& rhs)
	{
		r += rhs.r;
		g += rhs.g;
		b += rhs.b;
		a += rhs.a;
		return *this;
	}

	Colorf& operator-=(const Colorf& rhs)
	{
		r -= rhs.r;
		g -= rhs.g;
		b -= rhs.b;
		a -= rhs.a;
		return *this;
	}


	Colorf& operator*=(const Colorf& rhs)
	{
		r *= rhs.r;
		g *= rhs.g;
		b *= rhs.b;
		a *= rhs.a;
		return *this;
	}


	Colorf& operator/=(const Colorf& rhs)
	{
		r /= rhs.r;
		g /= rhs.g;
		b /= rhs.b;
		a /= rhs.a;
		return *this;
	}
	*/


	Colorf blendWfront(Colorf front) {
		float na = (1.0 - (1.0 - a) * (1.0 - front.a));
		float nr = (r * (1.0 - front.a) + front.r * front.a);
		float ng = (g * (1.0 - front.a) + front.g * front.a);
		float nb = (b * (1.0 - front.a) + front.b * front.a);
		return Colorf(std::fminf(nr, 1.0f), std::fminf(ng, 1.0f), std::fminf(nb, 1.0f), std::fminf(na, 1.0f));
	}

	Colorf blendWback(Colorf back) {
		float na = (1.0 - (1.0 - back.a) * (1.0 - a));
		float nr = (back.r * (1.0 - a) + r * a);
		float ng = (back.g * (1.0 - a) + g * a);
		float nb = (back.b * (1.0 - a) + b * a);
		return Colorf(std::fminf(nr, 1.0f), std::fminf(ng, 1.0f), std::fminf(nb, 1.0f), std::fminf(na, 1.0f));
	}

	Colorf avg(Colorf a, Colorf b) {
		return (a + b) / 2;
	}

	Colorf avg(int count, ...) {
		va_list args;
		
		va_start(args,count);
		Colorf ret = Colorf(0, 0, 0, 255);

		for (int x = 0; x < count; x++) {
			
			Colorf c = va_arg(args, Colorf);
			
			ret.r += c.r;
			ret.g += c.g;
			ret.b += c.b;
			debugss << "Round " << x << " : " << ret.r << ", " << ret.g << ", " << ret.b << ".";
			
		}
		
		va_end(args);

		ret.r /= count;
		ret.g /= count;
		ret.b /= count;

		return ret;

	}
	Colorf screen(Colorf c) {
		float nr = 1 - ((1 - r) * (1 - c.r));
		float ng = 1 - ((1 - g) * (1 - c.g));
		float nb = 1 - ((1 - b) * (1 - c.b));
		return Colorf(std::fminf(nr, 1.0f), std::fminf(ng, 1.0f), std::fminf(nb, 1.0f), a);
	}

	//--------------[ equiality operator ]------------------------
	/**
	 * Equality test operator
	 * @param rhs Right hand side argument of binary operator.
	 * @note Test of equality is based of threshold EPSILON value. To be two
	 * values equal, must satisfy this condition | lhs.r - rhs.g | < EPSILON,
	 * same for g-coordinate, b-coordinate, and a-coordinate.
	 */
	bool operator==(const Colorf& rhs) const
	{
		return fabsf(r - rhs.r) < EPSILON && fabsf(g - rhs.g) < EPSILON && fabsf(b - rhs.b) < EPSILON
			&& fabsf(a - rhs.a) < EPSILON;
	}

	/**
	 * Inequality test operator
	 * @param rhs Right hand side argument of binary operator.
	 * @return not (lhs == rhs) :-P
	 */
	bool operator!=(const Colorf& rhs) const
	{
		return !(*this == rhs);
	}

	//-------------[ unary operations ]--------------------------
	/**
	 * Unary negate operator
	 * @return negated vector
	 */
	Colorf operator-() const
	{
		return Colorf(1.0f - r, 1.0f - g, 1.0f - b, a);
	}

	//--------------[ scalar vector operator ]--------------------

	/**
	 * Addition operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Colorf operator+(float rhs) const
	{
		return Colorf(r + rhs, g + rhs, b + rhs, a);
	}

	/**
	 * Subtraction operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Colorf operator-(float rhs) const
	{
		return Colorf(r - rhs, g - rhs, b - rhs, a);
	}

	/**
	 * Multiplication operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Colorf operator*(float rhs) const
	{
		if (rhs > 1)
			return Colorf(r, g, b, a);
		else
			return Colorf(r * rhs, g * rhs, b * rhs, a);
	}



	Colorf operator/(float rhs) const
	{
		return Colorf(r / rhs, g / rhs, b / rhs, a );
	}
	/*

	Colorf& operator+=(T rhs)
	{
		r += rhs;
		g += rhs;
		b += rhs;
		a += rhs;
		return *this;
	}


	Colorf& operator-=(T rhs)
	{
		r -= rhs;
		g -= rhs;
		b -= rhs;
		a -= rhs;
		return *this;
	}


	Colorf& operator*=(T rhs)
	{
		r *= rhs;
		g *= rhs;
		b *= rhs;
		a *= rhs;
		return *this;
	}


	Colorf& operator/=(T rhs)
	{
		r /= rhs;
		g /= rhs;
		b /= rhs;
		a /= rhs;
		return *this;
	}
	*/

	/**
	 * Return square of length.
	 * @return length ^ 2
	 * @note This method is faster then length(). For comparison
	 * of length of two vector can be used just this value, instead
	 * of more expensive length() method.
	 */
	float lenSq() const
	{
		return r * r + g * g + b * b + a * a;
	}


	Colorf floor() const { return Colorf(std::floor(r), std::floor(g), std::floor(b), std::floor(a)); }
	Colorf ceil() const { return Colorf(std::ceil(r), std::ceil(g), std::ceil(b), std::ceil(a)); }
	Colorf maxc(const Colorf& v) const { return Colorf(std::max(r, v.r), std::max(g, v.g), std::max(b, v.b), std::max(a, v.a)); }
	Colorf minc(const Colorf& v) const { return Colorf(std::min(r, v.r), std::min(g, v.g), std::min(b, v.b), std::min(a, v.a)); }


	//-------------[ conversion ]-----------------------------

	void Print(void)
	{
		printf("(%1.5f,%1.5f,%1.5f)", r, g, b);
	}

	/**
	 * Conversion to pointer operator
	 * @return Pointer to internally stored (in management of class Colorf)
	 * used for passing Colorf values to gl*4[fd] functions.
	 */
	operator float* ()
	{
		return (float*)this;
	}

	/**
	 * Conversion to pointer operator
	 * @return Constant Pointer to internally stored (in management of class Colorf)
	 * used for passing Colorf values to gl*4[fd] functions.
	 */
	operator const float* () const
	{
		return (const float*)this;
	}

	//-------------[ output operator ]------------------------
	/**
	 * Output to stream operator
	 * @param lhs Left hand side argument of operator (commonly ostream instance).
	 * @param rhs Right hand side argument of operator.
	 * @return Left hand side argument - the ostream object passed to operator.
	 */
	friend std::ostream& operator<<(std::ostream& lhs, const Colorf& rhs)
	{
		lhs << "[" << rhs.r << "," << rhs.g << "," << rhs.b << "," << rhs.a << "]";
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
