#pragma once

#include "Ray.h"
#include "LineSeg.h"
#include "AABB.h"
#include "Vector3.h"
#include "Plane.h"

#define SMALL_NUM   0.00000001 // anything that avoids division overflow
// dot product (3D) which allows vector operations in arguments
#define dot(u,v)   ((u).x * (v).x + (u).y * (v).y + (u).z * (v).z)
#define perp(u,v)  ((u).x * (v).y - (u).y * (v).x)  // perp product  (2D




template< class T>
class HitTest {
public:

    enum hitType { NONE = 0, INTERSECT = 1, OVERLAP = 2 };
	

	/**
	 * Checks if Line Segment A intersects Line Segment B
	 * @param A - first line segment.
	 * @param B - second line segment.
	 * @param IO (RETURN VALUE) - Intersection point of the 2 segments, if they exist
	 * @param I1 (RETURN VALUE) - End point of intersect segment (pointA, pointB) if it exists.
	 * @return hitType - enum containing result of test.
	 */
	static hitType I2D_seg_seg(LineSeg2D<T> A, LineSeg2D<T> B, Vector2<T>* I0, Vector2<T>* I1) {
		Vector2<T>    u = A.P1 - A.P0;
		Vector2<T>    v = B.P1 - B.P0;
		Vector2<T>    w = A.P0 - B.P0;
		float     D = perp(u, v);

        // test if  they are parallel (includes either being a point)
        if (fabs(D) < SMALL_NUM) {           // A and B are parallel
            if (perp(u, w) != 0 || perp(v, w) != 0) {
                return NONE;                    // they are NOT collinear
            }
            // they are collinear or degenerate
            // check if they are degenerate  points
            float du = dot(u, u);
            float dv = dot(v, v);
            if (du == 0 && dv == 0) {            // both segments are points
                if (A.P0 != B.P0)         // they are distinct  points
                    return 0;
                *I0 = A.P0;                 // they are the same point
                return INTERSECT;
            }
            if (du == 0) {                     // A is a single point
                if (inSegment(A.P0, B) == 0)  // but is not in B
                    return 0;
                *I0 = A.P0;
                return INTERSECT;
            }
            if (dv == 0) {                     // B a single point
                if (inSegment(B.P0, A) == 0)  // but is not in A
                    return 0;
                *I0 = B.P0;
                return INTERSECT;
            }
            // they are collinear segments - get  overlap (or not)
            float t0, t1;                    // endpoints of A in eqn for B
            Vector2<T> w2 = A.P1 - B.P0;
            if (v.x != 0) {
                t0 = w.x / v.x;
                t1 = w2.x / v.x;
            }
            else {
                t0 = w.y / v.y;
                t1 = w2.y / v.y;
            }
            if (t0 > t1) {                   // must have t0 smaller than t1
                float t = t0; t0 = t1; t1 = t;    // swap if not
            }
            if (t0 > 1 || t1 < 0) {
                return NONE;      // NO overlap
            }
            t0 = t0 < 0 ? 0 : t0;               // clip to min 0
            t1 = t1 > 1 ? 1 : t1;               // clip to max 1
            if (t0 == t1) {                  // intersect is a point
                *I0 = B.P0 + t0 * v;
                return INTERSECT;
            }

            // they overlap in a valid subsegment
            *I0 = B.P0 + t0 * v;
            *I1 = B.P0 + t1 * v;
            return OVERLAP;
        }

        // the segments are skew and may intersect in a point
        // get the intersect parameter for A
        float     sI = perp(v, w) / D;
        if (sI < 0 || sI > 1)                // no intersect with A
            return 0;
        // get the intersect parameter for B
        float     tI = perp(u, w) / D;
        if (tI < 0 || tI > 1)                // no intersect with B
            return 0;

        *I0 = A.P0 + sI * u;                // compute A intersect point
        return 1;
	}

    static hitType I2D_seg_point(Vector2<T> P, LineSeg2D<T> S)
    {
        if (S.P0.x != S.P1.x) {    // S is not  vertical
            if (S.P0.x <= P.x && P.x <= S.P1.x)
                return INTERSECT;
            if (S.P0.x >= P.x && P.x >= S.P1.x)
                return INTERSECT;
        }
        else {    // S is vertical, so test y  coordinate
            if (S.P0.y <= P.y && P.y <= S.P1.y)
                return INTERSECT;
            if (S.P0.y >= P.y && P.y >= S.P1.y)
                return INTERSECT;
        }
        return NONE;
    }

    static inline const hitType htlut7[7] = {NONE,NONE,NONE,NONE,NONE,OVERLAP, INTERSECT};

    static hitType i3D_AABB_AABB(AABB<T> a, AABB<T> b) {
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
        
        return HitTest<T>::htlut7[GT+GTET];
    }

    // intersect3D_SegmentPlane(): get the intersect of a segment and plane
//    Input:  S = a segment, and Pn = a plane = {Point V0;  Vector n;}
//    Output: *I0 = the intersect point (when it exists)
//    Return: 0 = disjoint (no intersection)
//            1 =  intersection in the unique point *I0
//            2 = the  segment lies in the plane
    static hitType i3D_seg_plane(LineSeg3D<T> S, Plane3d<T> Pn, Vector3<T> *I)
    {
        Vector3<T>    u = S.P1 - S.P0;
        Vector3<T>    w = S.P0 - Pn.point;

        float     D = dot(Pn.normal, u);
        float     N = -dot(Pn.n, w);

        if (fabs(D) < SMALL_NUM) {           // segment is parallel to plane
            if (N == 0)                      // segment lies in plane
                return OVERLAP;
            else
                return NONE;                    // no intersection
        }
        // they are not parallel
        // compute intersect param
        float sI = N / D;
        if (sI < 0 || sI > 1)
            return NONE;                        // no intersection

        *I = S.P0 + sI * u;                  // compute segment intersect point
        return INTERSECT;
    }
   
    // intersect3D_2Planes(): find the 3D intersection of two planes
//    Input:  two planes Pn1 and Pn2
//    Output: *L = the intersection line (when it exists)
//    Return: 0 = disjoint (no intersection)
//            1 = the two  planes coincide
//            2 =  intersection in the unique line *L
    static hitType i3D_plane_plane(Plane3d<T> Pn1, Plane3d<T> Pn2, LineSeg3D<T>* L)
    {
        Vector3<T>   u = Pn1.n * Pn2.n;          // cross product
        float    ax = (u.x >= 0 ? u.x : -u.x);
        float    ay = (u.y >= 0 ? u.y : -u.y);
        float    az = (u.z >= 0 ? u.z : -u.z);

        // test if the two planes are parallel
        if ((ax + ay + az) < SMALL_NUM) {        // Pn1 and Pn2 are near parallel
            // test if disjoint or coincide
            Vector3<T>   v = Pn2.V0 - Pn1.V0;
            if (dot(Pn1.n, v) == 0)          // Pn2.V0 lies in Pn1
                return INTERSECT;                    // Pn1 and Pn2 coincide
            else
                return NONE;                    // Pn1 and Pn2 are disjoint
        }

        // Pn1 and Pn2 intersect in a line
        // first determine max abs coordinate of cross product
        int      maxc;                       // max coordinate
        if (ax > ay) {
            if (ax > az)
                maxc = 1;
            else maxc = 3;
        }
        else {
            if (ay > az)
                maxc = 2;
            else maxc = 3;
        }

        // next, to get a point on the intersect line
        // zero the max coord, and solve for the other two
        Vector3<T>    iP;                // intersect point
        float    d1, d2;            // the constants in the 2 plane equations
        d1 = -dot(Pn1.n, Pn1.V0);  // note: could be pre-stored  with plane
        d2 = -dot(Pn2.n, Pn2.V0);  // ditto

        switch (maxc) {             // select max coordinate
        case 1:                     // intersect with x=0
            iP.x = 0;
            iP.y = (d2 * Pn1.n.z - d1 * Pn2.n.z) / u.x;
            iP.z = (d1 * Pn2.n.y - d2 * Pn1.n.y) / u.x;
            break;
        case 2:                     // intersect with y=0
            iP.x = (d1 * Pn2.n.z - d2 * Pn1.n.z) / u.y;
            iP.y = 0;
            iP.z = (d2 * Pn1.n.x - d1 * Pn2.n.x) / u.y;
            break;
        case 3:                     // intersect with z=0
            iP.x = (d2 * Pn1.n.y - d1 * Pn2.n.y) / u.z;
            iP.y = (d1 * Pn2.n.x - d2 * Pn1.n.x) / u.z;
            iP.z = 0;
        }
        
        L->P0 = iP;
        L->P1 = iP + u;
        return OVERLAP;
    }

    static inline hitType htlut12[13] = { NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, OVERLAP, NONE, NONE, INTERSECT };

    static hitType i3D_AABB_Point(AABB<T> aa, Vector3<T> p) {
        int minGT = (aa.getMin().x < p.x) + (aa.getMin().x < p.y) + (aa.getMin().x < p.z);
        int maxGT = (aa.getMax().x > p.x) + (aa.getMax().x > p.y) + (aa.getMax().x > p.z);
        int minGTET = (aa.getMin().x <= p.x) + (aa.getMin().x <= p.y) + (aa.getMin().x <= p.z);
        int maxGTET = (aa.getMax().x >= p.x) + (aa.getMax().x >= p.y) + (aa.getMax().x >= p.z);
        return htlut12[minGT + maxGT + minGTET + maxGTET-1];
    }
	
    static bool I3D_ray_aabb(Ray<T> r, AABB<T> b) {
        double t1 = (b.getMin().x - r.orig.x) * r.invdir.x;
        double t2 = (b.getMax().x - r.orig.x) * r.invdir.x;

        double tmin = std::min(t1, t2);
        double tmax = std::max(t1, t2);

		t1 = (b.getMin().y - r.orig.y) * r.invdir.y;
		t2 = (b.getMax().y - r.orig.y) * r.invdir.y;

		tmin = std::max(tmin, std::min(t1, t2));
		tmax = std::min(tmax, std::max(t1, t2));

		t1 = (b.getMin().z - r.orig.z) * r.invdir.z;
		t2 = (b.getMax().z - r.orig.z) * r.invdir.z;

		tmin = std::max(tmin, std::min(t1, t2));
		tmax = std::min(tmax, std::max(t1, t2));



        return tmax > std::max(tmin, 0.0);
   
    }
  
    
};
