#include <cmath>
#include <iostream>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

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
		 0.5f,	-0.5f,	0.0f,	1.0f,	0.0f,	0.0f,
		-0.5f,	-0.5f,	0.0f,	0.0f,	1.0f,	0.0f,
		 0.0f,	 0.5f,	0.0f,	0.0f,	0.0f,	1.0f
	};

	unsigned int indicies[] = 
	{
		0, 1, 2 // first triangle
	};

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
	
	const char *vertex_shader_src = "#version 330 core\n"
			"layout (location = 0) in vec3 in_pos;\n"
			"layout (location = 1) in vec3 in_color;\n"
			"out vec3 color;\n"
			"void main()\n"
			"{\n"
			"	gl_Position = vec4(in_pos, 1.0);\n"
			"	color = in_color;\n"
			"}\n";

	unsigned int vertex_shader_id;
	vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader_id, 1, &vertex_shader_src, NULL);
	glCompileShader(vertex_shader_id);

	int compile_success;
	char log_data[512];
	glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &compile_success);
	if(!compile_success)
	{
		glGetShaderInfoLog(vertex_shader_id, 512, NULL, log_data);
		std::cout << log_data << std::endl;
	}

	const char *fragment_shader_src = "#version 330 core\n"
		"out vec4 out_color;\n"
		"in vec3 color;\n"
		"void main()\n"
		"{\n"
		"	out_color = vec4(color, 1.0);\n"
		"}\n";
	unsigned int fragment_shader_id;
	fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader_id, 1, &fragment_shader_src, NULL);
	glCompileShader(fragment_shader_id);

	glGetShaderiv(fragment_shader_id, GL_COMPILE_STATUS, &compile_success);
	if(!compile_success)
	{
		glGetShaderInfoLog(fragment_shader_id, 512, NULL, log_data);
		std::cout << log_data << std::endl;
	}

	unsigned int shader_program_id;
	shader_program_id = glCreateProgram();
	glAttachShader(shader_program_id, vertex_shader_id);
	glAttachShader(shader_program_id, fragment_shader_id);
	glLinkProgram(shader_program_id);

	glGetProgramiv(shader_program_id, GL_COMPILE_STATUS, &compile_success);
	if(!compile_success)
	{
		glGetProgramInfoLog(shader_program_id, 512, NULL, log_data);
		std::cout << log_data << std::endl;
	}

	glDeleteShader(vertex_shader_id);
	glDeleteShader(fragment_shader_id);

	//std::cout << "sizeof(float)=" << sizeof(float) << std::endl;
	//				AttNum, values
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void *>(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//DEBUG INFO
	int attributes_count;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &attributes_count);
	std::cout << attributes_count << std::endl;

	while(!glfwWindowShouldClose(window))
	{
		// Check user inputs
		processInput(window);
		// Render
		//   Clean
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//   Render figures
		glUseProgram(shader_program_id);
		glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
		

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
