#pragma once
#include <entt/entity/registry.hpp>
#include "dorkestBaseEntity.h"
#include "../Util/dorkestProfiler.h"

class Scene {
public:
	Scene() { sceneReg = new entt::registry(); }
	~Scene() { 
		sceneReg->clear();
		delete sceneReg; 
	}

	bool doFrame(float fElapsedTime);
	dorkestBaseEntity* createNewEntity()
	{
		dorkestProfile dp("CreateNewEntity");
		dorkestBaseEntity* newE = new dorkestBaseEntity(this->sceneReg);
		regClean = false;
		return newE;
	};

private:
	entt::registry* sceneReg;
	bool regClean = false;
};