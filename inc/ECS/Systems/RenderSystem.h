#pragma once
#include <entt/entity/registry.hpp>
#include "../components.h"

class ECS_RenderSys {
	entt::registry* reg;

	ECS_RenderSys(entt::registry* reg) { this->reg = reg; }
};