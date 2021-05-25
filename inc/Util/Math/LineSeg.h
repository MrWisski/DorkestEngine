#pragma once
#include "Vector2.h"
#include "Vector3.h"


/*
 2 Dimensional Line Segment.
*/
template <class T>
class LineSeg2D {
	Vector2<T> P1;
	Vector2<T> P2;

	LineSeg2D(Vector2<T> A, Vector2<T> B) : P1(A), P2(B) {}

	void operator= (LineSeg2D<T> O) { P1 = O.P1;  P2 = O.P2; }
	template <class tOther>
	void operator= (LineSeg2D<tOther> O) { P1 = O.P1;  P2 = O.P2; }

	void set(Vector2<T> A, Vector2<T> B) { P1 = A; P2 = B; }
	void set(LineSeg2D<T> O) { P1 = O.P1;  P2 = O.P2; }
	template <class tOther>
	void set(LineSeg2D<tOther> O){ P1 = O.P1;  P2 = O.P2; } 

};

/*
 3 Dimensional Line Segment.
*/
template <class T>
class LineSeg3D {
	Vector3<T> P1;
	Vector3<T> P2;

	LineSeg3D(Vector3<T> A, Vector3<T> B) : P1(A), P2(B) {}

	void operator= (LineSeg3D<T> O) { P1 = O.P1;  P2 = O.P2; }
	template <class tOther>
	void operator= (LineSeg3D<tOther> O) { P1 = O.P1;  P2 = O.P2; }

	void set(Vector3<T> A, Vector3<T> B) { P1 = A; P2 = B; }
	void set(LineSeg3D<T> O) { P1 = O.P1;  P2 = O.P2; }
	template <class tOther>
	void set(LineSeg3D<tOther> O) { P1 = O.P1;  P2 = O.P2; }


};

typedef LineSeg3D<float> LineSeg3Df;
typedef LineSeg3D<double> LineSeg3Dd;
typedef LineSeg3D<int> LineSeg3Di;

typedef LineSeg2D<float> LineSeg2Df;
typedef LineSeg2D<double> LineSeg2Dd;
typedef LineSeg2D<int> LineSeg2Di;
