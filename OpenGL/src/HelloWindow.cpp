#include <iostream>
#include "Shader.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void framebuffer_size_callback(GLFWwindow*, int, int);
void processInput(GLFWwindow*);

void transform(glm::mat4& t);

const int WND_WIDTH = 800;
const int WND_HEIGHT = 600;

glm::vec3 cameraPos(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp(0.0f, 1.0f, 0.0f);

float deltaTime = 0.0f;
float lastFrame = 0.0f;

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
	// for EBO usage
	unsigned int indices[] = { 
		0,1,2, 0,2,3, 1,2,4
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
	//unsigned int EBO;
	//glGenBuffers(1, &EBO);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	//linking vertex attributes
	// 1st  paramater : index which relates to "layout (location=0)"
	// 2nd : number of attributes per vertex, must be 1, 2, 3 or 4
	// 3rd : size of an attribute

	int stride = (3+2) * sizeof(float);

	//-> link the coordinates attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);


	//=>link the texture at level 2
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	//unbind as the above (call to glVertexAttribPointer) have done the necessary registrations
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	////////////////
	// Texture
	////////////////
	const int NB_TEXTURE = 2;
	unsigned int  texture[NB_TEXTURE];
	glGenTextures(NB_TEXTURE, texture);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height, nrChannels;
	unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);


	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	data = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);

	///////////////////
	// shaders
	////////////////////

	Shader shader("4.1.shader.vs","4.1.shader.fs");
	shader.use();
	shader.setInt("texture0", 0);
	shader.setInt("texture1", 1);




	/////////////
	// main loop
	/////////////

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_DEPTH_TEST);

	glm::vec3 cubePositions[] = {
					glm::vec3(-1.0f, -0.0f, -0.0f),
					glm::vec3(-1.0f, 1.0f, -2.0f),
					glm::vec3(2.0f, -2.0f, -3.0f),
	};

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		float currentFrame = (float) glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//rendering commands
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		float time = (float) glfwGetTime();
		//float greenColor = (sin(time) / 2.0f) + 0.5f;
		//int vertexColorLocation = glGetUniformLocation(shader.ID, "ourColor");
		//glUniform1f(glGetUniformLocation(shader.ID, "time"), time);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture[1]);

		shader.use();

		//glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		
		//view = glm::translate(view, glm::vec3(0.0f, 0.0f, -5.0f));
		float radius = 10.0f;
		float cam_x = radius * sin((float)glfwGetTime());
		float cam_z = radius * cos((float)glfwGetTime());
		//view = glm::lookAt(glm::vec3(cam_x, 0.0f, cam_z), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		view =  glm::lookAt(cameraPos, cameraFront + cameraPos , cameraUp);
		projection = glm::perspective(glm::radians(45.0f), (float)WND_WIDTH / WND_HEIGHT, 0.1f, 100.0f);
		
		shader.setMat4("projection", glm::value_ptr(projection));
		shader.setMat4("view", glm::value_ptr(view));

		glBindVertexArray(VAO);
		float rot = 1;
		for (int i = 0; i < 3; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			model = glm::rotate(model, rot *glm::radians(30.0f) * (float)glm::pow(-1, i) * (float)glfwGetTime() * cubePositions[i].z, glm::vec3(1.0f, 0.3f, 0.0f));
			shader.setMat4("model", glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
			//glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);

		}

		//call events et buffers swap
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
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
	float speed = 2.0f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		cameraPos += speed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		cameraPos -= speed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		cameraPos -= speed * glm::normalize(glm::cross(cameraFront, cameraUp));
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		cameraPos += speed * glm::normalize(glm::cross(cameraFront, cameraUp));
}

void transform(glm::mat4& t)
{
	t = glm::mat4(1.0f);
//	t = glm::translate(t, 0.5f * glm::vec3(sin(glfwGetTime()), 0.0f, 0.0f));
//	t = glm::rotate(t, glm::radians(80.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	t = glm::scale(t, glm::vec3(2.0f, 2.0f, 1.0f));
	
}