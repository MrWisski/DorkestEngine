#pragma once

#include "../Util/Math/Vector3.h"
#include <olcPixelGameEngine.h>
#include "Util/Log.h"
#include <Util/Math/Geometry/AABB.h>
#include <Engine/World/Light.h>
#include <Util/Color.h>

/// <summary>
/// Dorkest Engine ECS Nametag Component
/// Stores a text name of an entity that includes it.
/// </summary>

struct c_nameTag {
public:
	std::string pubName = "";
	bool init = false;

	c_nameTag() = default;
	c_nameTag(const c_nameTag& tag) = default;
	c_nameTag(const std::string& name) : pubName(name) { init = true; }

	operator std::string& () { return pubName; }
	operator const std::string& () const { return pubName; }

};

/// <summary>
/// Dorkest Engine ECS Position Component
/// Stores the world and screen position of an entity that includes it.
/// </summary>
struct c_position {
public:
	Vector3f worldPos = { 0,0,0 };
	Vector2i screenPos = { 0,0 };
	bool init = false;

	c_position() = default;
	c_position(const c_position& pos) = default;
	c_position(const Vector3f& wPos, const Vector2i& sPos) : worldPos(wPos), screenPos(sPos) { init = true; }

	operator Vector3f () { return worldPos; }
	operator Vector3f& () { return worldPos; }
	operator const Vector3f() const { return worldPos; }
	operator const Vector3f& () const { return worldPos; }

	operator Vector2i () { return screenPos; }
	operator Vector2i& () { return screenPos; }
	operator const Vector2i& () const { return screenPos; }

	bool operator < (c_position rhs) {
		if (screenPos.y == rhs.screenPos.y)
			if (screenPos.x == rhs.screenPos.x)
				if (worldPos.y == rhs.worldPos.y)
					return worldPos.z < rhs.worldPos.z;
				else return worldPos.y < rhs.worldPos.y;
			else return screenPos.x < rhs.screenPos.x;
		else return screenPos.y < rhs.screenPos.y;
	}
};

/// <summary>
/// Dorkest Engine ECS Base Color Component
/// Stores the base color of an entity that includes it for rendering.
/// </summary>
struct c_baseColor {
public:
	Colorf color;
	bool init = false;

	c_baseColor() = default;
	c_baseColor(const c_baseColor& color) = default;
	c_baseColor(const Colorf& color) : color(color) { init = true; }

	operator Colorf& () { return color; }
	operator const Colorf& () const { return color; }

};

/// <summary>
/// Dorkest Engine ECS Sprite Component
/// Stores the name of a sprite to use in rendering, for an entity that includes it.
/// </summary>

struct c_sprite {
public:
	std::string spriteName;
	bool init = false;

	c_sprite() = default;
	c_sprite(const c_sprite& tag) = default;
	c_sprite(const std::string& spriteName) : spriteName(spriteName) { init = true; }

	operator std::string& () { return spriteName; }
	operator const std::string& () const { return spriteName; }

};

/// <summary>
/// Imposter Component
/// Stores a pre-rendered image of an entity that includes it. Controls the pointer to the imposter decal.
/// </summary>
struct c_imposter {
public:
	olc::Decal* imposter = nullptr;
	Vector2i decalsize = { 0,0 };
	bool init = false;

	c_imposter() = default;
	c_imposter(const c_imposter& tag) = default;
	c_imposter(olc::Decal* imposter) : imposter(imposter) { init = true; }
	~c_imposter() { if (imposter != nullptr) delete imposter; }

	operator olc::Decal* () { return imposter; }
	operator const olc::Decal* () const { return imposter; }

};

/// <summary>
/// Status Component
/// Stores status flags from the entities it is attached to.
/// </summary>
struct c_statusflags {
public:
	bool init = true;
	bool isNew = true;
	bool needsImposter = false;

	c_statusflags() = default;
	c_statusflags(const c_statusflags& tag) = default;

};

/// <summary>
/// 3D AABB Component
/// Stores a 3d AABB for the entity.
/// </summary>
struct c_aabb3 {
public:
	AABB3f bBox;
	bool init = false;

	c_aabb3() = default;
	c_aabb3(const c_aabb3& tag) = default;

	c_aabb3(AABB3f boundingBox) : bBox(boundingBox) { init = true; }
	~c_aabb3() {}

	operator AABB3f () { return bBox; }
	operator const AABB3f() const { return bBox; }

};

/// <summary>
/// 2D AABB Component
/// Stores a 2D AABB for the entity.
/// </summary>
struct c_aabb2 {
public:
	AABB2f bBox;
	bool init = false;

	c_aabb2() = default;
	c_aabb2(const c_aabb2& tag) = default;

	c_aabb2(AABB2f boundingBox) : bBox(boundingBox) { init = true; }
	~c_aabb2() {}

	operator AABB2f () { return bBox; }
	operator const AABB2f() const { return bBox; }

};

class BlankData {};

/// <summary>
/// 
/// </summary>
struct c_blank {
public:
	BlankData data;
	bool init = false;

	c_blank() = default;
	c_blank(const c_blank& data) = default;

	c_blank(const BlankData& data) : data(data) { init = true; };
	~c_blank() = default;

	operator BlankData() { return data; }
	operator const BlankData() const { return data; }
};

/// <summary>
/// A component indicating the entity emits light, but never changes.
/// </summary>
struct c_staticLight {
public:
	LightSource data;
	bool init = false;

	c_staticLight() = default;
	c_staticLight(const c_staticLight& data) = default;

	c_staticLight(const LightSource& data) : data(data) { init = true; };
	~c_staticLight() = default;

	operator LightSource() { return data; }
	operator const LightSource() const { return data; }
};

/// <summary>
/// A component indicating the entity emits light, and that source might change at any time.
/// </summary>
struct c_dynamicLight {
public:
	LightSource data;
	bool init = false;

	c_dynamicLight() = default;
	c_dynamicLight(const c_dynamicLight& data) = default;

	c_dynamicLight(const LightSource& data) : data(data) { init = true; };
	~c_dynamicLight() = default;

	operator LightSource() { return data; }
	operator const LightSource() const { return data; }
};

/// <summary>
/// A component that accumulates incoming light sources
/// </summary>
struct c_lightSink {
public:
	LightSink data;
	bool init = false;

	c_lightSink() = default;
	c_lightSink(const c_lightSink& data) = default;

	c_lightSink(const LightSink& data) : data(data) { init = true; };
	~c_lightSink() = default;

	operator LightSink() { return data; }
	operator const LightSink() const { return data; }
};


