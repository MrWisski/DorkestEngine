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
 * Class for matrix 3x3.
 * @note Data stored in this matrix are in column major order. This arrangement suits OpenGL.
 * If you're using row major matrix, consider using fromRowMajorArray as way for construction
 * Matrix3<T> instance.
 */
template<class T>
class Matrix3
{
public:
	/// Data stored in column major order
	T data[9];

	//--------------------------[ constructors ]-------------------------------
	/**
	 * Creates identity matrix
	 */
	Matrix3()
	{
		for (int i = 0; i < 9; i++)
			data[i] = (i % 4) ? 0 : 1;
	}

	/**
	 * Copy matrix values from array (these data must be in column
	 * major order!)
	 */
	Matrix3(const T* dt)
	{
		std::memcpy(data, dt, sizeof(T) * 9);
	}

	/**
	 * Copy constructor.
	 * @param src Data source for new created instance of Matrix3
	 */
	Matrix3(const Matrix3<T>& src)
	{
		std::memcpy(data, src.data, sizeof(T) * 9);
	}

	/**
	 * Copy casting constructor.
	 * @param src Data source for new created instance of Matrix3
	 */
	template<class FromT>
	Matrix3(const Matrix3<FromT>& src)
	{
		for (int i = 0; i < 9; i++)
		{
			data[i] = static_cast<T>(src.data[i]);
		}
	}

	void unitMatrix()
	{
		for (int r = 0; r < 4; r++)
		{
			for (int c = 0; c < 4; c++)
			{
				at(r,c) = (r == c);
			}
		}
	}

	void orthogonal(double xmin, double xmax, double ymin, double ymax, double zmin, double zmax)
	{
		unitMatrix();

		at(0,0) = 2.0 / (xmax - xmin);
		at(1,1) = 2.0 / (ymax - ymin);
		at(2,2) = 2.0 / (zmax - zmin);

		at(0,3) = -(xmax + xmin) / (xmax - xmin);
		at(1,3) = -(ymax + ymin) / (ymax - ymin);
		at(2,3) = -(zmax + zmin) / (zmax - zmin);
	}


	void perspective(double fov, double aspect, double nearset, double farset)
	{
		unitMatrix();

		double t = tan(fov / 2.0);

		at(0,0) = 1.0 / (aspect * t);
		at(1,1) = 1.0 / t;
		at(2,2) = (farset + nearset) / (farset - nearset);
		at(2,3) = -(2 * farset * nearset) / (farset - nearset);

		at(3,2) = 1;
		at(3,3) = 0;
	}


	void screen(int width, int height)
	{
		unitMatrix();

		at(0,0) = static_cast<double>(width) / 2.0;
		at(0,3) = at(0,0);

		at(1,1) = -static_cast<double>(height) / 2.0;
		at(1,3) = -at(1,1);

		at(2,2) = 0.5;
		at(2,3) = 0.5;
	}


	void buildMatrixFromVectors(Vector3<T>& r0, Vector3<T>& r1, Vector3<T>& r2)
	{
		this->unitMatrix();

		at(0,0) = r0.x;
		at(0,1) = r0.y;
		at(0,2) = r0.z;

		at(1,0) = r1.x;
		at(1,1) = r1.y;
		at(1,2) = r1.z;

		at(2,0) = r2.x;
		at(2,1) = r2.y;
		at(2,2) = r2.z;
	}

	/**
	 * Resets matrix to be identity matrix
	 */
	void identity()
	{
		for (int i = 0; i < 9; i++)
			data[i] = (i % 4) ? 0 : 1;
	}

	/**
	 * Creates rotation matrix by rotation around axis.
	 * @param xDeg Angle (in degrees) of rotation around axis X.
	 * @param yDeg Angle (in degrees) of rotation around axis Y.
	 * @param zDeg Angle (in degrees) of rotation around axis Z.
	 */
	static Matrix3<T> createRotationAroundAxis(T xDeg, T yDeg, T zDeg)
	{
		T xRads(DEG2RAD(xDeg));
		T yRads(DEG2RAD(yDeg));
		T zRads(DEG2RAD(zDeg));

		Matrix3<T> ma, mb, mc;
		float ac = cosf(xRads);
		float as = sinf(xRads);
		float bc = cosf(yRads);
		float bs = sinf(yRads);
		float cc = cosf(zRads);
		float cs = sinf(zRads);

		ma.at(1, 1) = ac;
		ma.at(2, 1) = as;
		ma.at(1, 2) = -as;
		ma.at(2, 2) = ac;

		mb.at(0, 0) = bc;
		mb.at(2, 0) = -bs;
		mb.at(0, 2) = bs;
		mb.at(2, 2) = bc;

		mc.at(0, 0) = cc;
		mc.at(1, 0) = cs;
		mc.at(0, 1) = -cs;
		mc.at(1, 1) = cc;

		Matrix3<T> ret = ma * mb * mc;
		return ret;
	}

	/**
	 * Creates rotation matrix from ODE Matrix.
	 */
	template<class It>
	static Matrix3<T> fromOde(const It* mat)
	{
		Matrix3<T> ret;
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				ret.at(i, j) = static_cast<T>(mat[j * 4 + i]);
			}
		}
		return ret;
	}

	/**
	 * Creates new matrix 3x3 from array that represents such matrix 3x3
	 * as array of tightly packed elements in row major order.
	 * @param arr An array of elements for 3x3 matrix in row major order.
	 * @return An instance of Matrix3<T> representing @a arr
	 */
	template<class FromT>
	static Matrix3<T> fromRowMajorArray(const FromT* arr)
	{
		const T retData[] =
		{ static_cast<T>(arr[0]), static_cast<T>(arr[3]), static_cast<T>(arr[6]), static_cast<T>(arr[1]),
				static_cast<T>(arr[4]), static_cast<T>(arr[7]), static_cast<T>(arr[2]), static_cast<T>(arr[5]),
				static_cast<T>(arr[8]) };

		return retData;
	}

	/**
	 * Creates new matrix 3x3 from array that represents such matrix 3x3
	 * as array of tightly packed elements in column major order.
	 * @param arr An array of elements for 3x3 matrix in column major order.
	 * @return An instance of Matrix3<T> representing @a arr
	 */
	template<class FromT>
	static Matrix3<T> fromColumnMajorArray(const FromT* arr)
	{
		const T retData[] =
		{ static_cast<T>(arr[0]), static_cast<T>(arr[1]), static_cast<T>(arr[2]), static_cast<T>(arr[3]),
				static_cast<T>(arr[4]), static_cast<T>(arr[5]), static_cast<T>(arr[6]), static_cast<T>(arr[7]),
				static_cast<T>(arr[8]) };

		return retData;
	}

	//---------------------[ equiality operators ]------------------------------
	/**
	 * Equality test operator
	 * @param rhs Right hand side argument of binary operator.
	 * @note Test of equality is based of threshold EPSILON value. To be two
	 * values equal, must satisfy this condition all elements of matrix
	 * | lhs[i] - rhs[i] | < EPSILON,
	 * same for y-coordinate, z-coordinate, and w-coordinate.
	 */
	bool operator==(const Matrix3<T>& rhs) const
	{
		for (int i = 0; i < 9; i++)
		{
			if (fabsf(data[i] - rhs.data[i]) >= EPSILON)
				return false;
		}
		return true;
	}

	/**
	 * Inequality test operator
	 * @param rhs Right hand side argument of binary operator.
	 * @return not (lhs == rhs) :-P
	 */
	bool operator!=(const Matrix3<T>& rhs) const
	{
		return !(*this == rhs);
	}

	//---------------------[ access operators ]---------------------------------
	/**
	 * Get reference to element at position (x,y).
	 * @param x Number of column (0..2)
	 * @param y Number of row (0..2)
	 */
	T& at(int x, int y)
	{
		assert(x >= 0 && x < 3);
		assert(y >= 0 && y < 3);
		return data[x * 3 + y];
	}

	/**
	 * Get constant reference to element at position (x,y).
	 * @param x Number of column (0..2)
	 * @param y Number of row (0..2)
	 */
	const T& at(int x, int y) const
	{
		assert(x >= 0 && x < 3);
		assert(y >= 0 && y < 3);
		return data[x * 3 + y];
	}

	/**
	 * Get reference to element at position (i,j), with math matrix notation
	 * @param i Number of row (1..3)
	 * @param j Number of column (1..3)
	 */
	T& operator()(int i, int j)
	{
		assert(i >= 1 && i <= 3);
		assert(j >= 1 && j <= 3);
		return data[(j - 1) * 3 + i - 1];
	}

	/**
	 * Get constant reference to element at position (i,j), with math matrix notation
	 * @param i Number of row (1..3)
	 * @param j Number of column (1..3)
	 */
	const T& operator()(int i, int j) const
	{
		assert(i >= 1 && i <= 3);
		assert(j >= 1 && j <= 3);
		return data[(j - 1) * 3 + i - 1];
	}

	/**
	 * Copy operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Matrix3<T>& operator=(const Matrix3<T>& rhs)
	{
		std::memcpy(data, rhs.data, sizeof(T) * 9);
		return *this;
	}

	/**
	 * Copy casting operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	template<class FromT>
	Matrix3<T>& operator=(const Matrix3<FromT>& rhs)
	{
		for (int i = 0; i < 9; i++)
		{
			data[i] = static_cast<T>(rhs.data[i]);
		}
		return *this;
	}

	/**
	 * Copy operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Matrix3<T>& operator=(const T* rhs)
	{
		std::memcpy(data, rhs, sizeof(T) * 9);
		return *this;
	}

	/*Matrix3<T> & operator=(const double* m)
	 {
	 for (int i = 0; i < 9; i++) data[i] = (T)m[i];
	 return * this;
	 }*/

	 //--------------------[ matrix with matrix operations ]---------------------
	 /**
	  * Addition operator
	  * @param rhs Right hand side argument of binary operator.
	  */
	Matrix3<T> operator+(const Matrix3<T>& rhs) const
	{
		Matrix3<T> ret;
		for (int i = 0; i < 9; i++)
			ret.data[i] = data[i] + rhs.data[i];
		return ret;
	}

	/**
	 * Subtraction operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Matrix3<T> operator-(const Matrix3<T>& rhs) const
	{
		Matrix3<T> ret;
		for (int i = 0; i < 9; i++)
			ret.data[i] = data[i] - rhs.data[i];
		return ret;
	}

	//--------------------[ matrix with scalar operations ]---------------------
	/**
	 * Addition operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Matrix3<T> operator+(T rhs) const
	{
		Matrix3<T> ret;
		for (int i = 0; i < 9; i++)
			ret.data[i] = data[i] + rhs;
		return ret;
	}

	/**
	 * Subtraction operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Matrix3<T> operator-(T rhs) const
	{
		Matrix3<T> ret;
		for (int i = 0; i < 9; i++)
			ret.data[i] = data[i] - rhs;
		return ret;
	}

	/**
	 * Multiplication operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Matrix3<T> operator*(T rhs) const
	{
		Matrix3<T> ret;
		for (int i = 0; i < 9; i++)
			ret.data[i] = data[i] * rhs;
		return ret;
	}

	/**
	 * Division operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Matrix3<T> operator/(T rhs) const
	{
		Matrix3<T> ret;
		for (int i = 0; i < 9; i++)
			ret.data[i] = data[i] / rhs;
		return ret;
	}

	//--------------------[ multiply operators ]--------------------------------
	/**
	 * Multiplication operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector3<T> operator*(const Vector3<T>& rhs) const
	{
		return Vector3<T>(data[0] * rhs.x + data[3] * rhs.y + data[6] * rhs.z,
			data[1] * rhs.x + data[4] * rhs.y + data[7] * rhs.z,
			data[2] * rhs.x + data[5] * rhs.y + data[8] * rhs.z);
	}

	/**
	 * Multiplication operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Matrix3<T> operator*(Matrix3<T> rhs) const
	{
		static Matrix3<T> w;
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				T n = 0;
				for (int k = 0; k < 3; k++)
					n += rhs.at(i, k) * at(k, j);
				w.at(i, j) = n;
			}
		}
		return w;

	}

	//---------------------------[ misc operations ]----------------------------
	/**
	 * Transpose matrix.
	 */
	Matrix3<T> transpose()
	{
		Matrix3<T> ret;
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				ret.at(i, j) = at(j, i);
			}
		}
		return ret;
	}

	/**
	 * Linear interpolation of two matrices
	 * @param fact Factor of interpolation. For translation from positon
	 * of this matrix (lhs) to matrix rhs, values of factor goes from 0.0 to 1.0.
	 * @param rhs Second Matrix for interpolation
	 * @note However values of fact parameter are reasonable only in interval
	 * [0.0 , 1.0], you can pass also values outside of this interval and you
	 * can get result (extrapolation?)
	 */
	Matrix3<T> lerp(T fact, const Matrix3<T>& rhs) const
	{
		Matrix3<T> ret = (*this) + (rhs - (*this)) * fact;
		return ret;
	}

	T det()
	{
		return +at(0, 0) * at(1, 1) * at(2, 2) + at(0, 1) * at(1, 2) * at(2, 0) + at(0, 2) * at(1, 0) * at(2, 1)
			- at(0, 0) * at(1, 2) * at(2, 1) - at(0, 1) * at(1, 0) * at(2, 2) - at(0, 2) * at(1, 1) * at(2, 0);
	}

	/**
	 * Computes inverse matrix
	 * @return Inverse matrix of this matrix.
	 */
	Matrix3<T> inverse()
	{
		Matrix3<T> ret;
		ret.at(0, 0) = at(1, 1) * at(2, 2) - at(2, 1) * at(1, 2);
		ret.at(0, 1) = at(2, 1) * at(0, 2) - at(0, 1) * at(2, 2);
		ret.at(0, 2) = at(0, 1) * at(1, 2) - at(1, 1) * at(0, 2);
		ret.at(1, 0) = at(2, 0) * at(1, 2) - at(1, 0) * at(2, 2);
		ret.at(1, 1) = at(0, 0) * at(2, 2) - at(2, 0) * at(0, 2);
		ret.at(1, 2) = at(1, 0) * at(0, 2) - at(0, 0) * at(1, 2);
		ret.at(2, 0) = at(1, 0) * at(2, 1) - at(2, 0) * at(1, 1);
		ret.at(2, 1) = at(2, 0) * at(0, 1) - at(0, 0) * at(2, 1);
		ret.at(2, 2) = at(0, 0) * at(1, 1) - at(1, 0) * at(0, 1);
		return ret * (1.0f / det());
	}

	//-------------[ conversion ]-----------------------------

	/**
	 * Conversion to pointer operator
	 * @return Pointer to internally stored (in management of class Matrix3<T>)
	 * used for passing Matrix3<T> values to gl*[fd]v functions.
	 */
	operator T* ()
	{
		return (T*)data;
	}

	/**
	 * Conversion to pointer operator
	 * @return Constant Pointer to internally stored (in management of class Matrix3<T>)
	 * used for passing Matrix3<T> values to gl*[fd]v functions.
	 */
	operator const T* () const
	{
		return (const T*)data;
	}

	//----------[ output operator ]----------------------------
	/**
	 * Output to stream operator
	 * @param lhs Left hand side argument of operator (commonly ostream instance).
	 * @param rhs Right hand side argument of operator.
	 * @return Left hand side argument - the ostream object passed to operator.
	 */
	friend std::ostream& operator <<(std::ostream& lhs, const Matrix3<T>& rhs)
	{
		for (int i = 0; i < 3; i++)
		{
			lhs << "|\t";
			for (int j = 0; j < 3; j++)
			{
				lhs << rhs.at(j, i) << "\t";
			}
			lhs << "|" << std::endl;
		}
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

/// Matrix 3x3 of floats
typedef Matrix3<float> Matrix3f;
/// Matrix 3x3 of doubles
typedef Matrix3<double> Matrix3d;
/// Matrix 3x3 of int
typedef Matrix3<int> Matrix3i;
