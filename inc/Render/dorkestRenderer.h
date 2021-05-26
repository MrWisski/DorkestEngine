/*
	Name : Dorkest Renderer
	Created : 5/26/2021
	Author : MrWisski
	License : (WISSKI-1)

	Copyright 2012 - 2021 Alex Wisnieski AKA MrWisski

	Permission is hereby granted, free of charge, to any person (hereafter "You") obtaining a copy or portion of this software, its resulting
	compilation products, or associated documentation files (hereafter "Software"), to deal in the Software with the following
	restrictions, using the following terminology :

	TERMINOLOGY :
		1) COMMERCIAL : Any use of the Software in which You, or others recieve compensation for any reason, of any kind.

	RESTRICTIONS :
		1) You MAY use, copy, publish, distribute, and/or merge, this Software, with any project or library or software,
		   that is not COMMERCIAL in nature or intent.

		2) You MAY NOT use, copy, publish, distribute, redistribute, sublicense, merge and/or sell copies of any software including partially,
		   containing whole, or any portion of the Software, nor permit persons to whom the Software is furnished to do so for COMMERCIAL
		   uses.

		3) Derivations of the Software are permitted, provided the above copyright notice and this permission notice are NOT
		   modified, with the exception of character encoding changes that do not substantially change the intent of the notice or the
		   above copyright notice, or the readibility or useability of this permission notice or the above copyright notice, except for
		   COMMERCIAL uses.

		4) You MAY NOT imply endorsement of any product that includes or derives from the Software, by any any person or group of people
		   involved in the creation of the Software without direct written permission from said persons or group of people.

		5) You MAY NOT use the legal names, handles, aliases, or nicknames of any person involved in the creation of the Software,
		   to promote any product derived from, or using the Software in any way, without written permission from said persons.

	The above copyright notice and this permission notice shall be included in all copies or portions of the Software.

	Any of the above restrictions which are unlawful or not permitted, whole or in part, in the jurisdiction You live in will not
	affect the rest of the portion of the restriction it exists in, the other restrictions listed in this permission notice.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
	MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
	FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
	WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

	CERTAIN JURISDICTIONS DO NOT ALLOW LIMITATIONS ON IMPLIED WARRANTIES OR THE EXCLUSION OR LIMITATION OF CERTAIN DAMAGES.
	IF YOU RESIDE IN SUCH A JURISDICTION, SOME OR ALL OF THE ABOVE DISCLAIMERS, EXCLUSIONS, OR LIMITATIONS MAY NOT APPLY TO YOU,
	AND YOU MAY HAVE ADDITIONAL RIGHTS. THE LIMITATIONS OR EXCLUSIONS OF WARRANTIES, REMEDIES, OR LIABILITY CONTAINED IN THESE TERMS
	APPLY TO YOU TO THE FULLEST EXTENT SUCH LIMITATIONS OR EXCLUSIONS ARE PERMITTED UNDER THE LAWS OF THE JURISDICTION IN WHICH
	YOU ARE LOCATED.
*/
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
	
	
	enum sType { DIFFUSE = 64, NORMAL = 32, WIRE = 0 };

	void setCam(dorkestCamera* cam) { this->cam = cam; }
	void setForcedColor(olc::Pixel c) { this->forcedColor = c; }
	void setForcedSpriteSize(Vector2f newSize) { this->forceSize = newSize; }
	void setForcedScale(float newScale) { this->forceScale = newScale; }
	dorkestCamera* getCamera() { if (this->cam != nullptr) return this->cam; else error("getCamera RETURNING NULL"); return nullptr; }
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
	

	dorkestRenderer() = default;
	~dorkestRenderer() = default;
	friend class dorkestScene;
protected:
	bool drawDecal(Vector2f dest, Vector2f destSize, olc::Decal*, Vector2f src, Vector2f srcSize, olc::Pixel color);
	bool drawSprite(Vector2f dest, int scale, olc::Sprite*, Vector2f src, Vector2f srcSize);
private:
	dorkestCamera* cam = nullptr;
	olc::Pixel forcedColor = olc::BLACK;
	float forceScale = 1.0f;
	Vector2f forceSize = {32,32};

};