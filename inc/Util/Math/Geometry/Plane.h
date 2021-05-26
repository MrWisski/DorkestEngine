#pragma once
#include "Util/Math/Vector3.h"
#include "Util/Math/Geometry/Ray.h"
template <class T>
class Plane3d {
public:
	Vector3<T> point;
	Vector3<T> normal;

	Plane3d(Vector3<T> loc, Vector3<T> norm) : point(loc), normal(norm) { normal.norm(); }
	Plane3d(Ray<T> r) : point(r.orig), normal(r.dir) { normal.norm(); }
	Plane3d(const Plane3d<T>& other) : point(other.point), normal(other.normal) { normal.norm(); }
	
	void set(Vector3<T> loc, Vector3<T> norm) {	point = (loc); normal = (norm);  normal.norm(); }
	void set(Ray<T> r) { point = (r.orig); normal = (r.dir); normal.norm();	}
	void set(Plane3d<T> other){	point = (other.point); normal = (other.normal); normal.norm();}

};