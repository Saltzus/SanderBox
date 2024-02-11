#include "Camera.h"
#include "imgui/imgui.h"


Camera::Camera(int width, int height, glm::vec3 position)
{
	//Loads information
	Camera::width = width;
	Camera::height = height;
	Position = -position;
}

void Camera::updateMatrix(float FOVdeg, float nearPlane, float farPlane, bool ortho)
{

	// Initializes matrices since otherwise they will be the null matrix
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	// Adds perspective to the scene
	if (EditMode || ortho)
	{
		float aspect = ((float)width / height) * 20;
		projection = glm::ortho(-aspect, aspect, -aspect, aspect, nearPlane - 10, farPlane * 100);
	}
	else
		projection = glm::perspective(glm::radians(FOVdeg), (float)width / height, nearPlane, farPlane);

	
	// Makes camera look in the right direction from the right position
	view = glm::lookAt(Position, Position + Orientation, Up);




	// Sets new camera matrix
	cameraMatrix = projection * view;
}


bool TestClick = false;
bool TestClick2 = false;

glm::vec3 OldPos = { 9.2671f, 6.12119f, 32.9878f };
glm::vec3 OldRot = { 0.00817754,0.0290847,-0.999543 };
void Camera::Inputs(GLFWwindow* window, double deltatime)
{
	// Adds deltatime to movement
	speed = speed * 100 * deltatime;

	// Checks if Imgui want keyboard inputs and if m key is pressed
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS && !ImGui::GetIO().WantCaptureKeyboard)
	{
		if (TestClick == false)
		{
			// Saves 3d positions when going to edit mode and moving player for edit mode
			if (EditMode == false)
			{
				OldPos = Position;
				OldRot = Orientation;
			}
			if (EditMode == true)
			{
				Position = OldPos;
				Orientation = OldRot;
			}

			EditMode = !EditMode;
			TestClick = true;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_RELEASE)
	{
		if (TestClick == true)
			TestClick = false;
	}

	// Edit mode ----------------------------------------
	if (EditMode)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

		// Sets the position to teleport player to when in edit mode
		Position = glm::vec3(25.6871f, -21.12119f, 32.9878f);
		Orientation = glm::vec3(0,0.0,-0.00001);

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		{

			// Stores the coordinates of the cursor
			double mouseX;
			double mouseY;


			// Fetches the coordinates of the cursor
			glfwGetCursorPos(window, &mouseX, &mouseY);
			

			// Gets the coordinates for 2d input
			for (size_t i = 0; i < 52; i++)
			{
				if (mouseX >= (8 + (i*12)) && mouseX <= (8 + ((1 + i) * 12)))
				{
					// sets what grid x axis mouse is inside
					Mapx = i;
				}
			}
			for (size_t i = 0; i < 47; i++)
			{
				if (mouseY >= (46 + (i * 9)) && mouseY <= (46 + ((1 + i) * 9)))
				{
					// same but for y axis
					Mapy = i;
				}
			}
		}

		
	}
	else
	{

		// Handles key inputs
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			Position += speed * Orientation;
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			Position += speed * -glm::normalize(glm::cross(Orientation, Up));
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			Position += speed * -Orientation;
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			Position += speed * glm::normalize(glm::cross(Orientation, Up));
		}
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			Position += speed * Up;
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		{
			Position += speed * -Up;
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		{
			speed = 0.2f;
		}
		else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE)
		{
			speed = 0.07f;
		}

		// Handles mouse inputs
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
		{
			// Hides mouse cursor
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

			// Prevents camera from jumping on the first click
			if (firstClick)
			{
				glfwSetCursorPos(window, (width / 2), (height / 2));
				firstClick = false;
			}

			// Stores the coordinates of the cursor
			double mouseX;
			double mouseY;
			// Fetches the coordinates of the cursor
			glfwGetCursorPos(window, &mouseX, &mouseY);

			// Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
			// and then "transforms" them into degrees 
			float rotX = sensitivity * (float)(mouseY - (height / 2)) / height;
			float rotY = sensitivity * (float)(mouseX - (width / 2)) / width;

			// Calculates upcoming vertical change in the Orientation
			glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up)));

			// Decides whether or not the next vertical Orientation is legal or not
			if (abs(glm::angle(newOrientation, Up) - glm::radians(90.0f)) <= glm::radians(85.0f))
			{
				Orientation = newOrientation;
			}

			// Rotates the Orientation left and right
			Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);

			// Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
			glfwSetCursorPos(window, (width / 2), (height / 2));
		}
		else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
		{
			// Unhides cursor since camera is not looking around anymore
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			// Makes sure the next time the camera looks around it doesn't jump
			firstClick = true;
		}
	}


}