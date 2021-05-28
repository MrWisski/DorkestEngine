#include "World/dorkestMap.h"
#include "ECS/dorkestScene.h"
#include <Util/Math/Geometry/AABB.h>
#include <World/MapSeg.h>

dorkestMap::dorkestMap(dorkestScene* parent) {
	for (int x = 0; x < MAX_MAPSEG_DIMENSION; x++)
		for (int y = 0; y < MAX_MAPSEG_DIMENSION; y++) {
			map[x + MAX_MAPSEG_DIMENSION * y] = new MapSeg(AABB<float>(Vector3f(x * SEGMENT_DIMENSION, y * SEGMENT_DIMENSION, 0), Vector3f(SEGMENT_DIMENSION, SEGMENT_DIMENSION, SEGMENT_DIMENSION)), parent);
		}
};

MapSeg* dorkestMap::get(int x, int y) { return map[x + MAX_MAPSEG_DIMENSION * y]; }

void dorkestMap::sortAll()
{
	dorkestDataPoint dp("mapSortAll");
	for (int x = 0; x < MAX_MAPSEG_DIMENSION; x++)
		for (int y = 0; y < MAX_MAPSEG_DIMENSION; y++) {
			map[x + MAX_MAPSEG_DIMENSION * y]->sort();
		}

};

dorkestMap::~dorkestMap() {
	int count = 0;
	for (auto seg : map) {
		if (seg != nullptr) {
			delete seg;
			count++;
		}
	}
	debug("Cleaned up " + std::to_string(count));
};