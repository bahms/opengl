#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow*, int, int);
void processInput(GLFWwindow*);
int createShader(const char*, char);
int createProgram(const unsigned int*, int);

const int WND_WIDTH = 800;
const int WND_HEIGHT = 600;

const char* vertexShaderSource = R"(
#version 410
layout ( location = 0 ) in vec3 pos;
void main()
{
	gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);
}
)";

const char* fragShaderSource = R"(
#version 410
out vec4 outColor;
void main()
{
	outColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}
)";

const char* vertexShaderSourcer_yellow = R"(
#version 410
layout ( location = 0 ) in vec3 pos;
void main()
{
	gl_Position = vec3( pos, 1.0);
}
)";

const char* fragShaderSource_yellow = R"(
#version 410
out vec4 outColor;
void main()
{
	outColor =  vec4(1.0f, 1.0f, 0.0f, 1.0f);	
}
)";

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


	/////////////////
	// VAO, VBO for the first triangle
	/////////////////
	float vertices_1[] = { -0.5f, 0.5f, 0.0f,
					 0.5f, -0.5f, 0.0f,
					 -0.5f, -0.5f, 0.0f
	};

	unsigned int VAO_1;
	glGenVertexArrays(1, &VAO_1);
	glBindVertexArray(VAO_1);

	unsigned int VBO_1;
	glGenBuffers(1, &VBO_1);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_1), vertices_1, GL_STATIC_DRAW);

	int NB_ATTR_PER_VERTEX = 3;
	glVertexAttribPointer(0, NB_ATTR_PER_VERTEX, GL_FLOAT, GL_FALSE, NB_ATTR_PER_VERTEX * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	/////////////////
// VAO, VBO for the second triangle
/////////////////

	float vertices_2[] = {
					0.0f, 0.5f, 0.0f,
					0.5f, 0.5f, 0.0f,
					0.5f, 0.0f, 0.0f
	};
	unsigned int VAO_2;
	glGenVertexArrays(1, &VAO_2);
	glBindVertexArray(VAO_2);

	unsigned int VBO_2;
	glGenBuffers(1, &VBO_2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_2), vertices_2, GL_STATIC_DRAW);


	NB_ATTR_PER_VERTEX = 3;
	glVertexAttribPointer(0, NB_ATTR_PER_VERTEX, GL_FLOAT, GL_FALSE, NB_ATTR_PER_VERTEX * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//unbind as the above (call to glVertexAttribPointer) have done the necessary registrations
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// first triangle

	unsigned int  vertexShader = createShader(vertexShaderSource, 'V');
	unsigned int fragShader = createShader(fragShaderSource, 'F');

	unsigned int shaders[] = { vertexShader, fragShader };
	unsigned int shaderProgram_1 = createProgram(shaders , 2);

	//second triangle

	unsigned int yellow_vertexShader = createShader(vertexShaderSourcer_yellow, 'V');
	unsigned int yellow_fragShader = createShader(fragShaderSource_yellow, 'F');
	unsigned int shaders_2[] = { yellow_vertexShader, yellow_fragShader };
	unsigned int shaderProgram_2 = createProgram(shaders_2, 2);


	// ==> after having linked shader to the program, the shader can be deleted
	glDeleteShader(vertexShader);
	glDeleteShader(fragShader);
	glDeleteShader(yellow_vertexShader);
	glDeleteShader(yellow_fragShader);

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

		glUseProgram(shaderProgram_1);
		glBindVertexArray(VAO_1);
		glDrawArrays(GL_TRIANGLES, 0, 3); 

		glUseProgram(shaderProgram_2);
		glBindVertexArray(VAO_2);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		//call events et buffers swap
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO_1);
	glDeleteVertexArrays(1, &VAO_2);
	glDeleteBuffers(1, &VBO_1);
	glDeleteBuffers(1, &VBO_2);

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

int createShader(const char* shaderSource, char shaderType)
{

	int shader = glCreateShader(shaderType=='V' ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER);
	glShaderSource(shader, 1, &shaderSource,NULL);
	glCompileShader(shader);
	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR:SHADER:TYPE=(" << shaderType << "): " << infoLog << std::endl;
		return -1;
	}
	return shader;
}


int createProgram(const unsigned int* shaderList, int shaderCount)
{
	unsigned int program = glCreateProgram();
	for (int i = 0; i < shaderCount; i++)
	{
		glAttachShader(program, shaderList[i]);
	}
	glLinkProgram(program);
	int success;
	char infoLog[512];
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		std::cout << "ERROR:SHADER:PROGRAM:" << infoLog << std::endl;
		return -1;
	}
	return program;
}