#pragma once

#include "../Util/Math/Vector3.h"
#include <olcPixelGameEngine.h>
#include "Util/Log.h"
#include <Util/Math/Geometry/AABB.h>
#include <World/Light.h>

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
	Vector2i screenPos = {0,0};
	bool init = false;

	c_position() = default;
	c_position(const c_position & pos) = default;
	c_position(const Vector3f& wPos, const Vector2i& sPos) : worldPos(wPos), screenPos(sPos) { init = true; }

	operator Vector3f () { return worldPos; }
	operator Vector3f& () { return worldPos; }
	operator const Vector3f() const { return worldPos; }
	operator const Vector3f& () const { return worldPos; }

	operator Vector2i () { return screenPos; }
	operator Vector2i& () { return screenPos; }
	operator const Vector2i& () const { return screenPos; }
};

/// <summary>
/// Dorkest Engine ECS Base Color Component
/// Stores the base color of an entity that includes it for rendering.
/// </summary>

struct c_baseColor {
public:
	olc::Pixel color;
	bool init = false;

	c_baseColor() = default;
	c_baseColor(const c_baseColor& color) = default;
	c_baseColor(const olc::Pixel& color) : color(color) { init = true; }

	operator olc::Pixel& () { return color; }
	operator const olc::Pixel& () const { return color; }

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
	c_sprite(const c_sprite & tag) = default;
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
	c_imposter(const c_imposter & tag) = default;
	c_imposter(olc::Decal* imposter) : imposter(imposter) { init = true; }
	~c_imposter() {if (imposter != nullptr) delete imposter;}

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
	struct FLAGS_A {
		bool isNew = true;
		bool needsImposter = false;
		bool UNUSED_2;
		bool UNUSED_3;
		bool UNUSED_4;
		bool UNUSED_5;
		bool UNUSED_6;
		bool UNUSED_7;
	};
	
	union {
		FLAGS_A flagBits;
		unsigned char FlagByte;
	} Bank_A;

	c_statusflags() = default;
	c_statusflags(const c_statusflags& tag) = default;

};

/// <summary>
/// AABB Component
/// Stores an AABB for the entity.
/// </summary>
struct c_aabb {
public:
	AABB<float> bBox;
	bool init = false;

	c_aabb() = default;
	c_aabb(const c_aabb& tag) = default;

	c_aabb(AABB<float> boundingBox) : bBox(boundingBox) { init = true; }
	~c_aabb() {}

	operator AABB<float> () { return bBox; }
	operator const AABB<float> () const { return bBox; }

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
/// A component indicating the entity emits light.
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
/// A component indicating the entity emits light.
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


 