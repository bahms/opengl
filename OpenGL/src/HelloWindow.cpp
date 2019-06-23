#include <iostream>
#include "Shader.h"
#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow*, int, int);
void processInput(GLFWwindow*);

const int WND_WIDTH = 800;
const int WND_HEIGHT = 600;

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
	
	// for VBO usage
	float vertices[] = { -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
						 0.0f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
						 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f
	};
	// for EBO usage
	unsigned int indices[] = { 
		0,1,2 
	};

	/////////////////
	// VAO, VBO, EBO
	/////////////////

	//vertex array object : this object is necessary and help reduce api calls
	// stores the calls to vertex attrib pointer for reuse
	//here we only have one buffer ; could have several buffers by using an array
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);


	//prepare graphics memory : vertex buffer object
	//here we only have one buffer ; could have several buffers by using an array
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// elements buffer attributes
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	//linking vertex attributes
	// 1st  paramater : index which relates to "layout (location=0)"
	// 2nd : number of attributes per vertex, must be 1, 2, 3 or 4
	// 3rd : size of an attribute

	int COMPONENTS_PER_VERTEX_ATTRIBUTE = 3;
	int NB_ATTRIBUTES_COUNT = 2; // coordinates + color
	int stride = COMPONENTS_PER_VERTEX_ATTRIBUTE * NB_ATTRIBUTES_COUNT * sizeof(float);

	//-> link the coordinates attributes
	glVertexAttribPointer(0, COMPONENTS_PER_VERTEX_ATTRIBUTE, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);

	//=> link the color attributes
	glVertexAttribPointer(1, COMPONENTS_PER_VERTEX_ATTRIBUTE, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	//unbind as the above (call to glVertexAttribPointer) have done the necessary registrations
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	///////////////////
	// shaders
	////////////////////

	Shader shader("4.1.shader.vs","4.1.shader.fs");

	/////////////
	// main loop
	/////////////

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		//rendering commands
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		//float time = glfwGetTime();
		//float greenColor = (sin(time) / 2.0f) + 0.5f;
		//int vertexColorLocation = glGetUniformLocation(shader.ID, "ourColor");
		//glUniform4f(vertexColorLocation, 0.0f, greenColor, 0.0f, 1.0f);

		shader.use();
		glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 3); //only suitable where there are no duplicates in the data mesh
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//call events et buffers swap
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

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
}