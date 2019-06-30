#include <iostream>
#include "Shader.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void framebuffer_size_callback(GLFWwindow*, int, int);
void mouse_callback(GLFWwindow*, double, double);
void scroll_callback(GLFWwindow*, double, double);
void processInput(GLFWwindow*);

bool firstMouse = true;

const int WND_WIDTH = 800;
const int WND_HEIGHT = 600;

float last_x = WND_WIDTH / 2;
float last_y = WND_HEIGHT / 2;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

Camera cam(glm::vec3(0.0f, 0.0f, 3.0f));

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WND_WIDTH, WND_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	// load open gl function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, WND_WIDTH, WND_HEIGHT);

	//register the callback for when the window is resized
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	
	// for VBO usage
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	/////////////////
	// VAO, VBO, EBO
	/////////////////

	//vertex array object : this object is necessary and help reduce api calls
	// stores the calls to vertex attrib pointer for reuse
	//here we only have one buffer ; could have several buffers by using an array
	unsigned int color_VAO;
	glGenVertexArrays(1, &color_VAO);
	glBindVertexArray(color_VAO);

	//prepare graphics memory : vertex buffer object
	//here we only have one buffer ; could have several buffers by using an array
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	int stride = (3+2) * sizeof(float);

	//-> link the coordinates attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);

	//unbind as the above (call to glVertexAttribPointer) have done the necessary registrations
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//vao and vbo linking for the light
	unsigned int light_VAO;
	glGenVertexArrays(1, &light_VAO);
	glBindVertexArray(light_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);

	///////////////////
	// shaders
	////////////////////

	Shader colorShader("4.1.color.vs","4.1.color.fs");
	Shader lightShader("4.1.light.vs","4.1.light.fs");

	/////////////
	// main loop
	/////////////

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_DEPTH_TEST);

	glm::vec3 cubePositions[] = {
					glm::vec3(-1.0f, -0.0f, -0.0f),
					glm::vec3(2.0f, 4.0f, -8.0f),
	};

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glm::vec3 lightColor(1.0f, 1.0f, 1.0f);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		float currentFrame = (float) glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//rendering commands
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		colorShader.use();
		colorShader.setVec3("objectColor", 1.0f, 0.5f, 0.3f);
		colorShader.setVec3("lightColor", lightColor.x, lightColor.y, lightColor.z);
		glm::mat4 projection = glm::perspective(glm::radians(cam.Zoom), (float)WND_WIDTH / (float)WND_HEIGHT, 0.1f, 100.0f);
		colorShader.setMat4("projection", glm::value_ptr( projection));

		// camera/view transformation
		glm::mat4 view = cam.GetViewMatrix();
		colorShader.setMat4("view", glm::value_ptr(view));

		// render boxes
		glBindVertexArray(color_VAO);
		for (unsigned int i = 0; i < 1; i++)
		{
			// calculate the model matrix for each object and pass it to shader before drawing
			glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			colorShader.setMat4("model", glm::value_ptr(model));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// render boxes
		lightShader.use();
		lightShader.setVec3("lightCol", lightColor.x, lightColor.y, lightColor.z);
		colorShader.setMat4("projection", glm::value_ptr(projection));
		colorShader.setMat4("view", glm::value_ptr(view));
		for (unsigned int i = 1; i < 2; i++)
		{
			// calculate the model matrix for each object and pass it to shader before drawing
			glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			lightShader.setMat4("model", glm::value_ptr(model));

		glBindVertexArray(light_VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &color_VAO);
	glDeleteVertexArrays(1, &light_VAO);
	glDeleteBuffers(1, &VBO);
	//glDeleteBuffers(1, &EBO);

	glfwTerminate();

	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		cam.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		cam.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		cam.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		cam.ProcessKeyboard(RIGHT, deltaTime);
}

	void mouse_callback(GLFWwindow* window, double x, double y)
	{
		if (firstMouse)
		{
			last_x = x;
			last_y =! y;
			firstMouse = false;
		}

		float xoffset = x - last_x;
		float yoffset = last_y - y; // reversed since y-coordinates go from bottom to top

		last_x = x;
		last_y = y;
		cam.ProcessMouseMovement(xoffset, yoffset, true);
	}

	void scroll_callback(GLFWwindow* w, double x, double y)
	{
		cam.ProcessMouseScroll(y);
	}