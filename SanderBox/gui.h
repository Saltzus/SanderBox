#pragma once
#include "game.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"


class Gui
{
public:
	Gui(Game game);
	~Gui();

	void draw();
private:

};
