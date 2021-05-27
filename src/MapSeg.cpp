#include "World/MapSeg.h"

MapSeg::MapSeg(AABB<float> bounds, dorkestScene* parent)
{
	this->bBox = bounds;
	this->owner = parent;
}

MapSeg::~MapSeg()
{
	for (auto e : this->entVec) {
		if (e != nullptr) { delete e; }
	}
}

void MapSeg::loadPrefab(std::string path)
{
	//TODO : Create loadPrefab - just load in an entity registry from disk.
	error("Feature not coded yet!");
}

dorkestBaseEntity* MapSeg::createEntity()
{
	dorkestBaseEntity* ret = new dorkestBaseEntity(this->entities);
	entVec.push_back(ret);
	return ret;
}

TerrainEntity* MapSeg::createTerrainEntity(AABB<float> bounds)
{
	TerrainEntity* tEnt = new TerrainEntity(this, bounds);
	entVec.push_back(tEnt);

	return tEnt;
}

dorkestBaseEntity* MapSeg::getEntity(entt::entity ent)
{
	for (auto e : this->entVec) {
		if (e->getHandle() == ent) { return e; }
	}
	return nullptr;
}

std::vector<dorkestBaseEntity*> MapSeg::getImpacts(Ray<float> r)
{
	return std::vector<dorkestBaseEntity*>();
}

dorkestBaseEntity* MapSeg::getImpact(Vector3f wPoint)
{
	return nullptr;
}

void MapSeg::updateStaticLighting(std::vector<dorkestBaseEntity*> staticLights)
{
}

void MapSeg::updateDynamicLighting(std::vector<dorkestBaseEntity*> dynLights)
{
}
