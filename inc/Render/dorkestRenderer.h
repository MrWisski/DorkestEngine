#pragma once
#include<vector>

#include "Util/Math/Vector2.h"
#include "Util/Math/Vector3.h"
#include "dorkestCamera.h"
#include "dorkestSprite.h"
#include "Util/Math/Geometry/LineSeg.h"
#include "Util/Math/Geometry/AABB.h"

class dorkestSprite;



class dorkestRenderer {
public:
	dorkestCamera* cam;
	
	enum sType { DIFFUSE = 64, NORMAL = 32, WIRE = 0 };

	olc::Pixel forcedColor;
	float forceScale;
	Vector2f forceSize;


	Vector3d PixelToNormal(int r, int g, int b);
	
	olc::vi2d MapToScreen(Vector2f vTileSize, float scaleFactor, Vector3f map, Vector2f offset);
	olc::vf2d ScreenToMap(Vector2f vTileSize, float scaleFactor, Vector2f m, Vector2f offset);
	
	bool drawToScreen(float ScreenX, float ScreenY, std::string name, sType spriteType = sType::DIFFUSE, bool ForceColor = false, bool ForceScale = false, bool ForceSize = false);
	bool drawToWorld(float WorldX, float WorldY, float WorldZ, std::string name, sType spriteType = sType::DIFFUSE, bool ForceColor = false, bool ForceScale = false, bool ForceSize = false);

	/* Draws a ray */
	void draw(Ray<float> ray);
	/* Draws a line in screenspace. */
	void draw(LineSeg2Di line);
	/* Draws a line in worldspace. */
	void draw(LineSeg3Df line);
	/* Draws a Bounding Box */
	void draw(AABB<float> box);


	void drawRay(Ray<float> ray);

	void screenLineW(Vector3i from, Vector3i to, olc::Pixel col);

	void screenLine(Vector2i from, Vector2i to, olc::Pixel col);

	void line3D(Vector3i from, Vector3i to, olc::Pixel col);

	void doRender(float fElapsedTime);
	
	dorkestRenderer() {}
	~dorkestRenderer() {}
protected:
	bool drawDecal(Vector2f dest, Vector2f destSize, olc::Decal*, Vector2f src, Vector2f srcSize, olc::Pixel color);
	bool drawSprite(Vector2f dest, int scale, olc::Sprite*, Vector2f src, Vector2f srcSize);
};