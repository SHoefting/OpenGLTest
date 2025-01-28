

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);


//Shaders------------------------------------------------
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"	gl_Position = vec4(aPos. x, aPos.y, aPos.z, 1.0f);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";


int main()
{
	// Initializing glfw, setting the min and maj required Versions and telling the program to use the core profile
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// creating the Window (sizing and naming)

	GLFWwindow* window = glfwCreateWindow(800, 600, "Second OpenGL Test", NULL, NULL);
	
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	//setting the Viewportsize ; could be smaller than the Window to 
	// have a 3d Viewport and some other stuff elewhere
	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glfwSwapInterval(1); // Enable VSync (1 frame per refresh)

	//setup and buid shaderprograms -----------------------------------------------------------------------------------------------------

	int success;
	char infoLog[512];

	//VertexShader ---------------------------------------------------------------------
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);


	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//FragmentShader ---------------------------------------------------------------------------
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// create ShaderProgram and Linking----------------------------------------------------------
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADERPROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	


	//deleting Shaders since we dont need them anymore
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);




	// VBO and VAO setup -------------------------------------------------------------------------------------------------------

	float vertices[] = {
	 0.5f,  0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	-0.5f, -0.5f, 0.0f,
	-0.5f,  0.5f, 0.0f
	};
	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};
	
	unsigned int VBO, VAO, EBO;
	

	//Generate a Buffer inside the Variable
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// Binding the Buffer stored in VBO to the GL_ARRAY_BUFFER Target 
	// all calls on the ARRAY_BUFFER will be used to configure VBO
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//specifies the Bufffertype, the size of the data, a reference to the data and how to draw it
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//telling OpenGL how to interpret the vertex Data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0); //Needs to be enabled? is deabled by default
	
	

	
	



	// render loop ----------------------------------------------------------------------------------------
	while (!glfwWindowShouldClose(window))
	{
		//input:
		processInput(window);

		//rendering:
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		//draw first triangle
		//Every shader and rendering call after glUseProgram will now use this program object
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO); //VertexData safed in VBO is indirectly safed in VAO
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		// check and call events and swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);

	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}