/*
	Name : Dorkest Engine
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

#define OLC_PGEX_SOUND
#define OLC_PGEX_AUDIOCONVERT

#include <Engine\Engine.h>

#include <Engine/Render/dorkestSpriteManager.h>
#include "Util/Math/Vector4.h"
#include "Extensions\olcPGEX_SplashScreen.h"
#include "ECS/components.h"
#include "Util/dorkestProfiler.h"
#include <Engine/Render/dorkestRenderer.h>
#include "ECS/dorkestScene.h"

#include "Util/Math/Vector2.h"

#include "Util/Log.h"
#include "instPGE.h"
#include <Util/Math/Geometry/FibSphere.h>
#include <Util/Color.h>

/*
Megarev — Today at 2:23 PM
Calculate the dot product of a given side with the light vector(edited)
[2:24 PM]
If dot product != 0, then you know light is right in front of it(edited)
[2:24 PM]
If dot product = 0, then light is not able to reach that face

*/

/*
std::string textIn(olc::PixelGameEngine* pge) {
	std::string s = "";


		// A control has requested that keyboard focus be given to it
		bool bShift = pge->GetKey(olc::Key::SHIFT).bHeld;

		// Handle standard Characters
		for (int i = 1; i < 27; i++)
			if (pge->GetKey((olc::Key)i).bPressed) s += std::string(1, (bShift ? (char)(i + 64) : (char)(i + 96));

		// Handle Keyboard Numbers
		for (int i = 27; i < 37; i++)
			if (pge->GetKey((olc::Key)i).bPressed) s += std::string(1, (char)(i + 48 - 27));

		// Handle Numpad Numbers
		for (int i = 69; i < 79; i++)
			if (pge->GetKey((olc::Key)i).bPressed) s += std::string(1, (char)(i + 48 - 69));

		// Handle limited number of symbols
		if (pge->GetKey(olc::Key::SPACE).bPressed) s += std::string(1, ' ');
		if (pge->GetKey(olc::Key::BACK).bPressed) s.pop_back();
		if (pge->GetKey(olc::Key::ENTER).bPressed) s += std::string(1, 10);
		if (pge->GetKey(olc::Key::NP_SUB).bPressed) s += std::string(1, '-');
		if (pge->GetKey(olc::Key::NP_DECIMAL).bPressed) s += std::string(1, '.');

}
*/

bool Engine::OnUserCreate()
{
	this->scene = new dorkestScene("TEST SCENE", this);
	//this->scene->setupMap();

	dorkestSpriteMan* dsm = dorkestSpriteMan::getInstance();
	dsm->addNewSpriteSheet("BasicISO", "./res/ISO/BasicIso.png");
	dsm->makeDecal("BasicISO");

	dsm->makeDorkestSprite_Decal("basicCube", olc::WHITE, "BasicISO", 1, { 32,32 }, { 64,0 });
	dsm->makeDorkestSprite_Decal("wireCube", olc::WHITE, "BasicISO", 1, { 32,32 }, { 0,0 });

	dsm->makeDorkestSprite_Decal("basicPlane", olc::WHITE, "BasicISO", 1, { 32,32 }, { 96,0 });


	dsm->makeDorkestSprite_Decal("oCube", olc::WHITE, "BasicISO", 1, { 32,32 }, { 0,0 });
	dsm->makeDorkestSprite_Decal("bpPlane", olc::WHITE, "BasicISO", 1, { 32,32 }, { 32,0 });
	dsm->makeDorkestSprite_Decal("oPlane", olc::WHITE, "BasicISO", 1, { 32,32 }, { 608,0 });


	dsm->makeDorkestSprite_Decal("basicSphere", olc::WHITE, "BasicISO", 1, { 32,32 }, { 576,0 });

	debug(dsm->getDorkestSprite("basicCube")->toStr());
	debug(dsm->getDorkestSprite("basicPlane")->toStr());
	debug(dsm->getDorkestSprite("oCube")->toStr());
	debug(dsm->getDorkestSprite("bpPlane")->toStr());

	//Initialize sound subsystem
	olc::SOUND::InitialiseAudio(44100, 1);
	//olc::SOUND::PlaySample(m_sample,true);

	this->toolwin = new DebugStuff();


	return true;
}

bool Engine::pressAnyKey() {
	for (int x = 1; x < 255; x++) {
		olc::Key k = olc::Key(x);

		if (instPGE::getInstance()->GetKey(k).bPressed) {
			std::stringstream s;

			s << "User pressed key " << x;
			debug(s.str());
			return true;
		}
	}

	return false;
}

/*
auto Engine::FromScreen(int x, int y, int hoffset, int voffset) {

	//olc::vi2d tr = transform(map.x, map.y);// +map.z);
	//return olc::vi2d(tr.x+100,tr.y + 300);
	float sizey = 1 *vTileSize.x;
	float sizex = 1 * vTileSize.y;
	float hsizex = sizex / 2;
	float hsizey = sizey / 2;
	float qsizey = hsizey / 2;

	//float dx = x - hoffset - sizex;
	//float dy = y - voffset + hsizey;
	//float tx = (dy + dx / 0.5f) * (0.5f/ 2) / hsizex;
	//float ty = (dy - dx / 0.5f) * (0.5f / 2) / hsizex;
	//float tx = (offsetx - ((x + y) / hsizex)) + hsizex;

	int tx = x+y;
	int ty = y - x;

	//int tx = ((x - y) / (vTileSize.x / 2));
	//	int ty = ((x + y) / (vTileSize.y / 4));
	//int ltx = (offset.x + ((map.x - map.y) * hsizex)) - hsizex;
	//int lty = (offset.y + ((map.x + map.y) * qsizey) + (map.z * hsizey)) - hsizey;
	////int ltx = ((x - y) / (int)hsizex) - hsizex;
	////int lty = ((x + y) / (int)hsizey) - hsizey;
	//int ltx = (offsetx + ((x - y) * hsizex));// +hsizex;
	//int lty = (offsety + ((x + y) * (hsizey / 2)));// +hsizey;
	//ltx = ltx - (16 * sizex);
	return olc::vi2d(tx, ty);
};


// Labmda function to convert "world" coordinate into screen space
auto Engine::ToScreen(int x, int y, int z)
{

	return olc::vi2d
	{
		 ((x - y) * (vTileSize.x / 2)),
		 ((x + y) * (vTileSize.y / 4) - (z * vTileSize.y / 2))
	};
};



static Matrix3f matIso;
static Matrix3f Ident;


std::vector<Block> checkHitStack(olc::vi2d mt) {

	std::vector<Block> stack;
	Renderer* r = instPGE::getInstance()->engine->r;
	Engine* e = instPGE::getInstance()->engine;
	Vector3f toCam;

	toCam = { 1,1,1 };
	toCam.norm();
	//toCam = { 0.612375, 0.612375, -0.50000 };
	//toCam = r->PixelToNormal(256, 125, 254); // straight up?
	//toCam = r->PixelToNormal(128, 238, 191); // straight up?

	Vector3f s = Vector3d(mt.x, mt.y, 0);
	Ray<float> ray(s, toCam, 100);



	std::sort(stack.begin(), stack.end());
	return stack;
}


void checkHit(olc::vi2d mt) {

	Renderer* r = instPGE::getInstance()->engine->r;
	Engine* e = instPGE::getInstance()->engine;
	Vector3f toCam;

	toCam = { 1,1,1 };
	//toCam = { 0.612375, 0.612375, -0.50000 };
	//toCam = r->PixelToNormal(256, 125, 254); // straight up?
	//toCam = r->PixelToNormal(128, 238, 191); // straight up?

	if (matIso == Ident) {
		//matIso = matIso.createRotationAroundAxis(30.0f, 45.0, 0);
		matIso = matIso.createRotationAroundAxis(60.0f, 0, 45.0);
	}
	//toCam.norm();



	//toCam = matIso * toCam;
	//toCam.norm();

	//olc::vi2d mt = r->ScreenToMap(Vector2f(32, 16), 1, Vector2f(m.x, m.y), Vector2f(0, 0));

	Vector3f s = Vector3d(mt.x, mt.y, 0);
	Ray<float> ray(s, toCam, 160);
	r->forcedColor = olc::BLANK;


	std::vector<Block> retstack = checkHitStack(mt);

	if (retstack.size() == 0) {
		e->cellcolor = olc::BLANK;
	} else {
		Block ret = retstack[retstack.size() - 1];
		e->cellcolor = ret.c;
	}



	return;
	/*
	if (ret != nullptr) {
		//instPGE::getInstance()->DrawString({ 500,120 }, "ret (" + ret->m_bounds.toString() + ")", olc::WHITE, 2);
		if (ret->m_data != nullptr)
			r->forcedColor = ret->m_data->c;
		else
			r->forcedColor = olc::DARK_RED;

		//r->forceScale = ret->m_size;
		//r->drawToScreen(0, 0, "oCube", Renderer::DIFFUSE, true, true);
	}
	else {
		r->forcedColor = olc::BLANK;
		//instPGE::getInstance()->DrawString({ 500,120 }, "ret NULL", olc::WHITE, 2);
	}


	e->cellcolor = r->forcedColor;

}

void Engine::renderBlocks() {



	std::sort(blocks.begin(), blocks.end());// , sortBlocks);


	//std::sort(blocks.begin(), blocks.end(), sortByYZ);

	for (const Block &b : blocks) {
		r->forcedColor = b.c;
		AABB3f bb = AABB3f(b.wPos, { 1,1,1 });
		for (Ray<float> ray : rays) {

			if (HitTest<float>::I3D_ray_aabb(ray, bb)) {
				r->forcedColor = olc::Pixel(rand() % 255, rand() % 255, rand() % 255);
				break;
			}
		}

		r->drawToWorld(b.wPos.x, b.wPos.y, b.wPos.z, "oCube", Renderer::DIFFUSE, true);
	}

}

void Engine::renderRays() {
	for (Ray<float> ray : rays) {
		r->drawRay(ray);
	}

}
*/

void drawPlane(Engine* e) {
	return;
	for (int x = 0; x < 16; x++)
		for (int y = 0; y < 16; y++) {
			debug("Creating cube at " + std::to_string(x) + ", " + std::to_string(y));
			AABB3f tbb(Vector3f(x, y, 0), Vector3f(1, 1, 1));
			std::shared_ptr<dorkestBaseEntity> entp = e->scene->createTerrain(tbb, "oPlane");
			if (entp == nullptr) { error("NULL"); continue; }
			if (!entp->hasComponent<c_baseColor>()) {
				error("basecolor component missing from entity?");
				return;
			}
			c_baseColor cb = entp->getComponent<c_baseColor>();
			cb.color = Colorf(255, 255, 255, 255);
			entp->updateComponent<c_baseColor>(cb);

			if (entp == nullptr) { error("DBE is null!"); }
			e->ents.push_back(entp);
			debug("DONE!");

		}
}

void showCol(int col, int r, Colorf ca, Colorf cb, std::string sign, Colorf cr) {
	int ax = col;
	int tx = col + 20;
	int bx = col + 50;
	int ex = col + 70;
	int lx = col + 100;

	instPGE::getInstance()->FillCircle(ax, r, 10, ca);
	instPGE::getInstance()->DrawString(tx, r - 7, sign, olc::WHITE, 2);
	instPGE::getInstance()->FillCircle(bx, r, 10, cb);
	instPGE::getInstance()->DrawString(ex, r - 7, "=", olc::WHITE, 2);
	instPGE::getInstance()->FillCircle(lx, r, 10, cr);

}

/*
static bool runbefore = false;
static std::shared_ptr<Colorf> rcol[10];
void testColor() {

	if (runbefore == false || instPGE::getInstance()->GetKey(olc::Key::Y).bPressed) {
		for (int x = 0; x < 10; x++) rcol[x] = nullptr;
		runbefore = true;
	}

	Colorf cr, cb, cg, cbl, cw;
	cr = Colorf(olc::RED);
	cg = Colorf(olc::GREEN);
	cb = Colorf(olc::BLUE);
	cbl = Colorf(olc::BLACK);
	cw = Colorf(olc::WHITE);

	int r = 100;
	int rinc = 25;
	int c = 50;

	showCol(c, r, cr, cb, "+", cr + cb);
	r += rinc;
	showCol(c, r, cb, cg, "+", cb + cg);
	r += rinc;
	showCol(c, r, cg, cr, "+", cg + cr);
	r += rinc;
	showCol(c, r, cw, cr, "+", cw + cr);
	r += rinc;
	showCol(c, r, cw, cg, "+", cw + cg);
	r += rinc;
	showCol(c, r, cw, cb, "+", cw + cb);
	r += rinc;
	showCol(c, r, cbl, cr, "+", cbl + cr);
	r += rinc;
	showCol(c, r, cbl, cg, "+", cbl + cg);
	r += rinc;
	showCol(c, r, cbl, cb, "+", cbl + cb);
	r += rinc;

	instPGE::getInstance()->FillRect(c + 120, 90, 10, r - 100, olc::WHITE);

	c += 150;

	r = 100;
	showCol(c, r, cr, cb, "-", cr - cb);
	r += rinc;
	showCol(c, r, cb, cg, "-", cb - cg);
	r += rinc;
	showCol(c, r, cg, cr, "-", cg - cr);
	r += rinc;
	showCol(c, r, cw, cr, "-", cw - cr);
	r += rinc;
	showCol(c, r, cw, cg, "-", cw - cg);
	r += rinc;
	showCol(c, r, cw, cb, "-", cw - cb);
	r += rinc;
	showCol(c, r, cbl, cr, "-", cbl - cr);
	r += rinc;
	showCol(c, r, cbl, cg, "-", cbl - cg);
	r += rinc;
	showCol(c, r, cbl, cb, "-", cbl - cb);
	r += rinc;

	instPGE::getInstance()->FillRect(c + 120, 90, 10, r - 100, olc::WHITE);

	c += 150;
	r = 100;
	showCol(c, r, cr, cb, "*", cr * cb);
	r += rinc;
	showCol(c, r, cb, cg, "*", cb * cg);
	r += rinc;
	showCol(c, r, cg, cr, "*", cg * cr);
	r += rinc;
	showCol(c, r, cw, cr, "*", cw * cr);
	r += rinc;
	showCol(c, r, cw, cg, "*", cw * cg);
	r += rinc;
	showCol(c, r, cw, cb, "*", cw * cb);
	r += rinc;
	showCol(c, r, cbl, cr, "*", cbl * cr);
	r += rinc;
	showCol(c, r, cbl, cg, "*", cbl * cg);
	r += rinc;
	showCol(c, r, cbl, cb, "*", cbl * cb);
	r += rinc;

	instPGE::getInstance()->FillRect(c + 120, 90, 10, r - 100, olc::WHITE);

	c += 150;
	r = 100;
	showCol(c, r, cr, cb, "/", cr / cb);
	r += rinc;
	showCol(c, r, cb, cg, "/", cb / cg);
	r += rinc;
	showCol(c, r, cg, cr, "/", cg / cr);
	r += rinc;
	showCol(c, r, cw, cr, "/", cw / cr);
	r += rinc;
	showCol(c, r, cw, cg, "/", cw / cg);
	r += rinc;
	showCol(c, r, cw, cb, "/", cw / cb);
	r += rinc;
	showCol(c, r, cbl, cr, "/", cbl / cr);
	r += rinc;
	showCol(c, r, cbl, cg, "/", cbl / cg);
	r += rinc;
	showCol(c, r, cbl, cb, "/", cbl / cb);
	r += rinc;

	instPGE::getInstance()->FillRect(c + 120, 90, 10, r - 100, olc::WHITE);

	c = 25;
	Colorf base = olc::BLACK;

	srand(time(NULL));
	debug("STARTING X Y AVG.");
	for (int x = 0; x < 10; x++) {
		if (rcol[x] == nullptr) {
			int rr = rand() % 255;
			int rg = rand() % 255;
			int rb = rand() % 255;

			rcol[x] = std::make_shared<Colorf>(rr, rg, rb, 255);

		}
		instPGE::getInstance()->FillCircle(c, r, 10, base);

		instPGE::getInstance()->FillCircle(c, r + 30, 10, base.avg(2, base, *rcol[x]));

		instPGE::getInstance()->FillCircle(c, r + 60, 10, *rcol[x]);
		c += 20;

	}
	c += 20;
	debug("DOING thE BIG AVERAGE!!!!");
	instPGE::getInstance()->FillCircle(c, r + 30, 10, base.avg(10, base, *rcol[0], *rcol[1], *rcol[2], *rcol[3], *rcol[4], *rcol[5], *rcol[6], *rcol[7], *rcol[8], *rcol[9]));

	c += 30;

	base = olc::WHITE;

	srand(time(NULL));

	for (int x = 0; x < 10; x++) {
		if (rcol[x] == nullptr) {
			int rr = rand() % 255;
			int rg = rand() % 255;
			int rb = rand() % 255;

			rcol[x] = std::make_shared<Colorf>(rr, rg, rb, 255);

		}
		instPGE::getInstance()->FillCircle(c, r, 10, base);

		instPGE::getInstance()->FillCircle(c, r + 30, 10, base.avg(2, base, *rcol[x]));

		instPGE::getInstance()->FillCircle(c, r + 60, 10, *rcol[x]);
		c += 20;

	}
	c += 20;

	instPGE::getInstance()->FillCircle(c, r + 30, 10, base.avg(10, base, *rcol[0], *rcol[1], *rcol[2], *rcol[3], *rcol[4], *rcol[5], *rcol[6], *rcol[7], *rcol[8], *rcol[9]));




}
*/

void testPoly(Engine* e) {
	static bool frun = false;


	////Test our Vertexes.
	//static Vert v[10];

	//if (!frun) {
	//	for (int x = 0; x < 10; x++) {
	//		v[x].pos = Vector3f((rand() % 20) + 20, (rand() % 20) + 20, 0);
	//		v[x].col.setRand();
	//	}
	//	frun = true;
	//}

	//for (int x = 0; x < 10; x++) {
	//	e->scene->getRenderer()->drawVertex(v[x]);
	//}

	Vert v[3];
	
		v[0].pos = Vector3f(23, 0, 0);
		v[0].col = Colorf(olc::RED);

		v[1].pos = Vector3f(32,0, 0);
		v[1].col = Colorf(olc::BLUE);

		v[2].pos = Vector3f(32,9, 0);
		v[2].col = Colorf(olc::GREEN);

	
	Triangle t(v[0],v[1],v[2]);

	e->scene->getRenderer()->drawTriangle(t);

}

void Engine::doKeys(float fElapsedTime) {

	Vector3f mPos = scene->getCamera()->ScreenToMap(instPGE::getInstance()->GetMousePos());

	scene->getRenderer()->drawSprite(mPos, "basicSphere", dorkestRenderer::DIFFUSE, olc::YELLOW, 1);

	if (ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantCaptureKeyboard) {
		return;
	}

	if (instPGE::getInstance()->GetKey(olc::Key::Y).bPressed) {
		this->scene->getRenderer()->forceIgnoreDecal(false);

	}

	if (instPGE::getInstance()->GetKey(olc::Key::T).bHeld) {
		//testColor();
		testPoly(this);
	}


	if (instPGE::getInstance()->GetKey(olc::Key::M).bPressed) {
		drawPlane(this);
	}

	if (instPGE::getInstance()->GetKey(olc::Key::OEM_3).bPressed) {
		Log::Instance()->toggleShowHide();
	}

	if (instPGE::getInstance()->GetKey(olc::Key::ESCAPE).bPressed) {
		this->toolwin->toggleShowHide();
	}

	if (instPGE::getInstance()->GetKey(olc::Key::SPACE).bHeld) { toggle = true; }
	else { toggle = false; }

	if (instPGE::getInstance()->GetKey(olc::Key::C).bPressed) {
		scene->setCameraCenter(scene->getCamera()->getScreenWidth() / 2.0f, scene->getCamera()->getScreenHeight() / 2.0f, 0);
	}

	if (instPGE::getInstance()->GetKey(olc::Key::X).bPressed) {
		scene->setCameraCenter(0, 0, 0);

	}


	static float angle = 0;

	if (instPGE::getInstance()->GetKey(olc::Key::H).bHeld) {

		if (fsphere == nullptr) { fsphere = new FibSphere(600); }
		for (Vector3f p : this->fsphere->points) {

			p.rot(0, 0, angle);
			this->scene->getRenderer()->drawSprite(Vector3f(p.x * 8, p.y * 8, p.z * 8), "basicSphere", dorkestRenderer::DIFFUSE, Colorf(255, 255, 255, 255));
		}
		angle += 50 * fElapsedTime;

	}



	if (instPGE::getInstance()->GetKey(olc::Key::Z).bPressed) {
		debug(dorkestProfiler::getInstance()->viewAllPoints());
	}
	if (instPGE::getInstance()->GetKey(olc::Key::Z).bHeld) {

	}

	if (instPGE::getInstance()->GetKey(olc::Key::V).bPressed) {
		//DrawGrid();
	}
	else if (instPGE::getInstance()->GetKey(olc::Key::V).bHeld) {
		//DrawGrid();
	}

	if (instPGE::getInstance()->GetKey(olc::Key::L).bPressed) {
		debug("Recalculating lighting.");
		scene->recalcLight();

	}

	static Vector2d dragpos;
	if (instPGE::getInstance()->GetMouse(2).bPressed) {
		dragpos = vMouse;
	}
	else if (instPGE::getInstance()->GetMouse(2).bHeld) {
		//if (instPGE::getInstance()->GetMouse(1).bHeld) {
		Vector2d mpv = Vector2d(vMouse) - dragpos;// dragpos;//Vector2d(dragpos.x - (double)mp.x, dragpos.y - (double)mp.y);
		Vector2d cp = olc::vd2d(scene->getCamera()->getCenter());
		cp += (mpv * 3) * fElapsedTime;
		//cp.lerp(fElapsedTime,dragpos);
		debug(cp.toStr());
		scene->setCameraCenter(cp.x, cp.y, 0);

	}

	if (instPGE::getInstance()->GetMouse(0).bPressed) {
		return;
		dorkestBaseEntity* newLight = this->scene->createNewEntity();

		LightSource data;
		//data.linear = 0.09f;
		//data.constant = 0.000000001f;
		//data.quadratic = 0.023f;
		data.color = this->toolwin->getColor();
		data.constant = this->toolwin->getcon();
		data.linear = this->toolwin->getlin();
		data.quadratic = this->toolwin->getquad();

		newLight->addComponent<c_staticLight>(data);

		Vector3f vMT = this->scene->getCamera()->ScreenToMap(vMouse);


		newLight->addComponent<c_position>(Vector3f(vMT.x, vMT.y, std::max(this->toolwin->getZ(), 1)), vMouse);
		newLight->addComponent<c_sprite>("basicSphere");
		newLight->addComponent<c_baseColor>(data.color);
		newLight->addComponent<c_imposter>();
		newLight->addComponent<c_statusflags>();

		scene->recalcLight();



	};



	if (instPGE::getInstance()->GetMouse(1).bPressed) {

	}

	if (instPGE::getInstance()->GetKey(olc::Key::END).bPressed) {
		ents.clear();

		delete this->scene;
		this->scene = new dorkestScene("TEST SCENE", this);
		this->scene->setupMap();
		//drawPlane(this);
	}


	if (instPGE::getInstance()->GetKey(olc::Key::RIGHT).bPressed) { moveposx += 1; }
	if (instPGE::getInstance()->GetKey(olc::Key::LEFT).bPressed) { moveposx -= 1; }

	if (instPGE::getInstance()->GetKey(olc::Key::DOWN).bPressed) { moveposy += 1; }
	if (instPGE::getInstance()->GetKey(olc::Key::UP).bPressed) { moveposy -= 1; }

	if (instPGE::getInstance()->GetKey(olc::Key::W).bHeld) { vMouse = { 256,0 }; }
	if (instPGE::getInstance()->GetKey(olc::Key::A).bHeld) { vMouse = { 0,128 }; }
	if (instPGE::getInstance()->GetKey(olc::Key::S).bHeld) { vMouse = { 256,256 }; }
	if (instPGE::getInstance()->GetKey(olc::Key::D).bHeld) { vMouse = { 512,128 }; }




	if (instPGE::getInstance()->GetMouseWheel() < 0) {

	}
	else if (instPGE::getInstance()->GetMouseWheel() > 1) {

	}


}

bool Engine::OnUserUpdate(float fElapsedTime)
{

	vMouse = instPGE::getInstance()->GetMousePos();
	vMouseMap = scene->getCamera()->ScreenToMap(vMouse);

	static bool skipSplash = true;
	//Show splash screen.
	if (!skipSplash) {
		//if (pressAnyKey()) skipSplash = true;
		if (this->splashScreen->AnimateSplashScreen(fElapsedTime))	 return true;
	}
	olc::SOUND::StopAll();

	instPGE::getInstance()->SetPixelMode(olc::Pixel::ALPHA);
	instPGE::getInstance()->Clear(olc::GREY);

	doKeys(fElapsedTime);
	scene->doFrame(fElapsedTime);

	this->toolwin->render();
	return true;
};

bool Engine::OnUserDestroy() {
	debug("Cleaning up!");
	olc::SOUND::DestroyAudio();
	if (this->scene != nullptr)	delete this->scene;

	if (this->toolwin != nullptr) delete toolwin;

	if (!ents.empty()) {

		ents.clear();
	}

	if (splashScreen != nullptr) { delete splashScreen; }

	if (as != nullptr) { delete as; }

	if (fsphere != nullptr) { delete fsphere; }

	return true;
};