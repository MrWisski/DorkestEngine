#pragma once
#include <Util/Color.h>
#include <Util/Math/Vector3.h>

class Vert {
public:

	Vert() = default;
	Vert(Vector3f position) : pos(position) {}
	Vert(Vector3f position, Colorf color) : pos(position), col(color) {}
	Vert(Vector3f position, Colorf color, Vector3f texCoord) : pos(position), col(color), tCoord(texCoord) {}



	Vector3f pos = Vector3f(0.0f,0.0f,0.0f);
	Colorf col = Colorf(1.0f,1.0f,1.0f,1.0f);
	Vector3f tCoord;
};

class Triangle {
public:
	Triangle() = default;
	Triangle(Vert a, Vert b, Vert c) { v[0] = a, v[1] = b; v[2] = c; }
	~Triangle() = default;

	Vert operator[](int n) {
		assert(n >= 0); 
		assert(n <= 2);
		return v[n];
	}

	Colorf getCol(int n) {
		assert(n > 0); assert(n < 3);
		return v[n].col;
	}
private:
		Vert v[3];
		
};

class Square {
public:

	Square() = default;

	Square(Vert a, Vert b, Vert c, Vert d) {
		tris[0] = Triangle(a, b, c);
		tris[1] = Triangle(d, c, b);
	}

	Square(Triangle a, Triangle b){
	
	}

	Square(Vector3f min, Vector3f max) {

	}


	~Square() = default;
	
	Triangle tris[2];
};