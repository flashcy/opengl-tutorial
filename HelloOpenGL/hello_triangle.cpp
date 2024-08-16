#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// ������ ũ�� ����
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Vertex Shader �ҽ��ڵ�
const char* vertexShaderSource = "#version 330 core\n"			// �⺻������ HLSL�� ���� �������� �����Ѵ�.
	"layout (location = 0) in vec3 aPos;\n"						// layout���� ������ �Ӽ� index�� �����Ѵ�. in�� �Է����� ������ �������̴�.
	"void main() {\n"											// Vertex Buffer Object(VBO)���� index�� �����Ǵ� ���� ��� location�̴�.
	"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"		// ���������� ����Ǿ� �ִ� gl_Position�� ��ġ �����͸� �ִ´�. 
	"}\0";														// vec4(aPos, 1.0f); Ȥ�� vec4(aPos.xyz, 1.0f);�� ���� �ǹ��̴�.

// Fragment Shader �ҽ��ڵ�
const char* fragmentShaderSource = "#version 330 core\n"		// Fragment Shader�� �ȼ��� �÷� ���� �ַ� �ٷ��.
	"out vec4 FragColor;\n"										// out���� ��� �����͸� �����Ѵ�.
	"void main() {\n"
	"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}\n\0";

int main() {
	// glfw : initialize and configure
	// -------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad : load all OpenGL function pointers
	// ----------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// build and compile a shader program
	// ----------------------------------
	// vertex shader
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);	// vertex shader�� �����ϰ�, �� Ű ���� �����Ѵ�.
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);		// ������ �ۼ��� ���̴� �ڵ�� ���̴��� �����Ѵ�. 1�� ���ڿ��� �����̴�(�ҽ��ڵ� ���ڿ� �ϳ�).
	glCompileShader(vertexShader);									// ���̴� �ڵ带 �������Ѵ�.
	// ������ ���� Ȯ�� �ڵ� (cmd�� ��µ�)
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// fragment shader ���� ���� ������� �����Ѵ�.
	// fragment shader
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// ������ ���� Ȯ��
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	
	// �� ���̴��� link �Ѵ�.
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);		// glAttachShader�� ������ ���̴��� �����ϰ�,
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);						// glLinkProgram���� ��ŷ�� �Ѵ�.
	// ��ŷ ���� Ȯ��
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);	// Shaderiv�� �ٸ� �Լ��ӿ� �����Ѵ�.
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);	// ���������� ���̴� ���� �ٸ��� ProgramInfo�� �����;��Ѵ�.
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	// Program�� ��ŷ�� �Ϸ�� ���̴��� Delete ���ش�.
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		-0.5f, -0.5f, 0.0f, // left
		 0.5f, -0.5f, 0.0f, // right
		 0.0f,  0.5f, 0.0f  // top
	};

	unsigned int VBO, VAO;
	// VAO�� VBO�� ������ ��Ĵ�� �����Ѵ�.
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// VAO -> VBO�� ����Ǵ� ���. ���� VAO�� ���� bind �� ��, VBO�� bind �� �����͸� �����Ѵ�.
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// glBufferData�� ������ ���ڴ� Draw ����̴�. [GL_DYNAMIC_DRAW, GL_STREAM_DRAW, GL_STATIC_DRAW, ...]

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);	// (index, num, type, normalize, stride, offset)
	glEnableVertexAttribArray(0);	// (index)
	// index�� �ռ� ���̴� �ڵ忡�� location, �� �Ӽ��� index�� �ǹ��Ѵ�. �տ��� ��ġ�� location=0���� �����Ͽ����� 0���� ����.
	// ���� ��ġ�� vec3�̴�. �̴� float �� ���̹Ƿ� stride(���հ�)�� float * 3���� �������ش�.
	// gl���� ���� �迭�� stride ��ŭ ��� �Է����� ����ó���Ѵ�.

	// unbind (for safe)
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// ���̾����������� �� �� �ְ� ���ִ� �Լ�
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window)) {
		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// draw a triangle
		glUseProgram(shaderProgram);		// ���̴���
		glBindVertexArray(VAO);				// ������ ������ VAO ��� �Է��� �ְ�,
		glDrawArrays(GL_TRIANGLES, 0, 3);	// �׸���.
		//glBindVertexArray(0); // no need to unbind it every time

		// glfw : swap buffers and poll IO events
		// --------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}