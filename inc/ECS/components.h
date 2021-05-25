#pragma once

#include "../Util/Math/Vector3.h"
#include <olcPixelGameEngine.h>
#include "Util/Log.h"

struct c_nameTag {
	std::string pubName;

	c_nameTag() = default;
	c_nameTag(const c_nameTag& tag) = default;
	c_nameTag(const std::string& name): pubName(name) {}

	operator std::string& () { return pubName; }
	operator const std::string& () const { return pubName; }

};

struct c_position {
	Vector3f worldPos;
	Vector2i screenPos;

	c_position() = default;
	c_position(const c_position & pos) = default;
	c_position(const Vector3f & Wpos, const Vector2i & sPos) : worldPos(Wpos), screenPos(sPos) {}

	operator Vector3f& () { return worldPos; }
	operator const Vector3f& () const { return worldPos; }

};

struct c_baseColor {
	olc::Pixel color;

	c_baseColor() = default;
	c_baseColor(const c_baseColor& color) = default;
	c_baseColor(const olc::Pixel& color) : color(color) {}

	operator olc::Pixel& () { return color; }
	operator const olc::Pixel& () const { return color; }

};

struct c_sprite {
	std::string spriteName;

	c_sprite() = default;
	c_sprite(const c_sprite & tag) = default;
	c_sprite(const std::string & spriteName) : spriteName(spriteName) {}

	operator std::string& () { return spriteName; }
	operator const std::string& () const { return spriteName; }

};
