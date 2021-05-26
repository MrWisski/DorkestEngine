#pragma once
#include "olcPixelGameEngine.h"
#include "Extensions\olcPGEX_SplashScreen.h"
#include "Extensions\olcPGEX_Camera2D.h"
//The OLC Sound Extension
#include "Extensions\olcPGEX_Sound.h"
//The OLC AudioConversion Extension
#include "Extensions\olcPGEX_AudioConverter.h"

#include "Util/Math/Vector3.h"
#include "Util/Math/Vector2.h"
#include "Util/Math/Geometry/Ray.h"
#include "World/Block.h"
#include "Render/dorkestCamera.h"
#include "Render/dorkestRenderer.h"
#include "imguiWindows/ConsoleIMGUI.h"
#include "imguiWindows/DebugIMGUI.h"
#include <vector>
#include "ECS/Scene.h"
class dorkestRenderer;
class Block;


class Engine 
{
public:
	dorkestRenderer* r;
	DebugStuff* toolwin;
	Scene* scene;

	std::vector<Block> curStack;

	std::map<olc::vi2d, olc::Pixel> pMap;
	AppConsole console;

	olcPGEX_SplashScreen* splashScreen;
	olc::SOUND::AudioSample* as;
	int m_sample;

	std::vector<dorkestBaseEntity*> ents;

	bool skipSplash = false;
	bool toggleWireFrame = false;
	bool toggleNormal = false;
	bool toggleSprite = true;

	int count;
	bool toggle;
	olc::Pixel cellcolor;
	Vector2i vMouse;
	Vector2i vMouseMap;

	int moveposx = 0;
	int moveposy = 0;


	bool pressAnyKey();
	bool OnUserCreate();
	
	void DrawGrid();
	void drawTree();
	bool sort(Block lhs, Block rhs);
	
	void renderBlocks();
	void renderRays();
	void doKeys(float fElapsedTime);

	bool OnUserUpdate(float fElapsedTime);
	bool OnUserDestroy();
};