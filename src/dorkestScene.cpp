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
	delete sceneReg;
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

			//debugss << "Checking " << x << ", " << y << " (" << this->map->get(x, y)->bBox.toString() << ") contains " << bounds.toString();

			//debugss << "map contains bounds : " << (this->map->get(x, y)->bBox.contains(bounds) ? "True" : "False");
			//debugss << "bounds contains map : " << (bounds.contains(this->map->get(x, y)->bBox) ? "True" : "False");

			if (this->map->get(x, y)->bBox.contains(bounds)) {
				//We found one!

				regClean = false;
				assert(map != nullptr);
				auto seg = this->map->get(x, y);
				assert(seg != nullptr);

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
	sceneReg = new entt::registry();
	this->cam = std::make_shared<dorkestCamera>(instPGE::getInstance()->GetWindowSize().x, instPGE::getInstance()->GetWindowSize().y);
	this->r = std::make_shared<dorkestRenderer>(this->cam);

	this->r->setCam(cam);

	
	
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

	//LEFt SIDE
	r->drawTextRow(0, "Cam Center : " + r->cam->getCenter().toString(), 2, olc::RED, dorkestRenderer::LEFT);
	r->drawTextRow(1, "Cam zoom : " + std::to_string( r->cam->getZoom()), 2, olc::RED, dorkestRenderer::LEFT);
	r->drawTextRow(3, "sLights : " + std::to_string(sceneReg->size<c_staticLight>()),2, olc::YELLOW, dorkestRenderer::LEFT);
	r->drawTextRow(4, "dLights : " + std::to_string(sceneReg->size<c_dynamicLight>()),2, olc::YELLOW, dorkestRenderer::LEFT);
	
	//Right side
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
				entt::registry* mapreg = map->get(x, y)->entReg;
				//If this segment is NOT within the view frustrum, we need to skip.
				this->r->drawAABB3(map->get(x, y)->bBox);
				AABB3f box = map->get(x, y)->bBox;
				Vector3f lefte = Vector3f(box.getMax().x, box.getMin().y, 0);
				Vector3f righte = Vector3f(box.getMin().x, box.getMax().y, 0);
				Vert vl, vr, vt, vb;
				vl.pos = lefte; vl.col = olc::RED;
				vr.pos = righte; vr.col = olc::GREEN;
				vt.pos = Vector3f(box.getMin().x, box.getMin().y, box.getMax().z); vt.col = olc::BLUE;
				vb.pos = Vector3f(box.getMax().x, box.getMax().y,0); vb.col = olc::BLACK;
				r->drawVertex(vl);
				r->drawVertex(vr);
				r->drawVertex(vt);
				r->drawVertex(vb);



				r->drawTextRow(2, "BB min: " + map->get(x, y)->bBox.getMin().toString(), 2, olc::WHITE, dorkestRenderer::RIGHT);
				r->drawTextRow(3, "BB max: " + map->get(x, y)->bBox.getMax().toString(), 2, olc::WHITE, dorkestRenderer::RIGHT);
				Vector2f SBoxm = cam->iTOc({ 32.0f,16.0f }, cam->getZoom(), map->get(x, y)->bBox.getMin(), { 0,0 });
				Vector2f SBoxM = cam->iTOc({ 32.0f,16.0f }, cam->getZoom(), map->get(x, y)->bBox.getMax(), { 0,0 });
				r->drawTextRow(4, "BB Xlated min : " + SBoxm.toStr(), 2, olc::WHITE, dorkestRenderer::RIGHT);
				r->drawTextRow(5, "BB Xlated max : " + SBoxM.toStr(), 2, olc::WHITE, dorkestRenderer::RIGHT);

				if (!cam->canSee(map->get(x, y)->bBox)) {
					debugss << "FRUSTRUM CULLED SEG AT X : " << x << ", " << y;
					//debugss << "Mapseg BB : " << map->get(x, y)->bBox.toString();
					//debugss << "cam BB : " << cam->getBB().toString();
					
			
					
					continue;
				}

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


