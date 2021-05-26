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

#include "Engine.h"
#include "World/Block.h"
#include "Render/dorkestSpriteManager.h"
#include "Util/Math/Vector4.h"
#include <algorithm>
#include "Util/HitTest.h"
#include "Extensions\olcPGEX_SplashScreen.h"
#include "ECS/components.h"
#include "Util/dorkestProfiler.h"
/*
Megarev — Today at 2:23 PM
Calculate the dot product of a given side with the light vector(edited)
[2:24 PM]
If dot product != 0, then you know light is right in front of it(edited)
[2:24 PM]
If dot product = 0, then light is not able to reach that face

*/

#include "Util/Math/Vector2.h"

#include "Util/Log.h"
#include "instPGE.h"

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

	this->r = new dorkestRenderer();
	this->r->cam = new dorkestCamera();
	this->scene = new Scene();

	//debug("Initializing camera to " + std::to_string());
	r->cam->setScreenSize(instPGE::getInstance()->GetWindowSize().x, instPGE::getInstance()->GetWindowSize().y);
	r->cam->setCenter(r->cam->getScreenWidth() / 2.0f, r->cam->getScreenHeight() / 2.0f, 0);
	r->cam->setOrthogonal(instPGE::getInstance()->GetWindowSize().x, instPGE::getInstance()->GetWindowSize().y, 0, 1);

	//cam->setViewport(instPGE::getInstance()->GetScreenPixelSize().x, instPGE::getInstance()->GetScreenPixelSize().y);

	dorkestSpriteMan* dsm = dorkestSpriteMan::getInstance();
	dsm->addNewSpriteSheet("BasicISO", "./res/ISO/BasicIso.png");
	dsm->makeDecal("BasicISO");

	dsm->makeDorkestSprite_Decal("basicCube", olc::WHITE, "BasicISO", 1, { 32,32 }, { 64,0 });
	dsm->makeDorkestSprite_Decal("basicPlane", olc::WHITE, "BasicISO", 1, { 32,32 }, { 96,0 });
	dsm->makeDorkestSprite_Decal("wireCube", olc::WHITE, "BasicISO", 1, { 32,32 }, { 0,0 });

	dsm->makeDorkestSprite_Decal("oCube", olc::WHITE, "BasicISO", 1, { 32,32 }, { 0,0 });
	dsm->makeDorkestSprite_Decal("bpPlane", olc::WHITE, "BasicISO", 1, { 32,32 }, { 32,0 });

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
		AABB<float> bb = AABB<float>(b.wPos, { 1,1,1 });
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
void Engine::doKeys(float fElapsedTime) {

	if (ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantCaptureKeyboard) {
		return;
	}


	if (instPGE::getInstance()->GetKey(olc::Key::M).bPressed) {
		for (int x = 0; x < 16; x++)
			for (int y = 0; y < 16; y++) {
				debug("Creating cube at " + std::to_string(x) + ", " + std::to_string(y));
				dorkestBaseEntity* entp = this->scene->createNewEntity();
				entp->addComponent<c_baseColor>(olc::GREEN);
				entp->addComponent<c_sprite>("oCube");
				Vector3f wPos = Vector3f(x, y, this->toolwin->getZ());
				Vector2i sPos = r->MapToScreen({ 32, 32 }, 1, wPos, { 0,0 });
				entp->addComponent<c_position>(wPos, sPos);
				this->ents.push_back(entp);
				debug("DONE!");

			}
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
		r->cam->setCenter(r->cam->getScreenWidth() / 2.0f, r->cam->getScreenHeight() / 2.0f, 0);
	}

	if (instPGE::getInstance()->GetKey(olc::Key::X).bPressed) {
		r->cam->setCenter(0, 0, 0);

	}

	if (instPGE::getInstance()->GetKey(olc::Key::H).bPressed) {

	}



	if (instPGE::getInstance()->GetKey(olc::Key::Z).bPressed) {
		debug(dorkestProfiler::getInstance()->viewDataPoint("CreateNewEntity"));
		debug(dorkestProfiler::getInstance()->viewDataPoint("RenderEntities"));
		debug(dorkestProfiler::getInstance()->viewDataPoint("SortEntities"));
		debug(dorkestProfiler::getInstance()->viewDataPoint("DoFrame"));
	}
	else if (instPGE::getInstance()->GetKey(olc::Key::Z).bHeld) {

	}

	if (instPGE::getInstance()->GetKey(olc::Key::V).bPressed) {
		//DrawGrid();
	}
	else if (instPGE::getInstance()->GetKey(olc::Key::V).bHeld) {
		//DrawGrid();
	}

	if (instPGE::getInstance()->GetKey(olc::Key::DEL).bPressed) {

	}

	static Vector2d dragpos;
	if (instPGE::getInstance()->GetMouse(2).bPressed) {
		dragpos = vMouse;
	}
	else if (instPGE::getInstance()->GetMouse(2).bHeld) {
		//if (instPGE::getInstance()->GetMouse(1).bHeld) {
		Vector2d mpv = Vector2d(vMouse) - dragpos;// dragpos;//Vector2d(dragpos.x - (double)mp.x, dragpos.y - (double)mp.y);
		Vector2d cp = olc::vd2d(r->cam->getCenter());
		cp += (mpv * 3) * fElapsedTime;
		//cp.lerp(fElapsedTime,dragpos);
		debug(cp.toStr());
		r->cam->setCenter(cp.x, cp.y, 0);

	}

	if (instPGE::getInstance()->GetMouse(0).bPressed) {



	};



	if (instPGE::getInstance()->GetMouse(1).bPressed) {

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
		
	} else if (instPGE::getInstance()->GetMouseWheel() > 1) {
		
	}
	

}

bool Engine::OnUserUpdate(float fElapsedTime)
{
	
	vMouse = instPGE::getInstance()->GetMousePos();
	vMouseMap = r->ScreenToMap({ 32,16 }, 1, vMouse, { 0,0 });

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
	r->doRender(fElapsedTime);
	scene->doFrame(fElapsedTime);

	this->toolwin->render();
	return true;
};

bool Engine::OnUserDestroy() {
	olc::SOUND::DestroyAudio();
	return true;
};