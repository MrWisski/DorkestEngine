#pragma once
#include <entt/entity/registry.hpp>
#include "Util/Math/Vector3.h"
#include "Util/Math/Vector2.h"
#include "Util/Math/Geometry/AABB.h"
#include "Util/Math/Geometry/Ray.h"
#include "ECS/dorkestScene.h"
#include "ECS/dorkestBaseEntity.h"
#include <ECS/TerrainEntity.h>

/// <summary>
/// Just a container for segments of a dorkestScene
/// </summary>
class MapSeg {
public:

	MapSeg(AABB<float> bounds, dorkestScene* parent);
	~MapSeg();

	/// <summary>
	/// Loads a stored map segment from disk.
	/// </summary>
	/// <param name="path">String containing the path to the file to read.</param>
	void loadPrefab(std::string path);
	
	/// <summary>
	/// Creates a generic entity for the scene with no components.
	/// </summary>
	/// <returns>An empty basic entity wrapper</returns>
	dorkestBaseEntity* createEntity();

	TerrainEntity* createTerrainEntity(AABB<float> bounds);

	dorkestBaseEntity* getEntity(entt::entity ent);

	std::vector<dorkestBaseEntity*> getImpacts(Ray<float> r);
	dorkestBaseEntity* getImpact(Vector3f wPoint);

	void updateStaticLighting(std::vector<dorkestBaseEntity*> staticLights);
	void updateDynamicLighting(std::vector<dorkestBaseEntity*> dynLights);

	//Let the scene access
	friend class dorkestScene;
protected:
	AABB<float> bBox;
	entt::registry* entities;
	
	std::vector<dorkestBaseEntity*> entVec;

	dorkestScene* owner;
};