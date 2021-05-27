#pragma once
#include "dorkestBaseEntity.h"
#include "components.h"
class TerrainEntity : public dorkestBaseEntity {

	TerrainEntity(entt::registry* reg, AABB<float> bounds) : dorkestBaseEntity(reg) {
		this->addComponent<c_aabb>(bounds);
		this->addComponent<c_baseColor>(olc::WHITE);
		this->addComponent<c_sprite>("oBox");
		this->addComponent<c_imposter>();
		this->addComponent<c_statusflags>();
	}

	TerrainEntity(TerrainEntity& o) = default;

};