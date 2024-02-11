#pragma once
#include "game.h"
#include "camera.h"


#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"



class Gui
{
public:
	Gui(Game* game);
	~Gui();

	static int DrawType;
	static bool UseSim;
	static bool Ortho;
	static char fileName[128];

	static float SimSpeed;

	void draw();

private:

	void saveMap(const char* path);
	void loadMap(const char* path);
};
