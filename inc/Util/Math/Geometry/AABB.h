#pragma once
#include "Util/Math/Vector3.h"
#include <string>
#include <sstream>
#include "olcPixelGameEngine.h"
#include "Util/Log.h"


using String = std::string;

template <typename T>
class AABB {
public:


	String toString() {
		std::stringstream ss;
		ss << "AABB [location=" << location.toStr() << ", size=" << size.toStr() << "]";
		return ss.str();
	}

	AABB() {
		AABB(Vector3<T>(0, 0, 0), Vector3<T>(0, 0, 0));
	}

	AABB(Vector3<T> Location, Vector3<T> Size) {
		location = Location;
		size = Size;
		bounds[0] = location;
		bounds[1] = size;
	}

	bool operator== (const AABB other) const{
		std::stringstream ss;
		ss << location.toStr() << " == " << other.location.toStr() << std::endl;
		ss << size.toStr() << " == " << other.size.toStr() << std::endl;
		debug(ss.str());
		return (location == other.location) && (size == other.size);
	}

	bool operator<= (const AABB other) const {
		return (location <= other.location && (location + size) <= (other.location + other.size));
	}
	bool operator>= (const AABB other) const {
		return (location >= other.location && (location + size) >= (other.location + other.size));
	}
	bool operator< (const AABB other) const {
		return (location < other.location && (location + size) < (other.location + other.size));
	}
	bool operator> (const AABB other) const {
		return (location > other.location && (location + size) > (other.location + other.size));
	}

	

	bool contains(AABB<T> b) {
		const bool htlut7[7] = { false, false, false, false, false, false, true };

		AABB<T> a = *this;
		//T mslx, msly, mslz, mllx, mlly, mllz;
		//T Mslx, Msly, Mslz, Mllx, Mlly, Mllz;

		//T mslx = std::min(a.getMin().x, b.getMin().x); 
		T mllx = std::max(a.getMin().x, b.getMin().x);
		//T msly = std::min(a.getMin().y, b.getMin().y);
		T mlly = std::max(a.getMin().y, b.getMin().y);
		//T mslz = std::min(a.getMin().z, b.getMin().z);
		T mllz = std::max(a.getMin().z, b.getMin().z);

		T Mslx = std::min(a.getMax().x, b.getMax().x);
		//T Mllx = std::max(a.getMax().x, b.getMax().x);
		T Msly = std::min(a.getMax().y, b.getMax().y);
		//T Mlly = std::max(a.getMax().y, b.getMax().y);
		T Mslz = std::min(a.getMax().z, b.getMax().z);
		//T Mllz = std::max(a.getMax().z, b.getMax().z);



		//std::stringstream ss;
	   // ss << "tests : " << std::endl;

		//ss << "min/Smallest v Max/Smallest GT " << (mslx < Mslx) << (msly < Msly) << (mslz < Mslz) << std::endl;
		//ss << "min/Smallest v Max/Smallest GTET " << (mslx <= Mslx) << (msly <= Msly) << (mslz <= Mslz) << std::endl;

		//ss << "min/Largest v Max/Largest GT " << (mllx < Mllx) << (mlly < Mlly) << (mllz < Mllz) << std::endl;
		//ss << "min/Largest v Max/Largest GTET " << (mllx <= Mllx) << (mlly <= Mlly) << (mllz <= Mllz) << std::endl;


		//ss << "min/Smallest v Max/Largest GT " << (mslx < Mllx) << (msly < Mlly) << (mslz < Mllz) << std::endl;
		//ss << "min/Smallest v Max/Largest GTET " << (mslx <= Mllx) << (msly <= Mlly) << (mslz <= Mllz) << std::endl;


		//ss << "min/Largest v Max/Smallest GT " << (mllx < Mslx) << (mlly < Msly) << (mllz < Mslz) << std::endl;
		//ss << "min/Largest v Max/Smallest GTET " << (mllx <= Mslx) << (mlly <= Msly) << (mllz <= Mslz) << std::endl;

		int GT = (mllx < Mslx) + (mlly < Msly) + (mllz < Mslz);
		int GTET = (mllx <= Mslx) + (mlly <= Msly) + (mllz <= Mslz);

		//ss << "gt = " << GT << std::endl;
		//ss << "gtet = " << GTET << std::endl;
		//ss << "tot = " << GT + GTET << std::endl;



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


private:

		Vector3<T> location;
		Vector3<T> size;
		Vector3<T> bounds[2];
};