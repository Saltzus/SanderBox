#include "pixel.h"
#include "random"
#include "gui.h"

Pixel::Pixel(int posx, int posy)
{
    // Sets pixel start position
    Pixel::posx = posx;
    Pixel::posy = posy;





    //----------------Model stuff-----------------\\

    float vertices[] = {
     //  Positions          
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f,  0.5f,
         0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f 
    };

    unsigned int indices[] = {
     // Indices
        0, 1, 2,
        1, 3, 4,
        5, 6, 3,
        7, 3, 6,
        2, 4, 7,
        0, 7, 6,
        0, 5, 1,
        1, 5, 3,
        5, 0, 6,
        7, 4, 3,
        2, 1, 4,
        0, 2, 7
    };

    //----------------Buffer stuff-----------------\\

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // adds vertices and indices inside buffers
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


}

Pixel::~Pixel()
{
    // deletes buffers
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}


void Pixel::draw(Shader shader, Camera camera, int type, std::vector<std::vector<unsigned int>>* Map, double deltatime)
{
    shader.use();

    // creates random seed for simulation
    srand(rand());
    int ruleset;

    // Switch for all types and sets their color and simulation values
    switch (type)
    {
    case 0:
        return;
        break;
    case 1: // Grass -------------------------------
        color = {0.0f, 1.f, 0.0f, 0.1f};
        ruleset = 1;
        break;
    case 2: // Ground ------------------------------
        color = { 0.51f, 0.337f, 0.251f, 1.0f};
        ruleset = 2;
        break;
    case 3: // Sand --------------------------------
        color = { 0.922f, 0.847f, 0.337f, 1.0f };
        ruleset = 3;
        break;
    case 4: // Rock --------------------------------
        color = { 0.702f, 0.659f, 0.659f, 1.0f };
        ruleset = 0;
        break;
    case 5: // Water -------------------------------
        color = { 0.612f, 0.678f, 0.98f, 1.0f };
        ruleset = 4;
        break;
    default:
        break;
    }

    // Gets if game has simulations on or off and if it does selects sim type depending on type of pixel
    if (Gui::UseSim)
    {
        switch (ruleset)
        {
        case 0:
            break;
        case 1:// Only down ///////////////////////////////////////////////////////
            if (delay < 10)
                // Sets sim speed that you can change in settings
                delay = delay + 1*Gui::SimSpeed;

            if (posx <= 45 && delay >= 10)
            {
                if ((*Map)[posx + 1][posy] == 0 || (*Map)[posx + 1][posy] == 5)
                {
                    (*Map)[posx][posy] = (*Map)[posx + 1][posy];
                    (*Map)[posx + 1][posy] = type;
                }
                delay = 0;
            }
            break;
        case 2:// Only down but little bit faster /////////////////////////////////
            if (delay < 7)
                delay = delay + 1 * Gui::SimSpeed;
            if (posx <= 45 && delay >= 7)
            {
                if ((*Map)[posx + 1][posy] == 0 || (*Map)[posx + 1][posy] == 5)
                {
                    (*Map)[posx][posy] = (*Map)[posx + 1][posy];
                    (*Map)[posx + 1][posy] = type;
                }
                delay = 0;
            }
            break;
        case 3:// Down and down right and down left ///////////////////////////////
            if (delay < 7)
                delay = delay + 1 * Gui::SimSpeed;
            //checks if is inside bounds 
            if (posx <= 45 && delay >= 7)
            {
                if ((*Map)[posx + 1][posy] == 0 || (*Map)[posx + 1][posy] == 5)
                {
                    (*Map)[posx][posy] = (*Map)[posx + 1][posy];
                    (*Map)[posx + 1][posy] = type;
                }
                else
                {
                    if (posy <= 50 && !preferright)
                    {
                        // checks if next movement place is clear or has water
                        if ((*Map)[posx + 1][posy + 1] == 0 || (*Map)[posx + 1][posy + 1] == 5)
                        {
                            // if type 0 or 5 switches their places so sand can sink under water
                            (*Map)[posx][posy] = (*Map)[posx + 1][posy + 1];
                            (*Map)[posx + 1][posy + 1] = type;
                        }
                        preferright = true;
                    }
                    else
                    {
                        if (posy >= 1) {
                            if ((*Map)[posx + 1][posy - 1] == 0 || (*Map)[posx + 1][posy - 1] == 5)
                            {
                                (*Map)[posx][posy] = (*Map)[posx + 1][posy - 1];
                                (*Map)[posx + 1][posy - 1] = type;
                            }
                        }
                        preferright = false;
                    }
                }
                delay = 0;
            }

        case 4:// Down and down right and down left and left and right////////////////////////////////
            if (delay < 7)
                delay = delay + 1 * Gui::SimSpeed;
            if (posx <= 45 && delay >= 7)
            {
                if ((*Map)[posx + 1][posy] == 0)
                {
                    (*Map)[posx][posy] = 0;
                    (*Map)[posx + 1][posy] = type;
                }
                else
                {
                    // randomly moves left or right
                    int test = rand() & 50;
                    if (posy <= 50 && !preferright)
                    {
                        if ((*Map)[posx + 1][posy + 1] == 0 )
                        {
                            (*Map)[posx][posy] = 0;
                            (*Map)[posx + 1][posy + 1] = type;
                        }
                        else if ((*Map)[posx][posy + 1] == 0 && test <= 25)
                        {
                            (*Map)[posx][posy] = 0;
                            (*Map)[posx][posy + 1] = type;
                        }
                        preferright = true;
                    }
                    else
                    {
                        if (posy >= 1) {
                            if ((*Map)[posx + 1][posy - 1] == 0)
                            {
                                (*Map)[posx][posy] = 0;
                                (*Map)[posx + 1][posy - 1] = type;
                            }
                            else if ((*Map)[posx][posy - 1] == 0 && test >= 25)
                            {
                                (*Map)[posx][posy] = 0;
                                (*Map)[posx ][posy - 1] = type;
                            }
                        }
                        preferright = false;
                    }
                }
                delay = 0;
            }

        default:
            break;
        }
    }

    // makes cubes smaller if in edit mode
    if (camera.EditMode)
        scale = glm::vec3(0.7f, 0.7f, 0.7f);
    else
        scale = glm::vec3(1.f, 1.f, 1.f);

    //If not makes pixel spin
    glm::mat4 model = glm::mat4(1.0f);

    // Adds translarion for pixels
    model = glm::translate(model, translation);

    model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

    model = glm::scale(model, scale);

    // Sends data to shader uniforms
    unsigned int modelLoc = glGetUniformLocation(shader.ID, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);

    unsigned int camLoc = glGetUniformLocation(shader.ID, "camera");
    glUniformMatrix4fv(camLoc, 1, GL_FALSE, &camera.cameraMatrix[0][0]);

    unsigned int colLoc = glGetUniformLocation(shader.ID, "color");
    glUniform4f(colLoc, (float)color.r, (float)color.g, (float)color.b, (float)color.a);



    // Draws the pixel
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    
}

