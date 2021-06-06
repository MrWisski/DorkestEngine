#include "ECS/dorkestScene.h"
#include <Engine/Render/dorkestCamera.h>
#include "ECS/dorkestBaseEntity.h"
#include "ECS/components.h"
#include "instPGE.h"
#include <Engine/Render/dorkestRenderer.h>
#include <Engine/World/dorkestMap.h>
#include <Engine/World/MapSeg.h>

std::shared_ptr<dorkestRenderer> dorkestScene::getRenderer() { return this->r; }
void dorkestScene::setCameraCenter(float x, float y, float z) { this->getCamera()->setCenter(x, y, z); }
std::shared_ptr < dorkestCamera> dorkestScene::getCamera() { return this->cam; }

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

void dorkestScene::setupMap()
{
	auto seg = map->get(MAX_MAPSEG_DIMENSION / 2, MAX_MAPSEG_DIMENSION / 2);

	Vector2f tm = this->cam->MapToScreen(seg->bBox.getLocation() + (SEGMENT_DIMENSION / 2));
	debug(tm.toString());
	cam->setCenter(tm.x,tm.y, 0);
	
	

	this->regClean = false;
}

dorkestScene::~dorkestScene() {
	debug("Disposing of scene " + name);
	
	if (cam != nullptr) cam.reset();
	if (r != nullptr) r.reset(); 
	if (this->map != nullptr) map.reset();

	sceneReg->clear();
	sceneReg.reset();
}
dorkestBaseEntity* dorkestScene::createNewEntity() {

	dorkestBaseEntity* newE = new dorkestBaseEntity(this->sceneReg);
	regClean = false;
	return newE;
}

std::shared_ptr<dorkestBaseEntity> dorkestScene::createTerrain(AABB3f bounds, std::string sprite)
{
	debug("Checking who owns the area of these bounds....");
	//Check to see if we have any mapsegs that can take this block.
	for (int x = 0; x < MAX_MAPSEG_DIMENSION; x++)
		for (int y = 0; y < MAX_MAPSEG_DIMENSION; y++) {
			assert(this->map != nullptr);

			if (this->map->get(x, y) == nullptr) { error("map seg is null"); continue; }
			if (this->map->get(x, y)->bBox.contains(bounds)) {
				//We found one!

				regClean = false;
				auto seg = this->map->get(x, y);

				return seg->createTerrainEntity(bounds,sprite);
			}
		}

	return nullptr;
}

std::shared_ptr<dorkestBaseEntity> dorkestScene::getEntity(entt::entity ent) {
	return std::make_shared<dorkestBaseEntity>(this->sceneReg, ent);
}


dorkestScene::dorkestScene(std::string name,Engine* e) {
	this->name = name;
	sceneReg = std::make_unique<entt::registry>();
	this->cam = std::make_shared<dorkestCamera>();
	this->r = std::make_shared<dorkestRenderer>(this->cam);

	this->r->setCam(cam);
	getCamera()->setScreenSize(instPGE::getInstance()->GetWindowSize().x, instPGE::getInstance()->GetWindowSize().y);
	
	getCamera()->setOrthogonal(instPGE::getInstance()->GetWindowSize().x, instPGE::getInstance()->GetWindowSize().y, 0, 1);
	
	
	map = std::make_unique <dorkestMap>(this);
	auto seg = map->get(MAX_MAPSEG_DIMENSION / 2, MAX_MAPSEG_DIMENSION / 2);
	getCamera()->setCenter(seg->bBox.getLocation().x + SEGMENT_DIMENSION/2, seg->bBox.getLocation().y + SEGMENT_DIMENSION / 2, 0);

	debug("Scene " + name + " is ready for use!");
}

bool dorkestScene::doFrame(float fElapsedTime) {
	dorkestDataPoint dp("DoFrame");

	//-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=
	//+	Initialize
	//-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=

	auto sceneView = sceneReg->view<c_position, c_baseColor, c_sprite, c_imposter, c_statusflags>();

	//TODO : This should be a variable gotten from ENGINE.
	Vector3f mMap = cam->ScreenToMap(instPGE::getInstance()->GetMousePos());

	//Set up our debug HUD stuff.
	//TODO : Move this somewhere sensible, PLEASE
	r->drawTextRow(0, "Cam Center : " + r->cam->getCenter().toString(), 2, olc::RED, dorkestRenderer::LEFT);
	r->drawTextRow(2, "sLights : " + std::to_string(sceneReg->size<c_staticLight>()),2, olc::YELLOW, dorkestRenderer::LEFT);
	r->drawTextRow(3, "dLights : " + std::to_string(sceneReg->size<c_dynamicLight>()),2, olc::YELLOW, dorkestRenderer::LEFT);
	r->drawTextRow(0, "Mouse (Map) : " + mMap.toString(), 2, Colorf(olc::BLUE), dorkestRenderer::RIGHT);
	r->drawTextRow(1, "Mouse (Screen) : " + instPGE::getInstance()->GetMousePos().str(),2, olc::BLUE, dorkestRenderer::RIGHT);
	
	//-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=
	//+	Sort our scene Entities
	//-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=

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

	//-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=
	//+	Render the scene map.
	//-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=-=[+]=

	if (true) {
		dorkestDataPoint dp("RenderSceneMap");
		for (int x = 0; x < MAX_MAPSEG_DIMENSION; x++)
			for (int y = 0; y < MAX_MAPSEG_DIMENSION; y++) {
				if (map->get(x, y) == nullptr) { continue; }
				//get the entity registry for the segment.
				std::shared_ptr < entt::registry> mapreg = map->get(x, y)->entReg;
				//create a view.
				//TODO : see if we can move this to a pregen variable inside the segment, maybe.

				if (mapreg != nullptr && mapreg->alive() != 0) {
					auto segView = mapreg->view<c_position, c_aabb3, c_baseColor, c_sprite, c_imposter, c_statusflags, c_lightSink>();
					if (segView.begin() != segView.end())
						for (auto [entity, pos, bounds, color, spritename, imposter, flags, lightsink] : segView.each()) {
							//Do we draw the sprite from spritename, or the imposter from imposter?
							//Prefer the imposter if it has one.
							if (imposter.imposter != nullptr) {
								debug("IMPOSTER RENDERING NOT IMPLEMENTED!!!");
								//r->drawDecal(r->MapToScreen({ 32,32 }, 1, bounds.bBox.getLocation(), { 0,0 }), imposter.decalsize, imposter.imposter, { 0,0 }, imposter.decalsize, color);
							}
							else {

								//r->setForcedColor(p);
								//r->setForcedColor(blendColor((color.color * lightsink.data.intensity), lightsink.data.combinedColor, 0.9f));
								r->drawSprite(bounds.bBox.getLocation(), spritename, dorkestRenderer::DIFFUSE);
							}
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
				error("IMPOSTER RENDERING NOT IMPLEMENTED!");
				//r->drawSprite(imposter., 1, pos.worldPos, { 0,0 }), imposter.decalsize, imposter.imposter, { 0,0 }, imposter.decalsize, color);
			}
			else {
				r->drawSprite(pos.worldPos, spritename, dorkestRenderer::DIFFUSE, color.color);
			}


		}
	}

	return true;
}


