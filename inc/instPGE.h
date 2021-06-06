#pragma once

#include "olcPixelGameEngine.h"
#include "Extensions\imgui_impl_pge.h"
#include <Extensions/olcPGEX_Graphics3D.h>

#include <Engine\Engine.h>

class Engine;

class instPGE : public olc::PixelGameEngine {
public:
	olc::imgui::PGE_ImGUI pge_imgui;
	
	int m_HudLayer;
	int m_GameLayer;
	Engine* engine;



	static instPGE* getInstance();
	bool OnUserCreate() override;
	void setGameDraw();
	void setHudDraw();
	bool OnUserUpdate(float fElapsedTime) override;
	bool OnUserDestroy() override;
	
	void DrawUI(void);

private:
	static instPGE* instance;
	instPGE();
};


