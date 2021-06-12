#pragma once
#include "Util/Math/Vector3.h"
#include <string>
#include <sstream>
#include "olcPixelGameEngine.h"
#include "Util/Log.h"


template <typename T>
class AABB2 {
public:


	std::string toString() {
		std::stringstream ss;
		ss << "AABB [location=" << location.toStr() << ", size=" << size.toStr() << "]";
		return ss.str();
	}

	AABB2() {
		AABB2(Vector2<T>(0, 0), Vector2<T>(0, 0));
	}

	AABB2(Vector2<T> Location, Vector2<T> Size) {
		location = Location;
		size = Size;
		bounds[0] = location;
		bounds[1] = size;
	}

	bool operator== (const AABB2 other) const {
		return (location == other.location) && (size == other.size);
	}

	bool operator<= (const AABB2 other) const {
		return (location <= other.location && (location + size) <= (other.location + other.size));
	}
	bool operator>= (const AABB2 other) const {
		return (location >= other.location && (location + size) >= (other.location + other.size));
	}
	bool operator< (const AABB2 other) const {
		return (location < other.location && (location + size) < (other.location + other.size));
	}
	bool operator> (const AABB2 other) const {
		return (location > other.location && (location + size) > (other.location + other.size));
	}



	bool contains(AABB2<T> b) {
		const bool htlut7[7] = { false, false, true, false, false, false, true };

		AABB2<T> a = *this;
		//T mllx, Mlly, mslz, mllx, mlly, mllz;
		//T mllx, Mlly, Mslz, Mllx, Mlly, Mllz;

		//T mllx = std::min(a.getMin().x, b.getMin().x); 
		T mllx = std::max(a.getMin().x, b.getMin().x);
		//T Mlly = std::min(a.getMin().y, b.getMin().y);
		T mlly = std::max(a.getMin().y, b.getMin().y);


		T Mllx = std::min(a.getMax().x, b.getMax().x);
		//T Mllx = std::max(a.getMax().x, b.getMax().x);
		T Mlly = std::min(a.getMax().y, b.getMax().y);
		//T Mlly = std::max(a.getMax().y, b.getMax().y);


		std::stringstream ss;
		ss << "tests : " << std::endl;

		ss << "min/Smallest v Max/Smallest GT " << (mllx < mllx) << (Mlly < Mlly) << std::endl;
		ss << "min/Smallest v Max/Smallest GTET " << (mllx <= mllx) << (Mlly <= Mlly) << std::endl;

		ss << "min/Largest v Max/Largest GT " << (mllx < Mllx) << (mlly < Mlly) << std::endl;
		ss << "min/Largest v Max/Largest GTET " << (mllx <= Mllx) << (mlly <= Mlly) << std::endl;


		ss << "min/Smallest v Max/Largest GT " << (mllx < Mllx) << (Mlly < Mlly) << std::endl;
		ss << "min/Smallest v Max/Largest GTET " << (mllx <= Mllx) << (Mlly <= Mlly) << std::endl;


		ss << "min/Largest v Max/Smallest GT " << (mllx < mllx) << (mlly < Mlly) << std::endl;
		ss << "min/Largest v Max/Smallest GTET " << (mllx <= mllx) << (mlly <= Mlly) << std::endl;

		int GT = (mllx < mllx) + (mlly < Mlly);
		int GTET = (mllx <= mllx) + (mlly <= Mlly);

		ss << "gt = " << GT << std::endl;
		ss << "gtet = " << GTET << std::endl;
		ss << "tot = " << GT + GTET << std::endl;



		debug(ss.str());

		return htlut7[GT + GTET];
	}


	/**
	* Calculates the center of the AABB
	*
	* @param  none.
	* @return      a Vector2 containing the centerpoint of the AABB.
	* @see
	*/
	Vector2<T> getCenter() {
		return Vector2<T>(location + (getHalfsize()));
	}


	/**
	 * Returns the current location of this AABB.
	 * @return the current location value.
	 */
	Vector2<T> getLocation() {
		return location;
	}

	/**
	 * Returns the current size of this AABB
	 * @return The current size value.
	 */
	Vector2<T> getSize() {
		return size;
	}
	/**
	 * Calculates the half size of this AABB
	 * @return The half size of this AABB
	 */
	Vector2<T> getHalfsize() {
		return Vector2<T>(size.x / 2, size.y / 2);
	}

	/**
	 * Calculates the Maximum extent of this AABB
	 * @return The maximum value (location+size)
	 */
	Vector2<T> getMax() {
		return Vector2<T>(location.x + size.x, location.y + size.y);
	}

	/**
	 * Calculates the Minimum extent of this AABB
	 * @return The min value (location)
	 */
	Vector2<T> getMin() {
		return location;
	}


	void setLocation(Vector2<T> l) {
		location = l;
		bounds[0] = location;
	}

	void setSize(Vector2<T> s) {
		size = s;
		bounds[1] = size;
	}



	void debugDraw() {

	}


private:

	Vector2<T> location;
	Vector2<T> size;
	Vector2<T> bounds[2];
};

typedef AABB2<float> AABB2f;
typedef AABB2<double> AABB2d;
typedef AABB2<int> AABB2i;


template <typename T>
class AABB3 {
public:


	std::string toString() {
		std::stringstream ss;
		ss << "AABB [location=" << location.toStr() << ", size=" << size.toStr() << "]";
		return ss.str();
	}

	AABB3() {
		AABB3(Vector3<T>(0, 0, 0), Vector3<T>(0, 0, 0));
	}

	AABB3(Vector3<T> Location, Vector3<T> Size) {
		location = Location;
		size = Size;
		bounds[0] = location;
		bounds[1] = size;
	}

	bool operator== (const AABB3 other) const{
		std::stringstream ss;
		ss << location.toStr() << " == " << other.location.toStr() << std::endl;
		ss << size.toStr() << " == " << other.size.toStr() << std::endl;
		debug(ss.str());
		return (location == other.location) && (size == other.size);
	}

	bool operator<= (const AABB3 other) const {
		return (location <= other.location && (location + size) <= (other.location + other.size));
	}
	bool operator>= (const AABB3 other) const {
		return (location >= other.location && (location + size) >= (other.location + other.size));
	}
	bool operator< (const AABB3 other) const {
		return (location < other.location && (location + size) < (other.location + other.size));
	}
	bool operator> (const AABB3 other) const {
		return (location > other.location && (location + size) > (other.location + other.size));
	}

	

	bool contains(AABB3<T> b) {
		const bool htlut7[7] = { false, false, false, false, false, true, true };

		AABB3<T> a = *this;
		//T mllx, Mlly, mslz, mllx, mlly, mllz;
		//T mllx, Mlly, Mslz, Mllx, Mlly, Mllz;

		//T mllx = std::min(a.getMin().x, b.getMin().x); 
		T mllx = std::max(a.getMin().x, b.getMin().x);
		//T Mlly = std::min(a.getMin().y, b.getMin().y);
		T mlly = std::max(a.getMin().y, b.getMin().y);
		//T mslz = std::min(a.getMin().z, b.getMin().z);
		T mllz = std::max(a.getMin().z, b.getMin().z);

		T Mllx = std::min(a.getMax().x, b.getMax().x);
		//T Mllx = std::max(a.getMax().x, b.getMax().x);
		T Mlly = std::min(a.getMax().y, b.getMax().y);
		//T Mlly = std::max(a.getMax().y, b.getMax().y);
		T Mslz = std::min(a.getMax().z, b.getMax().z);
		//T Mllz = std::max(a.getMax().z, b.getMax().z);



		//std::stringstream ss;
	   // ss << "tests : " << std::endl;

		//ss << "min/Smallest v Max/Smallest GT " << (mllx < mllx) << (Mlly < Mlly) << (mslz < Mslz) << std::endl;
		//ss << "min/Smallest v Max/Smallest GTET " << (mllx <= mllx) << (Mlly <= Mlly) << (mslz <= Mslz) << std::endl;

		//ss << "min/Largest v Max/Largest GT " << (mllx < Mllx) << (mlly < Mlly) << (mllz < Mllz) << std::endl;
		//ss << "min/Largest v Max/Largest GTET " << (mllx <= Mllx) << (mlly <= Mlly) << (mllz <= Mllz) << std::endl;


		//ss << "min/Smallest v Max/Largest GT " << (mllx < Mllx) << (Mlly < Mlly) << (mslz < Mllz) << std::endl;
		//ss << "min/Smallest v Max/Largest GTET " << (mllx <= Mllx) << (Mlly <= Mlly) << (mslz <= Mllz) << std::endl;


		//ss << "min/Largest v Max/Smallest GT " << (mllx < mllx) << (mlly < Mlly) << (mllz < Mslz) << std::endl;
		//ss << "min/Largest v Max/Smallest GTET " << (mllx <= mllx) << (mlly <= Mlly) << (mllz <= Mslz) << std::endl;

		int GT = (mllx < mllx) + (mlly < Mlly) + (mllz < Mslz);
		int GTET = (mllx <= mllx) + (mlly <= Mlly) + (mllz <= Mslz);

		//debugss << "gt = " << GT << std::endl;
		//debugss << "gtet = " << GTET << std::endl;
		//debugss << "tot = " << GT + GTET << std::endl;



		//debug(ss.str());

		return htlut7[GT + GTET];
	}


	/**
	* Calculates the center of the AABB
	*
	* @param  none.
	* @return      a Vector3 containing the centerpoint of the AABB.
	* @see
	*/
	Vector3<T> getCenter() {
		return Vector3<T>(location + (getHalfsize()));
	}

	
	/**
	 * Returns the current location of this AABB.
	 * @return the current location value.
	 */
	Vector3<T> getLocation() {
		return location;
	}

	/**
	 * Returns the current size of this AABB
	 * @return The current size value.
	 */
	Vector3<T> getSize() {
		return size;
	}
	/**
	 * Calculates the half size of this AABB
	 * @return The half size of this AABB
	 */
	Vector3<T> getHalfsize() {
		return Vector3<T>(size.x / 2, size.y / 2, size.z / 2);
	}

	/**
	 * Calculates the Maximum extent of this AABB
	 * @return The maximum value (location+size)
	 */
	Vector3<T> getMax() {
		debugss << "getMax = " << location.x << "+" << size.x << ", " << "getMax = " << location.y << "+" << size.y << ", " << "getMax = " << location.z << "+" << size.z;
		return Vector3<T>(location.x + size.x, location.y + size.y, location.z + size.z);
	}

	/**
	 * Calculates the Minimum extent of this AABB
	 * @return The min value (location)
	 */
	Vector3<T> getMin() {
		return location;
	}


	void setLocation(Vector3<T> l) {
		location = l;
		bounds[0] = location;
	}

	void setSize(Vector3<T> s) {
		size = s;
		bounds[1] = size;
	}


	
	void debugDraw() {

	}

	operator AABB2<int>() const { return AABB2i(location, size); }

private:

		Vector3<T> location;
		Vector3<T> size;
		Vector3<T> bounds[2];
};

typedef AABB3<float> AABB3f;
typedef AABB3<double> AABB3d;
typedef AABB3<int> AABB3i;


