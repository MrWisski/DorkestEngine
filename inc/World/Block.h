#pragma once
#include <olcPixelGameEngine.h>

class Block {
public:

	Vector3d wPos;
	Vector2i sPos;
	float dist = 0;
	olc::Pixel c;

	Block(const Block& b) {
		c = b.c;
		wPos = b.wPos;
		sPos = b.sPos;
		dist = b.dist;

	}

	Block(Block* b) {
		c = b->c;
		wPos = b->wPos;
		sPos = b->sPos;
		dist = b->dist;

	}

	Block(olc::Pixel color, float distance = 0, Vector3f w = { 0,0,0 }, Vector2i s = { 0,0 } ) {
		c = color;
		wPos = w;
		sPos = s;
		dist = distance;
	}
	~Block() {}
	
	bool operator < (Block other) {
		if (other.dist == 0 || dist == 0) {
			if ((sPos.y < other.sPos.y) == (other.sPos.y < sPos.y)) {
				return (sPos.x < other.sPos.x);
			}
			return (sPos.y < other.sPos.y);
		}
		else {
			return dist < other.dist;
		}
	}

	bool operator == (Block other) {
		return wPos == other.wPos;
	}
	bool operator == (Block* other) {
		return wPos == other->wPos;
	}

};