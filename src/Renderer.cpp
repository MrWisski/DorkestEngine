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

#include <Engine/Render/dorkestRenderer.h>
#include "instPGE.h"
#include "Util/Log.h"
#include <Engine/Render/dorkestSpriteManager.h>
#include <Engine/Render/dorkestSprite.h>
#include <Util/Math/Vector4.h>

#include <Util/Math/Geometry/Ray.h>
#include <algorithm>

void dorkestRenderer::drawSprite(Vector3f mapPos, std::string name, sType spriteType, Colorf forceColor, float forceScale, Vector2f forceSize)
{
	//Do we have a valid sprite to draw?
	dorkestSprite* spr = dorkestSpriteMan::getInstance()->getDorkestSprite(name);
	if ((spr == nullptr)) {
		error("Cannot find sprite with name " + name);
		return;
	}

	if (forceColor == Colorf(0, 0, 0, 0))
		forceColor = spr->getSpriteTint();

	if (forceScale == -1.0f) {
		forceScale = spr->getScaleFactor();
	} 
	if (forceSize == Vector2f(-1.0f, -1.0f))
		forceSize = Vector2f(spr->getSpriteSize());

	Vector2i Screent = cam->iTOc(spr->getSpriteSize(), forceScale, mapPos, { 0,0 });// this->MapToScreen(size, scale.x, Vector3d(WorldX, WorldY, WorldZ), { 0,0 });

	this->drawToScreen(Screent, name, spriteType, forceColor, forceScale, forceSize);

}

void dorkestRenderer::drawTextRow(int row, std::string Str, float scale, olc::Pixel color, textAlign TA)
{
	Vector2i al = Vector2i(0,row * (instPGE::getInstance()->GetTextSize(Str).y * scale));
	switch (TA) {
	case LEFT:
		al.x = 0;
		instPGE::getInstance()->DrawString(al, Str, color, scale);
		break;
	case RIGHT:
		al.x = this->cam->getScreenWidth() - (instPGE::getInstance()->GetTextSize(Str).x * scale);
		instPGE::getInstance()->DrawString(al, Str, color, scale);
		break;
	case CENTER:
		al.x = (this->cam->getScreenWidth() / 2) - ((instPGE::getInstance()->GetTextSize(Str).x * scale) / 2);
		instPGE::getInstance()->DrawString(al, Str, color, scale);
		break;
	default:
		al.x = 0;
		instPGE::getInstance()->DrawString(al, Str, color, 2);
	}

}

void dorkestRenderer::drawTextPos(Vector2f screenPos, std::string Str, float scale, olc::Pixel color, textAlign TA)
{
	Vector2f al = screenPos;
	switch (TA) {
	case LEFT:
		al.x = 0;
		instPGE::getInstance()->DrawString(al, Str, color, 2);
		break;
	case RIGHT: 
		al.x = this->cam->getScreenWidth() - (instPGE::getInstance()->GetTextSize(Str).x*2) ;
		instPGE::getInstance()->DrawString(al, Str, color, 2);
		break;
	case CENTER:
		al.x = (this->cam->getScreenWidth() / 2) - ((instPGE::getInstance()->GetTextSize(Str).x * 2)/2);
		instPGE::getInstance()->DrawString(al, Str, color, 2);
		break;
	default:
		instPGE::getInstance()->DrawString(al, Str, color, 2);

	}
	
}

void dorkestRenderer::drawRay(Ray<float> ray, Colorf sCol, Colorf rCol) {

	ray.dir.norm();
	ray.invdir.norm();

	Vector2i so = cam->MapToScreen( ray.orig);
	so.y += 8;
	Vector2i se = cam->MapToScreen(ray.orig + (ray.dir * ray.tmax));
	LineSeg2Di lseg = LineSeg2Di(so,se);

	//Draw the origin point as a sphere.
	drawSprite(ray.orig, "basicSphere",DIFFUSE,sCol);

	//draw the projected line segment.
	drawLine2D(lseg,rCol);

	
}

void dorkestRenderer::drawLine2D(LineSeg2Di line, Colorf col) {
	//instPGE::getInstance()->setHudDraw();
	instPGE::getInstance()->SetPixelMode(olc::Pixel::NORMAL);
	instPGE::getInstance()->DrawLine(line.P1, line.P2, col);
	//instPGE::getInstance()->setGameDraw();
}

void dorkestRenderer::drawLine3D(LineSeg3Df line, Colorf col) {
	//instPGE::getInstance()->setHudDraw();
	instPGE::getInstance()->SetPixelMode(olc::Pixel::NORMAL);

	Vector2i p1m = this->cam->iTOc({ 32.0f,32.0f }, 1, line.P1, { 0.0f,0.0f });
	Vector2i p2m = this->cam->iTOc({ 32.0f,32.0f }, 1, line.P2, { 0.0f,0.0f });


	instPGE::getInstance()->DrawLine(p1m, p2m, col);
	//instPGE::getInstance()->setGameDraw();
}

void dorkestRenderer::drawVertex(Vert v)
{
	
	drawSprite(v.pos, "basicSphere", DIFFUSE, v.col, 0.250f);

}

void dorkestRenderer::drawTriangle(Triangle t, Colorf col)
{
	drawSprite(t[0].pos, "basicSphere", DIFFUSE, t[0].col);
	drawSprite(t[1].pos, "basicSphere", DIFFUSE, t[1].col);
	drawSprite(t[2].pos, "basicSphere", DIFFUSE, t[2].col);

	dorkestSprite* spr = dorkestSpriteMan::getInstance()->getDorkestSprite("basicSphere");
	
	Vector2f qsize = (Vector2f)spr->getSpriteSize() / 4;

	Vector2f pa = this->cam->iTOc({ 32.0f,32.0f }, 1, t[0].pos, { 0.0f,0.0f });
	Vector2f pb = this->cam->iTOc({ 32.0f,32.0f }, 1, t[1].pos, { 0.0f,0.0f });
	Vector2f pc = this->cam->iTOc({ 32.0f,32.0f }, 1, t[2].pos, { 0.0f,0.0f });

	pa.y += qsize.y;
	pb.y += qsize.y;
	pc.y += qsize.y;

	drawLine2D(LineSeg2Di(pa, pb), Colorf(olc::RED));
	drawLine2D(LineSeg2Di(pb, pc), Colorf(olc::RED));
	drawLine2D(LineSeg2Di(pc, pa), Colorf(olc::RED));
}

//Vector3d dorkestRenderer::PixelToNormal(int r, int g, int b)
//{
//	double rr = (((double)r / 256.0f) * 2.0f) - 1.0f; // 0
//	double gg = (((double)g / 256.0f) * 2.0f) - 1.0f; // 0.0859375
//	double bb = (((double)b / 256.0f) * 2.0f) - 1.0f; // 0.984375
//	
//	Vector3d rgb = Vector3d(rr, gg, bb);
//	//rgb.norm();
//	//debug(rgb.toStr());
//
//	return rgb;
//}




bool dorkestRenderer::drawDecalINTERNAL(Vector2f dest, Vector2f destSize, olc::Decal* spr, Vector2f src, Vector2f srcSize, olc::Pixel color) {
	instPGE::getInstance()->DrawPartialDecal(dest, destSize, spr,	src, srcSize, color);
	return true;
}


bool dorkestRenderer::drawSpriteINTERNAL(Vector2f dest, int scale, olc::Sprite* spr, Vector2f src, Vector2f srcSize) {
	instPGE::getInstance()->DrawPartialSprite(dest,	spr, src, srcSize, scale);
	return true;
}

bool dorkestRenderer::drawToScreen(Vector2i screenPos, std::string name, sType spriteType, Colorf color, float scale, Vector2f size)
{
	dorkestSprite* spr = dorkestSpriteMan::getInstance()->getDorkestSprite(name);
	if (spr == nullptr) { error("Sprite not found : " + name); return false; }

	//Replace our defaults, if needed.
	if (color == Colorf(0,0,0,0)) { color = spr->getSpriteTint();}
	if (scale == -1.0f) { scale = spr->getScaleFactor(); }
	if (size = Vector2i(-1,-1)) { size = spr->getSpriteSize(); }

	//Get the offset for what sprite we're drawing.
	Vector2f coord = Vector2f(spr->getSpriteCoord().x, spriteType );

	scale = cam->getZoom();

	//Draw the sprite from the center.
	screenPos.x -= (size.x/2) * scale;
	screenPos.y -= (size.y/2) * scale;

	Vector2f scaledSize = (Vector2f)size * scale;

	//Prefer to draw a decal.
	if (spr->hasDecal() && this->forceIgnoreDecalFlag == false) {		
		instPGE::getInstance()->DrawPartialDecal(
			screenPos,
			scaledSize,
			spr->getDecalSheet(),
			coord,
			size,
			color);
	} else if (spr->hasSprite()) {
		//Otherwise, draw a sprite or error out.
		
		instPGE::getInstance()->DrawPartialSprite(
			screenPos,
			spr->getSpriteSheet(),
			coord,
			size,
			scale);
		debug("We are drawing a sprite without a color value ----- Make sure to use decals!");
	} else { error("Tried to draw a sprite that has neither a SPRITE based sheet, or a decal based sheet!");  return false; }
	return true;
}

void dorkestRenderer::drawAABB3(AABB3f box, Colorf col)
{
	Vector3f v[8];
	Vector3f l = box.getLocation();
	Vector3f m = box.getMax();

	float sx = box.getHalfsize().x * 2;
	float sy = box.getHalfsize().y * 2;
	//float sz = box.getHalfsize().z * 2;

	v[0] = Vector3f(l.x, l.y, l.z);
	v[1] = Vector3f(l.x + sx, l.y, l.z);
	v[2] = Vector3f(l.x, l.y + sy, l.z);
	v[3] = Vector3f(l.x + sx, l.y + sy, l.z);

	v[4] = Vector3f(m.x, m.y, m.z);
	v[5] = Vector3f(m.x - sx, m.y, m.z);
	v[6] = Vector3f(m.x, m.y - sy, m.z);
	v[7] = Vector3f(m.x - sx, m.y - sy, m.z);

	//Bottom face
	//this->drawLine3D(LineSeg3Df(v[0], v[1]), col);
	this->drawLine3D(LineSeg3Df(v[1], v[3]), col);
	this->drawLine3D(LineSeg3Df(v[3], v[2]), col);
	//this->drawLine3D(LineSeg3Df(v[2], v[0]), col);

	//Top Face
	this->drawLine3D(LineSeg3Df(v[4], v[5]), col);
	this->drawLine3D(LineSeg3Df(v[5], v[7]), col);
	this->drawLine3D(LineSeg3Df(v[7], v[6]), col);
	this->drawLine3D(LineSeg3Df(v[6], v[4]), col);

	//Connecting lines
	this->drawLine3D(LineSeg3Df(v[1], v[6]), col);
	this->drawLine3D(LineSeg3Df(v[3], v[4]), col);
	this->drawLine3D(LineSeg3Df(v[2], v[5]), col);


	//this->drawLine3D(LineSeg3Df(v[1], v[2]), col);
	//this->drawLine3D(LineSeg3Df(v[2], v[3]), col);
	//this->drawLine3D(LineSeg3Df(v[3], v[4]), col);
	//this->drawLine3D(LineSeg3Df(v[4], v[5]), col);
	//this->drawLine3D(LineSeg3Df(v[5], v[6]), col);
	//this->drawLine3D(LineSeg3Df(v[6], v[7]), col);
	//this->drawLine3D(LineSeg3Df(v[7], v[0]), col);
}

void dorkestRenderer::drawAABB2(AABB2i box, Colorf col)
{
	Vector2f v[4];
	Vector2f l = box.getLocation();
	Vector2f m = box.getMax();

	float sx = box.getHalfsize().x * 2;
	float sy = box.getHalfsize().y * 2;
	//float sz = box.getHalfsize().z * 2;

	v[0] = Vector2f(l.x, l.y);
	v[1] = Vector2f(l.x + sx, l.y);
	v[2] = Vector2f(l.x, l.y + sy);
	v[3] = Vector2f(l.x + sx, l.y + sy);



	//Bottom face
	this->drawLine2D(LineSeg2Di(v[0], v[1]), col);
	this->drawLine2D(LineSeg2Di(v[1], v[3]), col);
	this->drawLine2D(LineSeg2Di(v[3], v[2]), col);
	this->drawLine2D(LineSeg2Di(v[2], v[0]), col);

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
