#include <cmath>
#include <iostream>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include "stb_image/stb_image.h"

#include "shader.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if(window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return -1;;
	}
	glfwMakeContextCurrent(window);

	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	glViewport(0, 0, 800, 600);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Prepare data
	float vertices[] = 
	{
	//v| x		 y		z		r		g		b		s		t
		 0.5f, 	 0.5f,	0.0f,	1.0f,	0.0f,	0.0f, 	1.0f, 	1.0f,
		 0.5f,	-0.5f,	0.0f,	0.0f,	1.0f,	0.0f, 	1.0f, 	0.0f,
		-0.5f,	-0.5f,	0.0f,	0.0f,	0.0f,	1.0f, 	0.0f,	0.0f,
		-0.5f,	 0.5f,	0.0f,	1.0f,	1.0f,	0.0f,	0.0f,	1.0f
	};

	unsigned int indicies[] = 
	{
		0, 1, 3,// first triangle
		1, 2, 3 // second triange
	};

	// Init texture

	stbi_set_flip_vertically_on_load(true);

	unsigned int tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int tex_width, tex_height, tex_channels;
	unsigned char *tex_data = stbi_load("textures/container.jpg", &tex_width, &tex_height, &tex_channels, 0); 
	if(tex_data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex_width, tex_height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex_data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(tex_data);

	unsigned int tex_face;
	glGenTextures(1, &tex_face);
	glBindTexture(GL_TEXTURE_2D, tex_face);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int tex_face_width, tex_face_height, tex_face_channels;
	unsigned char *tex_face_data = stbi_load("textures/awesomeface.png", &tex_face_width, &tex_face_height, &tex_face_channels, 0);
	if(tex_face_data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex_face_width, tex_face_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex_face_data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(tex_face_data);

	// VAO (Vertex Array Object) (Handle to VBO and VertexAttributes)
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// VBO (Vertex Buffer Object)
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// EBO (Elements Buffer Object)
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);
	
	//std::cout << "sizeof(float)=" << sizeof(float) << std::endl;
	//				AttNum, values
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void *>(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void *>(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Load & Compile Shaders
	Shader shader("shaders/vertex.glsl", "shaders/fragment.glsl");

	//DEBUG INFO
	int attributes_count;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &attributes_count);
	std::cout << attributes_count << std::endl;

	shader.use();
	//glUniform1i(glGetUniformLocation(shader.get_program_id(), "tex_box"), 0); // set it manually
	//glUniform1i(glGetUniformLocation(shader.get_program_id(), "tex_face"), 1); // set it manually
	shader.set_uniform("tex_box", 0); // or with shader class
	shader.set_uniform("tex_face", 1); // or with shader class

	while(!glfwWindowShouldClose(window))
	{
		// Check user inputs
		processInput(window);
		// Render
		//   Clean
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//   Render figures
		

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, tex_face);

		glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		

		// Swap & Poll events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	std::cout << "Window size changed. width: " << width << "; height: " << height << std::endl;
}

void processInput(GLFWwindow *window)
{
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}
