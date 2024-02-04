#pragma once
#include "game.h"
#include "camera.h"


class Pixel
{
public:
	Pixel(int type);
	~Pixel();

	void draw(Shader shader, Camera camera);
    
	glm::mat4 model = glm::mat4(1.0f);
	glm::vec3 translation = glm::vec3(1.f, 0.f, 0.f);
	glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);

private:
	unsigned int VBO, VAO, EBO;
	unsigned int texture;
};


