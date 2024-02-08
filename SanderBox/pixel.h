#pragma once
#include "camera.h"

class Pixel
{
public:
	Pixel(int posx, int posy);
	~Pixel();

	int posx, posy;

	void draw(Shader shader, Camera camera, int type);
	glm::vec4 color = { 1.f, 1.f, 1.f, 1.f };


	glm::mat4 model = glm::mat4(1.0f);
	glm::vec3 translation = glm::vec3(1.f, 0.f, 0.f);
	glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 scale = glm::vec3(0.7f, 0.7f, 0.7f);

private:
	unsigned int VBO, EBO;
	unsigned int VAO;
	unsigned int texture;
};


