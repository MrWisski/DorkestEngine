#include "ECS/Scene.h"
#include "Render/dorkestCamera.h"
#include "ECS/dorkestBaseEntity.h"
#include "ECS/components.h"
#include "instPGE.h"

bool Scene::doFrame(float fElapsedTime) {
	dorkestDataPoint dp("DoFrame");
	auto sceneView = sceneReg->view<c_position, c_baseColor, c_sprite>();

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
		for (auto [entity, pos, color, spritename] : sceneView.each()) {
			instPGE::getInstance()->engine->r->forcedColor = color;
			instPGE::getInstance()->engine->r->drawToScreen(pos.screenPos.x, pos.screenPos.y, spritename, Renderer::DIFFUSE, true);

		}
	}

	return true;
}


