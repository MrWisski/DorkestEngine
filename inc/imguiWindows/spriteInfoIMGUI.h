#pragma once
#include <imgui.h>
#include <imgui-SFML.h>

#include <Util/Math/Vector2.h>
#include <array>

class spriteInfo {
public:

	void render(Vector2i windowSize)
	{

		if (this->vis) {
			*open = true;
			this->Draw("Sprite Data", open, windowSize);
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

	spriteInfo() {
		open = new bool(false);

		for (size_t x = 0; x < 16; x++) name.at(x) = std::string("").c_str()[x];
		for (size_t x = 0; x < 16; x++) altGroup.at(x) = std::string("").c_str()[x];


		locx = 0; locy = 0;
		scale = 1.0f;
		vis = false;

	}
	~spriteInfo() {

	}

	void clear() {
		for (size_t x = 0; x < 16; x++) name.at(x) = std::string("").c_str()[x];
		for (size_t x = 0; x < 16; x++) altGroup.at(x) = std::string("").c_str()[x];
		for (size_t x = 0; x < 16; x++) threeByGroup.at(x) = std::string("").c_str()[x];
		c[0] = 1;
		c[1] = 1;
		c[2] = 1;
		c[3] = 1;

		scale = 1.0f;
		is3x3 = false;
		

	}

	void setName(std::string newname) {
		for (size_t x = 0; x < 16; x++) {
			if (x < newname.length()) {
				name.at(x) = newname.c_str()[x];
			}
			else {
				name.at(x) = std::string("").c_str()[x];
			}
		}
	}

	std::string getName() {
			return std::string(name.data());
	}

	void setAltGroup(std::string newgroup) {
		for (size_t x = 0; x < 16; x++) {
			if (x < newgroup.length()) {
				altGroup.at(x) = newgroup.c_str()[x];
			}
			else {
				altGroup.at(x) = std::string("").c_str()[x];
			}
		}
	}

	std::string getAltGroup() {
		return std::string(altGroup.data());
	}

	std::string get3x3GroupName() {
		return std::string(threeByGroup.data());
	}
	void set3x3GroupName(std::string newName) {
		for (size_t x = 0; x < 16; x++) {
			if (x < newName.length()) {
				altGroup.at(x) = newName.c_str()[x];
			}
			else {
				altGroup.at(x) = std::string("").c_str()[x];
			}
		}
	}

	bool getIs3x3() { return is3x3; }
	void setIs3x3(bool newState) {
		is3x3 = newState;
	}

	std::array<char, 16> name;
	std::string namestr;
	std::array<char, 16> altGroup;
	std::array<char, 16> threeByGroup;
	bool is3x3 = false;
	std::string altGroupstr;

	int locx, locy;
	float c[4] = { 1,1,1,1 };
	float scale;

private:

	bool vis;
	bool* open;

	void    Draw(const char* title, bool* p_open, Vector2i w) {
		ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_Always);// ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowPos(ImVec2(w.x - 300, 0), ImGuiCond_Always);
		if (!ImGui::Begin(title, p_open))
		{
			ImGui::End();
			return;
		}

		ImGui::Text("Sprite Name");
		ImGui::InputText("", name.data(), 16);
		std::string t = "Location" + std::to_string(locx) + ", " + std::to_string(locy);
		ImGui::Text(t.c_str());
		ImGui::Text("Default Tint Color");
		ImGui::ColorEdit4(" ", c);
		ImGui::Text("Scale Factor");
		ImGui::InputFloat("  ", &scale);
		ImGui::Text("Alt-Group Name");
		ImGui::InputText("   ", altGroup.data(), 16);
		
		ImGui::Checkbox("Is 3x3", &is3x3);
		ImGui::Text("3x3 Group Name");
		ImGui::InputText("    ", threeByGroup.data(), 16);

		
		

		ImGui::End();
	}
};