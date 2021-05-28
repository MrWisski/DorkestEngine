#include "World/MapSeg.h"
#include <Render/dorkestRenderer.h>
#include <Util/Math/Vector4.h>

MapSeg::MapSeg(AABB<float> bounds, dorkestScene* parent)
{
	this->bBox = bounds;
	this->owner = parent;
	this->entities = new entt::registry();
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
	dorkestRenderer* r = this->owner->getRenderer();
	Vector2i screenLoc = r->MapToScreen({ 32.0f,32.0f }, 1, bounds.getLocation(), { 0,0 });

	TerrainEntity* tEnt = new TerrainEntity(this->entities,bounds,screenLoc);
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

void MapSeg::updateLighting(std::vector<dorkestBaseEntity*> staticLights, std::vector<dorkestBaseEntity*> dynLights)
{
	//If we have no entities - SKIP!
	if (this->entVec.size() == 0) { return; }

	//Create a view for each entity with a POSITION and a LIGHTSINK.
	auto sinkView = this->entities->view<c_position,c_lightSink>();

	if (staticLights.size() != 0) {
		//We have static lights. 
		//Iterate over the sinkView
		for (entt::entity en : sinkView) {
			//Create a DBE wrapper with the en entity handle.
			dorkestBaseEntity e(this->entities, en);
			//OK, first we need to get the position, and lightsink component, for this entity.
			c_position* ep = e.getComponent<c_position>();
			c_lightSink* c = e.getComponent<c_lightSink>();
			c_baseColor* bc = e.getComponent<c_baseColor>();

			c->data.intensity = 0;
			c->data.combinedColor = olc::BLACK;
			//NOW we can recalc the light for this entity.
			//Iterate over each light
			for (dorkestBaseEntity* l : staticLights) {
				c_staticLight* tl = l->getComponent<c_staticLight>();

				//Get the distance between the light and this entity.
				float dist = l->getComponent<c_position>()->worldPos.dist(ep->worldPos);
				//Calculate the attenuation.
				float attenuation = 1.0 / (tl->data.constant + tl->data.linear * dist + tl->data.quadratic * (dist * dist));
				c->data.intensity += attenuation;
				c->data.combinedColor += (tl->data.color * attenuation);
				debug("Our attenuation value at distance of " + std::to_string(dist) + " is " + std::to_string(attenuation));
			}

			Vector4f vc(c->data.combinedColor);
			debug("Our intensity is " + std::to_string(c->data.intensity) + " and our color ends up being " + vc.toStr());

			

		}
	}

	if (dynLights.size() != 0) {

	}

	this->sort();

}

void MapSeg::sort()
{
	this->entities->sort<c_position>([](const c_position& lhs, const c_position& rhs) {
		if (lhs.worldPos.y != rhs.worldPos.y)
			return lhs.worldPos.y < rhs.worldPos.y;
		if (lhs.worldPos.x != rhs.worldPos.x)
			return lhs.worldPos.x < rhs.worldPos.x;
		return lhs.worldPos.z < rhs.worldPos.z;
		});
}
