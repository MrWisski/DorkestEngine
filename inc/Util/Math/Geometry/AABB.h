#pragma once
#include "Util/Math/Vector3.h"
#include <string>
#include <sstream>
#include "olcPixelGameEngine.h"


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