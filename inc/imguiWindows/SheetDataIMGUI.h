#pragma once
#include <imgui.h>
#include <imgui-SFML.h>
class sheetData {
public:

	void render(Vector2i windowSize)
	{
	
		if (this->vis) {
	
			*open = true;
			this->Draw("Sheet Data", open, windowSize);
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

	sheetData() {
		open = new bool(false);

		sX = 16;
		sY = 16;
		padding = 0;
		tspriteX = sX;
		tsY = sY;
		tpadding = 0;
		vis = false;
	}

	int sX, sY, padding;
	int tspriteX, tsY, tpadding;
	bool valchanged = false;

private:

	bool vis;
	bool* open;

	void    Draw(const char* title, bool* p_open,Vector2i w) {
		ImGui::SetNextWindowSize(ImVec2(400, 125), ImGuiCond_Always);// ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowPos(ImVec2((w.x/2) - 200, (w.y/2)-62), ImGuiCond_Always);
		if (!ImGui::Begin(title, p_open))
		{
			ImGui::End();
			return;
		}
		ImGui::InputInt("Sprite Size X", &tspriteX);
		ImGui::InputInt("Sprite Size Y", &tsY);
		ImGui::InputInt("Cell Padding", &tpadding);
		ImGui::Indent(280 / 2);
		if (ImGui::Button("OK", { 100 - 20,20 })) {
			sX = tspriteX;
			sY = tsY;
			padding = tpadding;
			valchanged = true;
			*p_open = false;
			hide();
			ImGui::End();
			return;
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel", { 100 - 20,20 })) {
			*p_open = false;
			hide();
			ImGui::End();
			return;
		}



		ImGui::End();
	}
};