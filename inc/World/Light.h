#pragma once
#include "Util/Math/Vector3.h"
#include <olcPixelGameEngine.h>


class LightSource {
public:
	olc::Pixel color;

	float constant = 0.2f;
	float linear = 0.4f;
	float quadratic = 0;

	float getIntensityFromDistance(float distance)
	{
		return 1 / (constant + linear * distance + quadratic * pow(distance, 2));
	}

	float getIntensityAtPosition(Vector3f pos, Vector3f lightPos)
	{
		return getIntensityFromDistance(pos.dist(lightPos));
	}
};

class LightSink {
public:
	std::vector<std::pair<float, olc::Pixel>> staticLights;
	olc::Pixel combinedColor = olc::BLACK;
};

class lightFunc {
public:
	static olc::Pixel mixColors(olc::Pixel color_a, olc::Pixel color_b, std::string mode = "mix", float t = -1.0f, float gamma = 2.2f) {
	
		assert(mode == "mix" || mode == "blend");
		
		if (t == -1.0f) {
			if (mode == "mix") t = 0.5f;
			else t = 0;
		}

		float tinv = 0.0001f;
		if (t <= 1) {
			tinv = 1 - t;
			t = std::max(0.0f, std::min(t, 1.0f));
		}
		else {

			float tinv = (int)(255 - t);
			t = std::max(0.0f, std::min(t, 1.0f));
			t = t * 255;
		}
		olc::Pixel ret;


		if (mode == "mix" && gamma > 0) {


			ret = (color_a * tinv) + (color_b * t);

			//r, g, b, a = (1 - t) * color_a + t * color_b
		}
		else if (mode == "mix" && gamma > 0 && gamma < 1) {
			float r, g, b, a;
			r = std::powf((tinv)*std::powf(color_a.r, gamma) + t * std::powf(color_b.r, gamma), 1 / gamma);
			g = std::powf((tinv)*std::powf(color_a.g, gamma) + t * std::powf(color_b.g, gamma), 1 / gamma);
			b = std::powf((tinv)*std::powf(color_a.b, gamma) + t * std::powf(color_b.b, gamma), 1 / gamma);
			a = (tinv)*color_a.a + t * color_b.a;
		}
		else if (mode == "mix" && gamma > 1) {
			ret = (color_a * (tinv)) + (color_b * t);
		}
		else if (mode == "blend") {
			/*
			float alpha_a = color_a.a * (tinv);
			float a = 1 - (1 - alpha_a) * (1 - color_b.a);
			float s = color_b.a * (1 - alpha_a) / a
				if gamma in(1., None) :
					r, g, b, _ = (1 - s) * color_a + s * color_b
					elif gamma > 0:
			r, g, b, _ = np.power((1 - s) * color_a * *gamma + s * color_b * *gamma,
				1 / gamma)
				*/
		}




		return ret;
	}
};