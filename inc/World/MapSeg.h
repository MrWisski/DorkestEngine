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
	/// <returns>An empty basic entity wrapper. This segment controls the pointer!</returns>
	dorkestBaseEntity* createEntity();

	/// <summary>
	/// Creates a new TerrainEntity (prepopulated with all the components it needs to be terrain.
	/// </summary>
	/// <param name="bounds">The bounding box for the desired entity.</param>
	/// <returns>A pointer to a new terrain entity. This segment controls the pointer!</returns>
	TerrainEntity* createTerrainEntity(AABB<float> bounds);

	/// <summary>
	/// Returns a dorkestBaseEntity wrapper for an ENTT handle.
	/// </summary>
	/// <param name="ent">The entity handle you want</param>
	/// <returns>A pointer to the wrapped handle, or nullptr if no entity registered with that handle. This segment controls the pointer!</returns>
	dorkestBaseEntity* getEntity(entt::entity ent);

	/// <summary>
	/// Traces a ray through the mapseg, and returns the entities with AABBs that get hit.
	/// </summary>
	/// <param name="r">The ray to cast</param>
	/// <returns>a std::vector containing all the entities hit along the way. The segment controls all the pointers!</returns>
	std::vector<dorkestBaseEntity*> getImpacts(Ray<float> r);

	/// <summary>
	/// Checks what AABB equipped entities would get hit at a certain point.
	/// </summary>
	/// <param name="wPoint">a certain point</param>
	/// <returns>what AABB equipped entity got hit, otherwise nullptr.</returns>
	dorkestBaseEntity* getImpact(Vector3f wPoint);

	/// <summary>
	/// Calculates or recalculates the lighting in the mapseg.
	/// </summary>
	/// <param name="staticLights"> A std::vector containing a list of c_staticLight equipped components in the scene.</param>
	/// <param name="dynLights"> A std::vector containing a list of c_dynamicLight equipped components in the scene.</param>
	void updateLighting(std::vector<dorkestBaseEntity*> staticLights, std::vector<dorkestBaseEntity*> dynLights);

	/// <summary>
	/// Sort all entities in this segment according to their world pos (for rendering)
	/// </summary>
	void sort();

	//Let the scene access
	friend class dorkestScene;
protected:
	AABB<float> bBox;
	entt::registry* entities;
	
	std::vector<dorkestBaseEntity*> entVec;

	dorkestScene* owner;
};