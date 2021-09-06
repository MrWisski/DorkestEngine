#pragma once
#include <imgui.h>
#include <imgui-SFML.h>

class DebugStuff {
public:

	void render()
	{

		if (this->vis) {
			*open = true;
			this->Draw("dorkestEngine Debug Tools", open);
			if (*open == false) {
				this->vis = false;
			}
		}
	}

	void show()
	{
		this->vis = true;
	}

	void hide()
	{
		this->vis = false;
	}

	void toggleShowHide()
	{
		this->vis = !this->vis;
	}

	

	sf::Color getColor() {
		return sf::Color(c[0] * 255, c[1] * 255, c[2] * 255, c[3] * 255);
	}

	int getZ() { return *zVal; }
	int getZ2() { return *zVal2; }

	float getlin() { return linear; }
	float getcon() { return constant; }
	float getquad() { return quad; }


	
	DebugStuff() {
		open = new bool(false);
		zVal = new int(0);
		zVal2 = new int(0);
	}
	~DebugStuff() {
		delete open, zVal, zVal2;
	}
private:

	float linear = 0.6192f;
	float quad = 0.062f;
	float constant = 0.05f;

	bool vis;
	bool* open;
	float c[4] = {0.5,0.5,0.5,1};
	int* zVal = 0;
	int* zVal2 = 0;

	void    Draw(const char* title, bool* p_open) {
		ImGui::SetNextWindowSize(ImVec2(200, 600), ImGuiCond_FirstUseEver);
		if (!ImGui::Begin(title, p_open))
		{
			ImGui::End();
			return;
		}
		ImGui::ColorEdit4("Object Color", c);
		ImGui::SliderInt("Z Height P1", zVal, 0, 16);
		ImGui::SliderInt("Z Height P2", zVal2, 0, 16);

		ImGui::SliderFloat("Linear", &linear, 0.0f, 1.0f,"%1.3f");
		ImGui::SliderFloat("Quadratic", &quad, 0.0f, 1.0f, "%1.3f");
		ImGui::SliderFloat("Constant", &constant, 0.0f, 1.0f, "%1.3f");



		ImGui::End();
	}
};