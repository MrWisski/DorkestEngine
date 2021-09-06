#pragma once
#include "Util/Math/Vector3.h"
#include <Util/Color.h>

#include <algorithm>
#include <math.h>


class LightSource {
public:
	Colorf color;

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
	std::vector<std::pair<float, Colorf>> staticLights;
	Colorf combinedColor = Colorf(0,0,0,1);
};

class lightFunc {
public:
	static Colorf mixColors(Colorf color_a, Colorf color_b, std::string mode = "mix", float t = -1.0f, float gamma = 2.2f) {
	
		assert(mode == "mix" || mode == "blend");
	
		if (t == -1.0f) {
			if (mode == "mix") t = 0.5f;
			else t = 0;
		}

		float tinv = 0.0001f;
		if (t <= 1) {
			tinv = 1 - t;
			t = std::max<float>(0.0f, std::min<float>(t, 1.0f));
		}
		else {

			float tinv = (int)(255 - t);
			t = std::max<float>(0.0f, std::min<float>(t, 1.0f));
			t = t * 255;
		}
		Colorf ret;


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
			
			float alpha_a = color_a.a * (tinv);
			float a = 1 - (1 - alpha_a) * (1 - color_b.a);
			float s = color_b.a * (1 - alpha_a) / a;

				if (gamma == 1 || gamma == 0) {
					ret = (color_a + (color_b * s)) * (1 - s);
				} else {
					float nr = std::powf(std::powf(color_a.r,gamma) + (std::powf(color_b.r,gamma)*s) * (1 - s), 1 / gamma);
					float ng = std::powf(std::powf(color_a.g, gamma) + (std::powf(color_b.g, gamma) * s) * (1 - s), 1 / gamma);
					float nb = std::powf(std::powf(color_a.b, gamma) + (std::powf(color_b.b, gamma) * s) * (1 - s), 1 / gamma);
					ret = Colorf(nr, ng, nb, a);
				}
				
		}




		return ret;
	}
};