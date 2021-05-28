#pragma once
#include "dorkestBaseEntity.h"
#include "components.h"
class TerrainEntity : public dorkestBaseEntity {
public:

	TerrainEntity(entt::registry* reg, AABB<float> bounds, Vector2i screenLoc) : dorkestBaseEntity(reg) {
		this->addComponent<c_aabb>(bounds);
		Vector3f wl = bounds.getLocation();
		
		this->addComponent<c_position>(wl,screenLoc);
		this->addComponent<c_baseColor>(olc::WHITE);
		this->addComponent<c_sprite>("oCube");
		this->addComponent<c_imposter>();
		this->addComponent<c_statusflags>();
		this->addComponent<c_lightSink>();
	}

	TerrainEntity(TerrainEntity& o) = default;

};