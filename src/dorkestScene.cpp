#include "ECS/dorkestScene.h"
#include "Render/dorkestCamera.h"
#include "ECS/dorkestBaseEntity.h"
#include "ECS/components.h"
#include "instPGE.h"
#include "Render/dorkestRenderer.h"
#include <World/dorkestMap.h>
#include <World/MapSeg.h>

dorkestRenderer* dorkestScene::getRenderer() { return this->r; }
void dorkestScene::setCameraCenter(float x, float y, float z) { this->r->getCamera()->setCenter(x, y, z); }
dorkestCamera* dorkestScene::getCamera() { return this->r->getCamera(); }

void dorkestScene::recalcLight()
{
	std::vector<dorkestBaseEntity*> slvec;
	std::vector<dorkestBaseEntity*> dlvec;

	//Get a view with all entities with a static light component.
	auto staticLightView = sceneReg->view<c_staticLight>();
	//Iterate over the view, create a DBE of each, and stash it in a vector.
	for (entt::entity e : staticLightView) {
		dorkestBaseEntity* ebe = new dorkestBaseEntity(this->sceneReg, e);
		slvec.push_back(ebe);
	}

	//Get a view with all entities with a dynamic light component.
	auto dynLightView = sceneReg->view<c_dynamicLight>();
	//Iterate over the view, create a DBE of each, and stash it in a vector.
	for (entt::entity e : dynLightView) {
		dorkestBaseEntity* ebe = new dorkestBaseEntity(this->sceneReg, e);
		dlvec.push_back(ebe);
	}

	//TODO : Optimize this naive mess, once its actually working.

	for (int x = 0; x < MAX_MAPSEG_DIMENSION; x++)
		for (int y = 0; y < MAX_MAPSEG_DIMENSION; y++) {
			this->map->get(x, y)->updateLighting(slvec, dlvec);
		}

}

void dorkestScene::setDirty()
{
	this->regClean = false;
}

dorkestScene::~dorkestScene() {
	debug("Disposing of scene " + name);
	
	if (cam != nullptr) { delete cam; }
	if (r != nullptr) { delete r; }
	if (this->map != nullptr) delete map;

	sceneReg->clear();
	delete sceneReg;
}
dorkestBaseEntity* dorkestScene::createNewEntity() {

	dorkestBaseEntity* newE = new dorkestBaseEntity(this->sceneReg);
	regClean = false;
	return newE;
}

std::shared_ptr<dorkestBaseEntity> dorkestScene::createTerrain(AABB<float> bounds)
{
	//Check to see if we have any mapsegs that can take this block.
	for (int x = 0; x < MAX_MAPSEG_DIMENSION; x++)
		for (int y = 0; y < MAX_MAPSEG_DIMENSION; y++) {

			if (this->map->get(x, y)->bBox.contains(bounds)) {
				//We found one!

				regClean = false;
				return this->map->get(x, y)->createTerrainEntity(bounds);
			}
		}

	return nullptr;
}

std::shared_ptr<dorkestBaseEntity> dorkestScene::getEntity(entt::entity ent) {
	return std::make_shared<dorkestBaseEntity>(this->sceneReg, ent);
}


dorkestScene::dorkestScene(std::string name) {
	this->name = name;
	sceneReg = new entt::registry();
	this->cam = new dorkestCamera();
	this->r = new dorkestRenderer();
	this->r->setCam(cam);
	r->getCamera()->setScreenSize(instPGE::getInstance()->GetWindowSize().x, instPGE::getInstance()->GetWindowSize().y);
	r->getCamera()->setCenter(cam->getScreenWidth() / 2.0f, cam->getScreenHeight() / 2.0f, 0);
	r->getCamera()->setOrthogonal(instPGE::getInstance()->GetWindowSize().x, instPGE::getInstance()->GetWindowSize().y, 0, 1);

	map = new dorkestMap(this);

	debug("Scene " + name + " is ready for use!");
}

bool dorkestScene::doFrame(float fElapsedTime) {
	dorkestDataPoint dp("DoFrame");
	auto sceneView = sceneReg->view<c_position, c_baseColor, c_sprite, c_imposter, c_statusflags>();

	//Do we need to sort the entities?
	if (!this->regClean) {
		dorkestDataPoint dp("SortEntities");
		debug("Sorting the scene registry.");
		sceneReg->sort<c_position>([](const c_position& lhs, const c_position& rhs) {
			if (lhs.worldPos.y != rhs.worldPos.y)
				return lhs.worldPos.y < rhs.worldPos.y;
			if (lhs.worldPos.x != rhs.worldPos.x)
				return lhs.worldPos.x < rhs.worldPos.x;
			return lhs.worldPos.z < rhs.worldPos.z;
			});
		regClean = true;
	}

	if (true) {
		dorkestDataPoint dp("RenderSceneMap");
		for (int x = 0; x < MAX_MAPSEG_DIMENSION; x++)
			for (int y = 0; y < MAX_MAPSEG_DIMENSION; y++) {
				if (map->get(x, y) == nullptr) { continue; }
				//get the entity registry for the segment.
				entt::registry* mapreg = map->get(x, y)->entities;
				//create a view.
				//TODO : see if we can move this to a pregen variable inside the segment, maybe.

				auto segView = mapreg->view<c_position, c_aabb, c_baseColor, c_sprite, c_imposter, c_statusflags, c_lightSink>();
				for (auto [entity, pos, bounds, color, spritename, imposter, flags, lightsink] : segView.each()) {
					//Do we draw the sprite from spritename, or the imposter from imposter?
					//Prefer the imposter if it has one.
					if (imposter.imposter != nullptr) {
						debug("rendering imposter.");
						r->drawDecal(r->MapToScreen({ 32,32 }, 1, bounds.bBox.getLocation(), { 0,0 }), imposter.decalsize, imposter.imposter, { 0,0 }, imposter.decalsize, color);
					}
					else {
						//debug("lightsink data intensity = " + std::to_string(lightsink.data.intensity));
						r->setForcedColor(lightsink.data.combinedColor);
						
						//r->setForcedColor(p);
						//r->setForcedColor(blendColor((color.color * lightsink.data.intensity), lightsink.data.combinedColor, 0.9f));
						r->drawToWorld(bounds.bBox.getLocation().x, bounds.bBox.getLocation().y, bounds.bBox.getLocation().z, spritename, dorkestRenderer::DIFFUSE, true);
					}
				}

			}

	}


	if (true) {
		dorkestDataPoint dp("RenderSceneEntities");
		for (auto [entity, pos, color, spritename, imposter, flags] : sceneView.each()) {

			//Do we draw the sprite from spritename, or the imposter from imposter?
			//Prefer the imposter if it has one.
			if (imposter.imposter != nullptr) {
				debug("rendering imposter.");
				r->drawDecal(r->MapToScreen({ 32,32 }, 1, pos.worldPos, { 0,0 }), imposter.decalsize, imposter.imposter, { 0,0 }, imposter.decalsize, color);
			}
			else {
				r->setForcedColor(color);
				r->drawToWorld(pos.worldPos.x, pos.worldPos.y, pos.worldPos.z, spritename, dorkestRenderer::DIFFUSE, true);
			}


		}
	}

	return true;
}


