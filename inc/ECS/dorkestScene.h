#pragma once
#include "instPGE.h"
#include <entt/entity/registry.hpp>
#include "dorkestBaseEntity.h"
#include "Render/dorkestCamera.h"
#include "Util/dorkestProfiler.h"
#include <Util/Math/Geometry/AABB.h>

class dorkestRenderer;
class TerrainEntity;
class dorkestMap;
class MapSeg;

//The size that a mapseg initializes to (16x16x16)
const int SEGMENT_DIMENSION = 16;
//The max size mapseg map dimension (8x8)
const int MAX_MAPSEG_DIMENSION = 16;

//an 8x8 of 16x16x16 yields 4,000 meters per side.


class dorkestScene {
public:
	dorkestScene(std::string name);

	dorkestRenderer* getRenderer();
	void setCameraCenter(float x, float y, float z);
	dorkestCamera* getCamera();

	/// <summary>
	/// Forces the scene to recalculate lighting.
	/// </summary>
	void recalcLight();

	/// <summary>
	/// Does all the stuff you need to do to render a frame of the scene.
	/// </summary>
	/// <param name="fElapsedTime"> Time since last call </param>
	/// <returns> True if nothing bad happened, false if there was an error. </returns>
	bool doFrame(float fElapsedTime);

	/// <summary>
	/// Creates a new entity for this scene. Use createTerrain() to create suitable parts of a map.
	/// </summary>
	/// <returns>a dorkestBaseEntity pointer encapsulating the entt::entity handle</returns>
	dorkestBaseEntity* createNewEntity();

	/// <summary>
	/// Creates a new piece of terrain for this scene.
	/// </summary>
	/// <param name="bounds">The bounding box of the terrain (Restricted to 1x1x1 max!)</param>
	/// <returns>A TerrainEntity wrapper if successful, nullptr if not.</returns>
	std::shared_ptr<dorkestBaseEntity> createTerrain(AABB<float> bounds);

	/// <summary>
	/// retrieves a dorkestBaseEntity from the registry.
	/// </summary>
	/// <param name="ent">the entt handle to an entity.</param>
	/// <returns>the equavalent dorkestBaseEntity pointer.</returns>
	std::shared_ptr<dorkestBaseEntity> getEntity(entt::entity ent);

	/// <summary>
	/// Lets the scene know ENTT registry was mucked with, and needs to be updated.
	/// </summary>
	void setDirty();


	~dorkestScene();
private:
	entt::registry* sceneReg;
	std::string name = "INVALID";
	bool regClean = false;
	dorkestCamera* cam = nullptr;
	dorkestRenderer* r = nullptr;

	dorkestMap* map = nullptr;
	//std::vector<MapSeg*> map;
};