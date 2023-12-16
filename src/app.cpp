#include <cmath>
#include <iostream>

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GLFW/glfw3.h>

#include "stb_image/stb_image.h"


#include "shader.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow *window, double pos_x, double pos_y);
void scroll_callback(GLFWwindow *window, double offset_x, double offset_y);
void processInput(GLFWwindow *window);

float zoom = 0.0f;

float cam_speed = 0.05f;
glm::vec3 cam_pos(0.0f, 0.0f, 3.0f);
glm::vec3 cam_front(0.0f, 0.0f, -1.0f);
glm::vec3 cam_up(0.0f, 1.0f, 0.0f);

float last_frame = 0.0;
float delta_time = 0.0;

bool first_pos = true;
double last_pos_x;
double last_pos_y;	
float yaw = -90;
float pitch = 0;
float fov = 45;
	
int main()
{
	//GLM test
	/*
	glm::vec4 vec(1.f, 0.f, 0.f, 1.f);
	glm::mat4 trans = glm::mat4(1.f);
	trans = glm::translate(trans, glm::vec3(1.f, 1.f, 0.f));
	vec = trans * vec;
	
	
	std::cout << "x=" << vec.x << "y=" << vec.y << "z=" << vec.z << std::endl;
	*/
	//~GLM test

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

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
/*
	// Prepare data
	float vertices[] = 
	{
	//v| x		 y		z		r		g		b		s		t
		 0.5f, 	 0.5f,	0.0f,	1.0f,	0.0f,	0.0f, 	1.0f, 	1.0f,
		 0.5f,	-0.5f,	0.0f,	0.0f,	1.0f,	0.0f, 	1.0f, 	0.0f,
		-0.5f,	-0.5f,	0.0f,	0.0f,	0.0f,	1.0f, 	0.0f,	0.0f,
		-0.5f,	 0.5f,	0.0f,	1.0f,	1.0f,	0.0f,	0.0f,	1.0f
	};
*/
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

/*
	unsigned int indicies[] = 
	{
		0, 1, 3,// first triangle
		1, 2, 3 // second triange
	};
*/
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
/*	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);
*/	
	//std::cout << "sizeof(float)=" << sizeof(float) << std::endl;
	//				AttNum, values
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void *>(0));
	glEnableVertexAttribArray(0);
//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
//	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

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

	glEnable(GL_DEPTH_TEST);	


	glm::vec3 cubePositions[] = 
	{
  		glm::vec3( 0.0f,  0.0f,  0.0f),
	    glm::vec3( 2.0f,  5.0f, -15.0f),
	    glm::vec3(-1.5f, -2.2f, -2.5f),
	    glm::vec3(-3.8f, -2.0f, -12.3f),
	    glm::vec3( 2.4f, -0.4f, -3.5f),
	    glm::vec3(-1.7f,  3.0f, -7.5f),
	    glm::vec3( 1.3f, -2.0f, -2.5f),
	    glm::vec3( 1.5f,  2.0f, -2.5f),
	    glm::vec3( 1.5f,  0.2f, -1.5f),
	    glm::vec3(-1.3f,  1.0f, -1.5f)
	};


	while(!glfwWindowShouldClose(window))
	{
		float current_frame = glfwGetTime();
		if(last_frame == 0.0f)
			delta_time = 0.01f;
		else
			delta_time = current_frame - last_frame;
		last_frame = current_frame;

		// Check user inputs
		processInput(window);
		// Render
		//   Clean
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//   Render figures
		

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, tex_face);

		glBindVertexArray(VAO);
		

		// Init transforms
		//glm::mat4 trans = glm::mat4(1.0f);
		//trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
		//trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
		//trans = glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f));

		// Coordinate system transforms

/*
		glm::mat4 view = glm::mat4(1.0f);
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -6.0f + zoom));`
		view = glm::rotate(view, glm::radians(-25.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		unsigned int view_uniform_loc = glGetUniformLocation(shader.get_program_id(), "view");
		glUniformMatrix4fv(view_uniform_loc, 1.0, GL_FALSE, glm::value_ptr(view));
*/
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(fov), 1920.0f/1080.0f, 0.1f, 100.f);
		unsigned int projection_uniform_loc = glGetUniformLocation(shader.get_program_id(), "projection");
		glUniformMatrix4fv(projection_uniform_loc, 1.0f, GL_FALSE, glm::value_ptr(projection));


		for(unsigned int i = 0; i < 10; ++i)
		{
			glm::mat4 model = glm::mat4(1.0f);
//			model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * (i + 1);
			model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			unsigned int model_uniform_loc = glGetUniformLocation(shader.get_program_id(), "model");
			glUniformMatrix4fv(model_uniform_loc, 1, GL_FALSE, glm::value_ptr(model));

			float radius = 10.0f;
			float pos_x = radius * cos(glfwGetTime()); 
			float pos_z = radius * sin(glfwGetTime()); 
			glm::mat4 view;
			//view = glm::lookAt(glm::vec3(pos_x + cam_pos.x, cam_pos.y, pos_z + cam_pos.z), cam_pos + cam_front, cam_up);
			view = glm::lookAt(glm::vec3(cam_pos.x, cam_pos.y, cam_pos.z), cam_pos + cam_front, cam_up);
			unsigned int view_uniform_loc = glGetUniformLocation(shader.get_program_id(), "view");
			glUniformMatrix4fv(view_uniform_loc, 1.0, GL_FALSE, glm::value_ptr(view));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
//		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
//		unsigned int transformLoc = glGetUniformLocation(shader.get_program_id(), "transform");
//		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
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

void mouse_callback(GLFWwindow *window, double pos_x, double pos_y)
{
	float const sensitivity = 0.1f;

	if(first_pos)
	{
		last_pos_x = pos_x;
		last_pos_y = pos_y;
		first_pos = false;
	}

	double d_x = pos_x - last_pos_x;
	double d_y = last_pos_y - pos_y;

	last_pos_x = pos_x;
	last_pos_y = pos_y;

	yaw += d_x * sensitivity;
	pitch += d_y * sensitivity;


	if(pitch > 89.0f)
		pitch = 89.0f;
	if(pitch < -89.0f)
		pitch = 90.0f;

	glm::vec3 direction;
	direction.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	direction.y = sin(glm::radians(pitch));
	direction.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	cam_front = glm::normalize(direction);
}

void scroll_callback(GLFWwindow *window, double offset_x, double offset_y)
{
	fov -= (float)offset_y;

	if(fov < 1.0f)
		fov = 1.0f;
	if(fov > 45.0f)
		fov = 45.0f;
}

void processInput(GLFWwindow *window)
{
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
/*
	if(glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS)
		zoom += 0.1f;
	if(glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS)
		zoom -= 0.1f;
*/
	cam_speed = 2.5f * delta_time;
	if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cam_pos += cam_speed * cam_front;
	if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cam_pos -= cam_speed * cam_front;
	if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cam_pos -= glm::normalize(glm::cross(cam_up, cam_front)) * cam_speed;
	if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cam_pos += glm::normalize(glm::cross(cam_up, cam_front)) * cam_speed;
}
