#pragma once
#include "instPGE.h"
#include <entt/entity/registry.hpp>
#include "dorkestBaseEntity.h"
#include "Render/dorkestCamera.h"
#include "Util/dorkestProfiler.h"

class dorkestRenderer;

class dorkestScene {
public:
	dorkestScene(std::string name);

	dorkestRenderer* getRenderer();
	void setCameraCenter(float x, float y, float z);
	dorkestCamera* getCamera();

	/// <summary>
	/// Does all the stuff you need to do to render a frame of the scene.
	/// </summary>
	/// <param name="fElapsedTime"> Time since last call </param>
	/// <returns> True if nothing bad happened, false if there was an error. </returns>
	bool doFrame(float fElapsedTime);

	/// <summary>
	/// Creates a new entity for this scene.
	/// </summary>
	/// <returns>a dorkestBaseEntity pointer encapsulating the entt::entity handle</returns>
	dorkestBaseEntity* createNewEntity();

	/// <summary>
	/// retrieves a dorkestBaseEntity from the registry.
	/// </summary>
	/// <param name="ent">the entt handle to an entity.</param>
	/// <returns>the equavalent dorkestBaseEntity pointer.</returns>
	dorkestBaseEntity* getEntity(entt::entity ent);

	~dorkestScene();
private:
	entt::registry* sceneReg;
	std::string name = "INVALID";
	bool regClean = false;
	dorkestCamera* cam = nullptr;
	dorkestRenderer* r = nullptr;
};