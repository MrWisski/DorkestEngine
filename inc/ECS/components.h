#pragma once

#include "../Util/Math/Vector3.h"
#include <olcPixelGameEngine.h>
#include "Util/Log.h"

/// <summary>
/// Dorkest Engine ECS Nametag Component
/// Stores a text name of an entity that includes it.
/// </summary>

struct c_nameTag {
	std::string pubName;

	c_nameTag() = default;
	c_nameTag(const c_nameTag& tag) = default;
	c_nameTag(const std::string& name): pubName(name) {}

	operator std::string& () { return pubName; }
	operator const std::string& () const { return pubName; }

};

/// <summary>
/// Dorkest Engine ECS Position Component
/// Stores the world and screen position of an entity that includes it.
/// </summary>
struct c_position {
	Vector3f worldPos = { 0,0,0 };
	Vector2i screenPos = {0,0};

	c_position() = default;
	c_position(const c_position & pos) = default;
	c_position(const Vector3f & wPos, const Vector2i & sPos) : worldPos(wPos), screenPos(sPos) {}

	operator Vector3f& () { return worldPos; }
	operator const Vector3f& () const { return worldPos; }
	operator Vector2i& () { return screenPos; }
	operator const Vector2i& () const { return screenPos; }
};

/// <summary>
/// Dorkest Engine ECS Base Color Component
/// Stores the base color of an entity that includes it for rendering.
/// </summary>

struct c_baseColor {
	olc::Pixel color;

	c_baseColor() = default;
	c_baseColor(const c_baseColor& color) = default;
	c_baseColor(const olc::Pixel& color) : color(color) {}

	operator olc::Pixel& () { return color; }
	operator const olc::Pixel& () const { return color; }

};

/// <summary>
/// Dorkest Engine ECS Sprite Component
/// Stores the name of a sprite to use in rendering, for an entity that includes it.
/// </summary>

struct c_sprite {
	std::string spriteName;

	c_sprite() = default;
	c_sprite(const c_sprite & tag) = default;
	c_sprite(const std::string & spriteName) : spriteName(spriteName) {}

	operator std::string& () { return spriteName; }
	operator const std::string& () const { return spriteName; }

};

/// <summary>
/// Imposter Component
/// Stores a pre-rendered image of an entity that includes it. Controls the pointer to the imposter decal.
/// </summary>
struct c_imposter {
	olc::Decal* imposter = nullptr;
	Vector2i decalsize = { 0,0 };

	c_imposter() = default;
	c_imposter(const c_imposter & tag) = default;
	c_imposter(olc::Decal* imposter) : imposter(imposter) {}
	~c_imposter() {if (imposter != nullptr) delete imposter;}

	operator olc::Decal* () { return imposter; }
	operator const olc::Decal* () const { return imposter; }

};

/// <summary>
/// Status Component
/// Stores status flags from the entities it is attached to.
/// </summary>
struct c_statusflags {
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
 