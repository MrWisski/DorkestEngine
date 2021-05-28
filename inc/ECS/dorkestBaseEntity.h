#pragma once
#include <entt/entity/registry.hpp>
#include "../Util/Log.h"

class dorkestBaseEntity {
public:

	dorkestBaseEntity(entt::registry* reg) : m_reg(reg), m_handle(entt::null) { 
		if (m_reg == nullptr) {	error("Null pointer to registry passed to entity constructor!"); return;}
		m_handle = m_reg->create();

	}

	dorkestBaseEntity(entt::registry* reg, entt::entity ent) : m_reg(reg), m_handle(ent) {
		if (m_reg == nullptr) { error("Null pointer to registry passed to entity constructor!"); return; }
		if (m_handle == entt::null) { error("Null pointer to an entity passed to entity constructor!"); return; }
	}

	template<typename T, typename... Args>
	bool addComponent(Args&&... args)
	{
		if (m_reg == nullptr) { error("Null pointer to registry!"); return false; }
		if (m_handle == entt::null) { error("entity has not been initialized."); return false; }
		if (this->m_reg->any_of<T>(m_handle)) { error("Entity already has component!"); return false; }
		T& component = m_reg->emplace<T>(m_handle, std::forward<Args>(args)...);
		//m_Scene->OnComponentAdded<T>(*this, component);
		return true;
	}

	template<typename T>
	T* getComponent()
	{
		if (m_reg == nullptr) { error("Null pointer to registry!"); return nullptr; }
		//HZ_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
		T retval = m_reg->get<T>(m_handle);
		return &retval;
	}

	template<typename T>
	bool hasComponent()
	{
		if (m_reg == nullptr) { error("Null pointer to registry!"); return false; }
		return m_reg.has<T>(m_handle);
	}

	template<typename T>
	void removeComponent()
	{
		//HZ_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
		m_reg.remove<T>(m_handle);
	}

	entt::entity getHandle() { return m_handle; }

	operator bool() const { return m_handle != entt::null; }
	operator entt::entity() const { return m_handle; }
	operator uint32_t() const { return (uint32_t)m_handle; }

	bool operator==(const dorkestBaseEntity& other) const
	{
		return m_handle == other.m_handle && m_reg == other.m_reg;
	}

	bool operator!=(const dorkestBaseEntity& other) const
	{
		return !(*this == other);
	}

	bool isValid() { return m_reg != nullptr && m_handle != entt::null; }
	
private:
	entt::entity m_handle;
	entt::registry* m_reg{nullptr};

};