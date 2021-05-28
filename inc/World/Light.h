#pragma once
#include "Util/Math/Vector3.h"
#include <olcPixelGameEngine.h>

class LightSource {
public:
	olc::Pixel color;

	float constant;
	float linear;
	float quadratic;
		
};

class LightSink {
public:
	float intensity = 0.0f;
	olc::Pixel combinedColor = olc::BLACK;
};