#include "game.h"
#include "pixel.h"
#include "camera.h"
#include "gui.h"




int main()
{
    int ScrWidth = 640, ScrHeight = 480;

    Game game("SanderBox", ScrWidth, ScrHeight);
    Camera camera(ScrWidth, ScrHeight, glm::vec3(0.f,0.f,-4.f), false);


    Shader shader("resources/shaders/default.vert", "resources/shaders/default.frag");
    Shader shader2("resources/shaders/default2.vert", "resources/shaders/default2.frag");

    Gui gui(game);

    double delta_time = 0.0f;
    double last_frame = 0.0f;


    game.initMap();

    while (!glfwWindowShouldClose(game.window))
    {
        double current_frame = glfwGetTime();
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        // Handles camera inputs
        camera.Inputs(game.window, delta_time);
        // Updates and exports the camera matrix to the Vertex Shader
        camera.updateMatrix(45.0f, 0.1f, 100.0f);

        game.renderMap(shader, camera);

        //Draws pixels

        if (camera.EditMode)
        {
            gui.draw();
        }

        game.display();
    }
    


    game.cleanup();

    return 0;
}
