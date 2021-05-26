#include "ECS/dorkestScene.h"
#include "Render/dorkestCamera.h"
#include "ECS/dorkestBaseEntity.h"
#include "ECS/components.h"
#include "instPGE.h"
#include "Render/dorkestRenderer.h"

dorkestRenderer* dorkestScene::getRenderer() { return this->r; }
void dorkestScene::setCameraCenter(float x, float y, float z) { this->r->getCamera()->setCenter(x, y, z); }
dorkestCamera* dorkestScene::getCamera() { return this->r->getCamera(); }

dorkestScene::~dorkestScene(){
	debug("Disposing of scene " + name);
	sceneReg->clear();
	delete sceneReg;
	if (cam != nullptr) { delete cam; }
	if (r != nullptr) { delete r; }
}
dorkestBaseEntity* dorkestScene::createNewEntity() {

	dorkestBaseEntity* newE = new dorkestBaseEntity(this->sceneReg);
	regClean = false;
	return newE;
}

dorkestBaseEntity* dorkestScene::getEntity(entt::entity ent) {
	return new dorkestBaseEntity(this->sceneReg, ent);
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
		dorkestDataPoint dp("RenderEntities");
		for (auto [entity, pos, color, spritename, imposter, flags] : sceneView.each()) {
			if (flags.Bank_A.flagBits.needsImposter) {
				//some code to render the imposter.
			}
			//Do we draw the sprite from spritename, or the imposter from imposter?
			//Prefer the imposter if it has one.
			if (imposter.imposter != nullptr) {
				debug("rendering imposter.");
				r->drawDecal(r->MapToScreen({ 32,32 }, 1, pos.worldPos, { 0,0 }), imposter.decalsize, imposter.imposter, { 0,0 },imposter.decalsize,color);
			} else {
				r->setForcedColor(color);
				r->drawToWorld(pos.worldPos.x, pos.worldPos.y, pos.worldPos.z, spritename, dorkestRenderer::DIFFUSE, true);
			}
	

		}
	}

	return true;
}


