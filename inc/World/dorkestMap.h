#pragma once
#include "ECS/dorkestScene.h"


class dorkestMap {
public:
	dorkestMap(dorkestScene* parent);
	MapSeg* get(int x, int y) ;
	void sortAll();
	~dorkestMap();

private:
	MapSeg* map[MAX_MAPSEG_DIMENSION * MAX_MAPSEG_DIMENSION];

};

