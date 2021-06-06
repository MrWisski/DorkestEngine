#include <Engine/World/MapSeg.h>
#include <Engine/Render/dorkestRenderer.h>
#include <Util/Math/Vector4.h>
#include <Engine/Engine.h>

MapSeg::MapSeg(AABB3f bounds, dorkestScene* parent)
{
	this->bBox = bounds;
	this->owner = parent;
	this->entReg = std::shared_ptr < entt::registry>();

}


MapSeg::~MapSeg()
{
}

void MapSeg::loadPrefab(std::string path)
{
	//TODO : Create loadPrefab - just load in an entity registry from disk.
	error("Feature not coded yet!");
}

std::shared_ptr<dorkestBaseEntity> MapSeg::createEntity()
{
	return std::make_unique<dorkestBaseEntity>(this->entReg);
}



std::shared_ptr<dorkestBaseEntity> MapSeg::createTerrainEntity(AABB3f bounds, std::string sprite)
{
	Vector2i screenLoc = this->owner->getCamera()->MapToScreen(bounds.getLocation());

	return std::make_shared<dorkestBaseEntity>((dorkestBaseEntity)TerrainEntity(this->entReg,bounds,screenLoc,sprite));
	
}

std::shared_ptr<dorkestBaseEntity> MapSeg::getEntity(entt::entity ent)
{
	if (entReg->valid(ent)) {
		return std::make_shared<dorkestBaseEntity>(entReg, ent);
	} else 
		return nullptr;
}

std::vector<std::shared_ptr<dorkestBaseEntity>> MapSeg::getImpacts(Ray<float> r)
{
	//TODO : uhm. This.
	return std::vector<std::shared_ptr<dorkestBaseEntity>>();
}

std::shared_ptr<dorkestBaseEntity> MapSeg::getImpact(Vector3f wPoint)
{
	//TODO : uhm. This too.
	return nullptr;
}

void MapSeg::updateLighting(std::vector<dorkestBaseEntity*> staticLights, std::vector<dorkestBaseEntity*> dynLights)
{
	auto sinkView = this->entReg->view<c_position, c_lightSink>();
	for (entt::entity en : sinkView) {
		//Create a DBE wrapper with the en entity handle.
		dorkestBaseEntity e(this->entReg, en);
		//OK, first we need to get the position, and lightsink component, for this entity.
		c_lightSink c = e.getComponent<c_lightSink>();
		c.data.combinedColor = olc::WHITE;
		//finally, push our updated lightsink data back to the registry.
		e.updateComponent<c_lightSink>(c);
		//And let the scene know the scene is no longer clean.
		this->owner->setDirty();

	}
}

/*
void MapSeg::updateLighting(std::vector<dorkestBaseEntity*> staticLights, std::vector<dorkestBaseEntity*> dynLights)
{
	//If we have no entities - SKIP!
	if (this->entReg->alive() == 0) { return; }

	//Create a view for each entity with a POSITION and a LIGHTSINK.
	auto sinkView = this->entReg->view<c_position,c_lightSink>();

	if (staticLights.size() != 0) {
		//We have static lights. 
		//Iterate over the sinkView
		for (entt::entity en : sinkView) {
			//Create a DBE wrapper with the en entity handle.
			dorkestBaseEntity e(this->entReg, en);
			//OK, first we need to get the position, and lightsink component, for this entity.
			c_position ep = e.getComponent<c_position>();
			c_lightSink c = e.getComponent<c_lightSink>();
			c_baseColor bc = e.getComponent<c_baseColor>();
			c.data.staticLights.clear();
			c.data.combinedColor = olc::BLANK;
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
				if (inten > 0.021f) {
					std::pair<float, olc::Pixel> lPair;
					lPair.first = inten;
					lPair.second = tl.data.color;
					c.data.staticLights.push_back(lPair);
				}
				ss << "Intensity = " << inten << el;
				debug(ss.str());
			}

			std::stringstream ss;
			ss << std::endl;
			olc::Pixel lcol = olc::BLACK;
			for (auto lpair : c.data.staticLights) {
					
				olc::Pixel p = lightFunc::mixColors(lcol, (lpair.second * lpair.first),"mix",0.5f);
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
*/

void MapSeg::sort()
{
	this->entReg->sort<c_position>([](const c_position& lhs, const c_position& rhs) {
		if (lhs.worldPos.y != rhs.worldPos.y)
			return lhs.worldPos.y < rhs.worldPos.y;
		if (lhs.worldPos.x != rhs.worldPos.x)
			return lhs.worldPos.x < rhs.worldPos.x;
		return lhs.worldPos.z < rhs.worldPos.z;
		});
}

void MapSeg::drawPlane() {
	return;
	for (int x = 0; x < 16; x++)
		for (int y = 0; y < 16; y++) {
			Vector3f poss = this->bBox.getLocation() + Vector3f(x, y, 0);
			debug("Creating cube at " + std::to_string(poss.x) + ", " + std::to_string(poss.y));
			AABB3f tbb(poss, Vector3f(1, 1, 1));
			std::shared_ptr<dorkestBaseEntity> entp = this->createTerrainEntity(tbb, "oPlane");

			c_baseColor cb = entp->getComponent<c_baseColor>();
			cb.color = olc::WHITE;
			entp->updateComponent<c_baseColor>(cb);

			if (entp == nullptr) { error("DBE is null!"); }
			

		}
	debug("DONE!");
}