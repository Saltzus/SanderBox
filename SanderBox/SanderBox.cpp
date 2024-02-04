#include "game.h"
#include "pixel.h"
#include "camera.h"

int main()
{
    int ScrWidth = 640, ScrHeight = 480;

    Game game("SanderBox", ScrWidth, ScrHeight);
    Camera camera(ScrWidth,ScrHeight,glm::vec3(0.f,0.f,-4.f));

    Pixel pixel(1);

    Shader shader("default.vert", "default.frag");



    double delta_time = 0.0f;
    double last_frame = 0.0f;

    while (!glfwWindowShouldClose(game.window))
    {
        double current_frame = glfwGetTime();
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        // Handles camera inputs
        camera.Inputs(game.window, delta_time);
        // Updates and exports the camera matrix to the Vertex Shader
        camera.updateMatrix(45.0f, 0.1f, 100.0f);


        //Draws pixel
        pixel.draw(shader, camera);
        pixel.translation = glm::vec3(0.f, 0.f, 0.f);

 
        game.display();
    }
    

    game.cleanup();

    return 0;
}
