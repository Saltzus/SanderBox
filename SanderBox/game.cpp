#include "game.h"
#include "gui.h"

#include <Windows.h>;

// Vector for all the pixels
std::vector < Pixel* > pixels;

Game::Game(const char* WindowTitle, int WindowWidth, int WindowHeight)
{
	// Loads the map from resources/saves/Defaultsave.txt
	loadMap();

	
	// Initializes the glfw window
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Creates the glfw window
	window = glfwCreateWindow(WindowWidth, WindowHeight, WindowTitle, NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}

	glfwMakeContextCurrent(window);

	// loads glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	// Enables debt test and culling
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}

Game::~Game()
{
	// Deletes all the pixels
	for (size_t i = 0; i < pixels.size(); i++)
		delete pixels[i];
}

void Game::display()
{
	// Swaps buffers and gets inputs
	glfwSwapBuffers(window);
	glfwPollEvents();

	// Sets backround color and clears buffers
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Game::cleanup()
{
	// Destroys glfw window
	glfwDestroyWindow(window);
}


void Game::initMap()
{
	// Creates cubes for the whole map and sets their positions
	for (size_t i = 0; i < Width; i++)
	{
		int d = 0 - i;

		for (size_t j = 0; j < Height; j++)
		{
			Pixel* pixel = new Pixel(i,j);
			pixel->translation.y = d;
			pixel->translation.x = j;

			pixels.push_back(pixel);
		}
	}
}

void Game::renderMap(Shader shader, Camera camera, double deltatime)
{
	// Draws every pixel
	for (size_t i = 0; i < pixels.size(); i++)
	{
		pixels[i]->draw(shader, camera, Map[pixels[i]->posx][pixels[i]->posy], &Map, deltatime);
	}

	// Checks if Imgui wants to use mouse
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && !ImGui::GetIO().WantCaptureMouse)
	{
		// Gets position to draw to the map and what to draw
		Map[camera.Mapy][camera.Mapx] = Gui::DrawType;
	}


}

void Game::loadMap()
{
	// Clears map so .txt files won't become too big
	Map.clear();
	std::ifstream file("resources/saves/Defaultsave.txt");

	// Sends error box if Defaultsave not found / cant open it
	if (!file.is_open()) {
		MessageBox(NULL, TEXT("Error: Unable to open file ''resources/saves/Defaultsave.txt''"), TEXT("Can't open default save"), MB_ICONERROR | MB_OK);
	}

	// Reads the data from Defaultsave
	std::string line;
	while (std::getline(file, line)) {
		std::vector<unsigned int> row;
		std::istringstream iss(line);
		std::string cell;
		while (std::getline(iss, cell, ',')) {
			row.push_back(std::stoul(cell));
		}
		Map.push_back(row);
	}

	// Closes the file
	file.close();
}

