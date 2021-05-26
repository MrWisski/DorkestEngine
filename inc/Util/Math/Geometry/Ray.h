#pragma once
#include "Util/Math/Vector3.h"
#include "Util/Log.h"




template <class T>
class Ray
{
public:

	Vector3<T> orig, dir; /// ray orig and dir
	T tmin, tmax; /// ray min and max distances
	Vector3<T> invdir;
	int sign[3];



	Ray() { error("RAY CALLED WITH EMPTY CONSTRUCTOR!"); }

	Ray(Vector3<T> orig, Vector3<T> dir, float maxlength) : orig(orig), dir(dir), tmin(0), tmax(maxlength) {
		invdir.x = 1 / dir.x;
		invdir.y = 1 / dir.y;
		invdir.z = 1 / dir.z;
		sign[0] = (invdir.x < 0);
		//debug("r.sign[0] = " + std::to_string(sign[0]));
		sign[1] = (invdir.y < 0);
		//debug("r.sign[1] = " + std::to_string(sign[1]));
		sign[2] = (invdir.z < 0);
		//debug("r.sign[2] = " + std::to_string(sign[2]));
	}

	Ray(const Ray& src) : orig(src.orig), dir(src.dir), invdir(src.invdir), tmin(src.tmin), tmax(src.tmax){
		invdir.x = 1 / dir.x;
		invdir.y = 1 / dir.y;
		invdir.z = 1 / dir.z;
		sign[0] = (invdir.x < 0);
		//debug("r.sign[0] = " + std::to_string(sign[0]));
		sign[1] = (invdir.y < 0);
		//debug("r.sign[1] = " + std::to_string(sign[1]));
		sign[2] = (invdir.z < 0);
		//debug("r.sign[2] = " + std::to_string(sign[2]));

	}

	virtual ~Ray() {}

	void Set(Vector3<T> orig, Vector3<T> dir, float maxlength) {
		this->orig = orig;
		this->dir = dir;
		this->tmax = maxlength;
		invdir.x = 1 / dir.x;
		invdir.y = 1 / dir.y;
		invdir.z = 1 / dir.z;

		sign[0] = (invdir.x < 0);
		//debug("r.sign[0] = " + std::to_string(sign[0]));
		sign[1] = (invdir.y < 0);
		//debug("r.sign[1] = " + std::to_string(sign[1]));
		sign[2] = (invdir.z < 0);
		//debug("r.sign[2] = " + std::to_string(sign[2]));
	}

	// normalize ray = normalize direction
	void normalize() { dir.norm(); }

	// calculate point by factor t
	// point = start_ + t * dir
	Vector3<T> getPoint(float t) {
		Vector3<T> dir = dir;
		dir *= t;
		return orig + dir;
	}

	Vector3<T> getSegmentP2() {
		return orig + (dir * tmax);
	}
	
	/*
	bool AABBIntersection(AABB<T> box, T dist)
	{
		// if starting point is inside axis aligned bounding box, the ray goes through it for sure
		if (box.contains(orig))
		{
			debug("BOX CONTAINS START");
			return true;
		} else{
				debug("-----------------------------------------------");
		}

		Vector3<T> min = box.getLocation();
		Vector3<T> max = box.getMax();

		float t[6];

		// min is used for left, bottom and near plane
		t[0] = (min.x - orig.x) / dir.x;
		t[1] = (min.y - orig.y) / dir.y;
		t[2] = (min.z - orig.z) / dir.z;

		// max is used for right, top and far plane
		t[3] = (max.x - orig.x) / dir.x;
		t[4] = (max.y - orig.y) / dir.y;
		t[5] = (max.z - orig.z) / dir.z;

		// we have 6 different distance values, and therefore 6 different points
		//   along the ray. we need to query from x, y and z just 2 values, 1 of them
		//   is equal to to "min"/"max" (i.e. ray(tl) has min.x, y and z in query)
		

		Vector3<T> temp;

		for (int i = 0; i < 6; i++)
		{
			if (t[i] > dist || t[i] < 0.0)
				continue;

			temp = getPoint(t[i]);

			if ((i % 3) == 0)
			{
				// it's left/right, ask for y and z
				if (temp.y >= min.y && temp.y <= max.y &&
					temp.z >= min.z && temp.z <= max.z)
				{
					dist = t[i];
					return true;
				}
			}

			if ((i % 3) == 1)
			{
				// it's bottom/top, ask for x and z
				if (temp.x >= min.x && temp.x <= max.x &&
					temp.z >= min.z && temp.z <= max.z)
				{
					dist = t[i];
					return true;
				}
			}

			if ((i % 3) == 2)
			{
				// it's near/far, ask for x and y
				if (temp.x >= min.x && temp.x <= max.x &&
					temp.y >= min.y && temp.y <= max.y)
				{
					dist = t[i];
					return true;
				}
			}
		}

		// we never hit bounding box
		debug("No hit");
		return false;
	}
	*/
};
