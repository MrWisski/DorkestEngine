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
	int s = this->entVec.size();
	if ( s == 0) return;

	this->entVec.clear();

	entities->each([this](const entt::entity e) { this->entities->destroy(e); });

}

void MapSeg::loadPrefab(std::string path)
{
	//TODO : Create loadPrefab - just load in an entity registry from disk.
	error("Feature not coded yet!");
}

std::shared_ptr<dorkestBaseEntity> MapSeg::createEntity()
{
	std::shared_ptr<dorkestBaseEntity> ret = std::make_unique<dorkestBaseEntity>(this->entities);
	entVec.push_back(ret);
	return ret;
}

std::shared_ptr<dorkestBaseEntity> MapSeg::createTerrainEntity(AABB<float> bounds)
{
	assert(this->owner != nullptr);
	assert(this->entities != nullptr);

	dorkestRenderer* r = this->owner->getRenderer();
	Vector2i screenLoc = r->MapToScreen({ 32.0f,32.0f }, 1, bounds.getLocation(), { 0,0 });

	std::shared_ptr<dorkestBaseEntity> tEnt = std::make_shared<dorkestBaseEntity>(TerrainEntity(this->entities,bounds,screenLoc));
	entVec.push_back(tEnt);
	return tEnt;
}

std::shared_ptr<dorkestBaseEntity> MapSeg::getEntity(entt::entity ent)
{
	for (std::shared_ptr<dorkestBaseEntity> e : this->entVec) {
		if (e->getHandle() == ent) { return e; }
	}
	return nullptr;
}

std::vector<std::shared_ptr<dorkestBaseEntity>> MapSeg::getImpacts(Ray<float> r)
{
	return std::vector<std::shared_ptr<dorkestBaseEntity>>();
}

std::shared_ptr<dorkestBaseEntity> MapSeg::getImpact(Vector3f wPoint)
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
			c_position ep = e.getComponent<c_position>();
			c_lightSink c = e.getComponent<c_lightSink>();
			c_baseColor bc = e.getComponent<c_baseColor>();
			c.data.staticLights.clear();
			c.data.combinedColor = olc::BLACK;
			//NOW we can recalc the light for this entity.
			//Iterate over each light
			for (dorkestBaseEntity* l : staticLights) {
				c_staticLight tl = l->getComponent<c_staticLight>();

				std::stringstream ss;
				float dist = l->getComponent<c_position>().worldPos.dist(ep.worldPos);

				#define el std::endl
				ss << el;
				ss << "Distance between " << l->getComponent<c_position>().worldPos.toString() << " and " << ep.worldPos.toString() << " is " << dist << el;
				//Get the distance between the light and this entity.
				//Calculate the attenuation.
				float inten = tl.data.getIntensityFromDistance(dist);
				std::pair<float, olc::Pixel> lPair;
				lPair.first = inten;
				lPair.second = tl.data.color;
				c.data.staticLights.push_back(lPair);
				ss << "Intensity = " << inten << el;
				debug(ss.str());
			}

			std::stringstream ss;
			ss << std::endl;
			olc::Pixel lcol = olc::BLACK;
			for (auto lpair : c.data.staticLights) {
					
				olc::Pixel p = lightFunc::mixColors(lcol, (lpair.second * lpair.first),"mix",0.64f);
				lcol = lcol + (p / 2);
				
			}
			c.data.combinedColor = lightFunc::mixColors(lcol, bc.color, "mix", 0.5f); 
			c.data.combinedColor = lcol;
			Vector4f vc(c.data.combinedColor);
			debug("Our color ends up being " + vc.toStr());

			//finally, push our updated lightsink data back to the registry.
			e.updateComponent<c_lightSink>(c);
			//And let the scene know the scene is no longer clean.
			this->owner->setDirty();

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
