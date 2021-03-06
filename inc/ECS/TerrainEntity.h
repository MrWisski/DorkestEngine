#pragma once
#include "dorkestBaseEntity.h"
#include "components.h"
class TerrainEntity : public dorkestBaseEntity {
public:

	TerrainEntity(entt::registry* reg, AABB3f bounds, Vector2i screenLoc, std::string sprite = "oCube") : dorkestBaseEntity(reg) {
		assert(reg != nullptr);

		this->addComponent<c_aabb3>(bounds);
		Vector3f wl = bounds.getLocation();

		this->addComponent<c_position>(wl, screenLoc);
		this->addComponent<c_baseColor>(Colorf(0, 0, 1, 1));
		this->addComponent<c_sprite>(sprite);
		//this->addComponent<c_imposter>();
		this->addComponent<c_statusflags>();
		this->addComponent<c_lightSink>();
	}

	TerrainEntity(TerrainEntity& o) = default;

	~TerrainEntity() {}

};