#include "GL/glew.h"
#define OLC_PGE_APPLICATION
#define OLC_PGEX_DEAR_IMGUI_IMPLEMENTATION
#define PGE_GFX_OPENGL33
#define OLC_GFX_OPENGL33

#include "instPGE.h"




instPGE* instPGE::instance = 0;

bool instPGE::OnUserCreate() {
	engine = new Engine();

	//Create a new Layer which will be used for the game's hud
	m_HudLayer = CreateLayer();
	//The layer is not enabled by default,  so we need to enable it
	EnableLayer(m_HudLayer, true);


	//Create a new Layer which will be used for the game
	m_GameLayer = CreateLayer();
	//The layer is not enabled by default,  so we need to enable it
	EnableLayer(m_GameLayer, true);

	//Set a custom render function on layer 0.  Since DrawUI is a member of
	//our class, we need to use std::bind
	//If the pge_imgui was constructed with _register_handler = true, this line is not needed
	SetLayerCustomRenderFunction(0, std::bind(&instPGE::DrawUI, this));

	
	return engine->OnUserCreate();
}

void instPGE::setGameDraw(){
	SetDrawTarget((uint8_t)m_GameLayer);
}

void instPGE::setHudDraw() {
	SetDrawTarget((uint8_t)m_HudLayer);
}

bool instPGE::OnUserUpdate(float fElapsedTime) {
	//Make sure we're drawing to the proper gamelayer every frame!
	setHudDraw();
	Clear(olc::BLANK);
	setGameDraw();
	Clear(olc::BLACK);

	Log::Instance()->render();
	return engine->OnUserUpdate(fElapsedTime);
}

bool instPGE::OnUserDestroy() {
	return engine->OnUserDestroy();
}
void instPGE::DrawUI(void) {
	//This finishes the Dear ImGui and renders it to the screen
	pge_imgui.ImGui_ImplPGE_Render();
}

instPGE* instPGE::getInstance(){
	if (instPGE::instance == 0) { instPGE::instance = new instPGE(); }
	return instance;
}

instPGE::instPGE() : pge_imgui(false) {
	// Name your application
	sAppName = "Dorkest Engine";
}

