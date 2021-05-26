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

#include "Render/dorkestRenderer.h"
#include "instPGE.h"
#include "Util/Log.h"
#include "Render/dorkestSpriteManager.h"
#include "Render/dorkestSprite.h"
#include <Util/Math/Vector4.h>

#include <Util/Math/Geometry/Ray.h>
#include <algorithm>

void dorkestRenderer::drawRay(Ray<float> ray) {
	forcedColor = olc::WHITE;
	forceSize = Vector2f(32, 32);
	ray.dir.norm();
	ray.invdir.norm();
	Vector2i so = MapToScreen({ 32,32 }, 1, ray.orig, { 0,0 });
	Vector2i se = MapToScreen({ 32,32 }, 1, ray.orig + (ray.dir * ray.tmax), { 0,0 });
	so.y += 8;


	screenLine(so, se, olc::RED);
	drawToWorld(ray.orig.x, ray.orig.y, ray.orig.z, "basicSphere", dorkestRenderer::DIFFUSE, true, false, true);
}

void dorkestRenderer::screenLineW(Vector3i from, Vector3i to, olc::Pixel col) {
	olc::vi2d f = MapToScreen({ 32,32 }, 1, from, { 0,0 });
	olc::vi2d t = MapToScreen({ 32,32 }, 1, to, { 0,0 });

	screenLine(f, t, col);
}

void dorkestRenderer::screenLine(Vector2i from, Vector2i to, olc::Pixel col) {
	Vector4f c;
	c = Vector4f(col.r, col.b, col.g, col.a);
	debug("Drawing line " + from.toStr() + " to " + to.toStr() + " color = " + c.toString());
	instPGE::getInstance()->setHudDraw();
	instPGE::getInstance()->SetPixelMode(olc::Pixel::NORMAL);
	instPGE::getInstance()->DrawLine(from, to, col);
	instPGE::getInstance()->setGameDraw();
}


void dorkestRenderer::doRender(float fElapsedTime) {

	//errCol = olc::Pixel(std::rand() % 255, std::rand() % 255, std::rand() % 255, 255);
}

Vector3d dorkestRenderer::PixelToNormal(int r, int g, int b)
{
	double rr = (((double)r / 256.0f) * 2.0f) - 1.0f; // 0
	double gg = (((double)g / 256.0f) * 2.0f) - 1.0f; // 0.0859375
	double bb = (((double)b / 256.0f) * 2.0f) - 1.0f; // 0.984375
	
	Vector3d rgb = Vector3d(rr, gg, bb);
	//rgb.norm();
	//debug(rgb.toStr());

	return rgb;
}

olc::vi2d dorkestRenderer::MapToScreen(Vector2f vTileSize, float scaleFactor, Vector3f map, Vector2f offset)
{
	//The grid is universal. OBEY THE GRID.
	float sizex = vTileSize.x;
	float sizey = vTileSize.y;
	float hsizex = sizex / 2;
	float hsizey = sizey / 2;
	float qsizey = hsizey / 2;
	offset.x = 0; offset.y = 0;


	int ltx = cam->getCenter().x + (offset.x + ((map.x - map.y) * hsizex)) - hsizex;
	int lty = cam->getCenter().y + (offset.y + ((map.x + map.y) * qsizey) - (map.z * hsizey)) - hsizey;
	

	Vector2f lt = Vector2f(ltx, lty);
	//debug("Map coords of " + map.toStr() + " translate to screen coords of " + lt.toStr());
	return olc::vi2d(ltx, lty);

};

olc::vf2d dorkestRenderer::ScreenToMap(Vector2f vTileSize, float scaleFactor, Vector2f m, Vector2f offset) {
	//Correct for the camera position
	m.x -= cam->getCenter().x;
	m.y -= cam->getCenter().y;

	//The grid is universal. OBEY THE GRID.
	//Convert SCREEN coordinates to WORLD coordinates
	float ltx = (m.x / vTileSize.x) + (m.y / vTileSize.y);
	float lty = (m.y / vTileSize.y) - (m.x / vTileSize.x);
	
	//Correct for crossing 0
	if ((ltx < 0)) ltx -= 1;
	if ((lty < 0)) lty -= 1;
	
	//Correct for the centering of the sprite *BLACK MAGIC* I really have no clue why this works. Maybe rounding? *shrug*
	ltx += 1.5f;
	lty += 0.5f;

	//Return our screen Coordinates.
	return olc::vf2d(ltx, lty);
}

bool dorkestRenderer::drawDecal(Vector2f dest, Vector2f destSize, olc::Decal* spr, Vector2f src, Vector2f srcSize, olc::Pixel color) {
	instPGE::getInstance()->DrawPartialDecal(dest, destSize, spr,	src, srcSize, color);
	return true;
}


bool dorkestRenderer::drawSprite(Vector2f dest, int scale, olc::Sprite* spr, Vector2f src, Vector2f srcSize) {
	instPGE::getInstance()->DrawPartialSprite(dest,	spr, src, srcSize, scale);
	return true;
}

bool dorkestRenderer::drawToScreen(float ScreenX, float ScreenY, std::string name, sType spriteType, bool ForceColor, bool ForceScale, bool ForceSize)
{
	dorkestSprite* spr = dorkestSpriteMan::getInstance()->getDorkestSprite(name);
	if (spr == nullptr) { return false; }

	olc::Pixel C = spr->getSpriteTint();
	if (ForceColor) { C = this->forcedColor; }

	float SCALE = 1.0f;
	if (ForceScale) { SCALE = this->forceScale; }

	Vector2f SIZE = spr->getSpriteSize();
	if (ForceSize) { SIZE = this->forceSize; }

	Vector2f coord = Vector2f(spr->getSpriteCoord().x, spriteType );

	ScreenX -= (SIZE.x/2) * SCALE;
	ScreenY -= (SIZE.y/2) * SCALE;

	if (spr->hasDecal()) {
		instPGE::getInstance()->DrawPartialDecal(
			olc::vi2d((int)ScreenX, (int)ScreenY),
			SIZE * SCALE,// * spr->getScaleFactor(),
			spr->getDecalSheet(),
			coord,
			SIZE,
			C);
	}
	else if (spr->hasSprite()) {
		//debug("Has sprite");
		instPGE::getInstance()->DrawPartialSprite(
			olc::vi2d((int)ScreenX, (int)ScreenY),
			spr->getSpriteSheet(),
			coord,
			SIZE,
			SCALE);

	}
	else { debug("has no sheets.");  return false; }
	return true;
}

bool dorkestRenderer::drawToWorld(float WorldX, float WorldY, float WorldZ, std::string name, sType spriteType, bool ForceColor, bool ForceScale, bool ForceSize)
{
	//debug("Renderer::drawSpriteToWorld(" + std::to_string(WorldX) + ", " + std::to_string(WorldY) + ", " + std::to_string(WorldZ) + ", " + name + ")");

	dorkestSprite* spr = dorkestSpriteMan::getInstance()->getDorkestSprite(name);
	if ((spr == nullptr)) { 
		return false; 
	}

	olc::vi2d Screent;

	Vector2f scale = { 1,1 };

	if (ForceScale)
		scale = { this->forceScale,this->forceScale };

	Vector2f size = spr->getSpriteSize();
	if (ForceSize)
		size = this->forceSize;

	Screent = this->MapToScreen(size, scale.x, Vector3d(WorldX, WorldY, WorldZ), { 0,0 });
	
	return this->drawToScreen(Screent.x, Screent.y, name, spriteType, ForceColor, ForceScale, ForceSize);
	
}


/*
void Renderer::loadResources() {
	debug("Loading Resources...");
	//trans.resize(4);

	//trans[0] = 1.0f * (vTileSize.x * 0.5f);
	//trans[1] = 0.5f * (vTileSize.y * 0.5f);

	//trans[2] = -1.0f * (vTileSize.x * 0.5f);
	//trans[3] = -0.5f * (vTileSize.y * 0.5f);



	olc::ResourcePack* rpack = nullptr;
	sheet = new olc::Sprite("./res/ISO/BasicIso.png", rpack);

	dsheet = new olc::Decal(sheet, false, false);
	//m_sample = olc::AudioConvert::LoadAudioSample("CantinaBand3.wav");
	//m_sample = olc::SOUND::LoadAudioSample("CantinaBand3.wav");
	// as = new olc::SOUND::AudioSample();
	/*olc::rcode r = as->LoadFromFile("CantinaBand3.wav");
	if (r == olc::rcode::NO_FILE) {
		debug("NO FILE.");
	}
	else if (r == olc::rcode::FAIL) {
		debug("FAILED");
	}
	else {
		debug("OK");
	}

	
	//olc::Sprite* sprite = new olc::Sprite("../some/path/to/a/file.png", pack);
}
*/
