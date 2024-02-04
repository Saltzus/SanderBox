#pragma once

#include <iostream>
#include "glad/glad.h"
#include "vector"
#include "glm/glm.hpp"
#include "GLFW/glfw3.h"
#include "stb_image.h"

#include "VAO.h"
#include "EBO.h"
#include "VBO.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "shader.h"

class Game
{
public:
	Game(const char* WindowTitle, int WindowWidth, int WindowHeight);
	~Game();

	std::vector<std::vector<unsigned int>> Map;

	GLFWwindow* window;

	void display();
	void cleanup();

private:
	void framebuffer_size_callback(GLFWwindow* window, int width, int height);
};

