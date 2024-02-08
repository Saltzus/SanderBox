#include "pixel.h"

Pixel::Pixel(int posx, int posy)
{
    Pixel::posx = posx;
    Pixel::posy = posy;
    //----------------Types-----------------\\

    const char* picture;





    //----------------Model stuff-----------------\\

    float vertices[] = {
     //  Positions                Colors                                  TexCoords
         0.5f,  0.5f,  0.5f,    //color.x, color.y, color.z, color.w,   //0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,    //color.x, color.y, color.z, color.w,   //0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,    //color.x, color.y, color.z, color.w,   //1.0f, 0.0f,
         0.5f, -0.5f, -0.5f,    //color.x, color.y, color.z, color.w,   //1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,    //color.x, color.y, color.z, color.w,   //0.0f, 0.0f,
         0.5f,  0.5f, -0.5f,    //color.x, color.y, color.z, color.w,   //1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,    //color.x, color.y, color.z, color.w,   //0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f     //color.x, color.y, color.z, color.w    //1.0f, 1.0f,
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

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // color attribute
    //glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
    //glEnableVertexAttribArray(1);

    // texture coord attribute
    //glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    //glEnableVertexAttribArray(2);


    /*
    //----------------Texture stuff-----------------\\
    
    // generate textures
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
 
    unsigned char* data = stbi_load(picture, &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else std::cout << "Failed to load texture" << std::endl;
 
    stbi_image_free(data);

    */
   
}

Pixel::~Pixel()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void Pixel::draw(Shader shader, Camera camera, int type)
{
    shader.use();

    switch (type)
    {
    case 0: //picture = "resources/textures/PixelText.png";
        return;
        break;
    case 1: //picture = "resources/textures/Logo.png";
        color = {0.0f, 1.f, 0.0f, 0.1f};
        break;
    case 2: //picture = "resources/textures/Logo.png";
        color = { 0.51f, 0.337f, 0.251f, 1.0f};
        break;
    default: //picture = "resources/textures/PixelText.png";
        break;
    }


    if (camera.EditMode)
        scale = glm::vec3(0.7f, 0.7f, 0.7f);
    else
        scale = glm::vec3(1.f, 1.f, 1.f);

    //If not makes pixel spin
    glm::mat4 model = glm::mat4(1.0f);

    model = glm::translate(model, translation);

    model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

    model = glm::scale(model, scale);


    unsigned int modelLoc = glGetUniformLocation(shader.ID, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);

    unsigned int camLoc = glGetUniformLocation(shader.ID, "camera");
    glUniformMatrix4fv(camLoc, 1, GL_FALSE, &camera.cameraMatrix[0][0]);

    unsigned int colLoc = glGetUniformLocation(shader.ID, "color");
    glUniform4f(colLoc, (float)color.r, (float)color.g, (float)color.b, (float)color.a);


    //glBindTexture(GL_TEXTURE_2D, texture);


    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    
}