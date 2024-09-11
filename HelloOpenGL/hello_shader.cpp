#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// 윈도우 크기 지정
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Vertex Shader 소스코드
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"	// 하나의 in 변수를 더 지정
"out vec3 ourColor;\n"						// fragment shader로 넘겨줄 값을 저장할 변수 지정
"void main() {\n"
"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"	ourColor = aColor;\n"					// 넘겨줄 값 넣어주기
"}\0";

// Fragment Shader 소스코드
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"						// vertex shader에서 넘겨준 값 받아옴
"uniform float greenValue;\n"				// 시간에 따라 초록색에 변화를 줄 것이기에, global 변수인 uniform 타입으로 cpp에서도 조작 가능하도록 지정
"void main() {\n"
"	FragColor = vec4(ourColor.x, greenValue, ourColor.z, 1.0f);\n"		// 녹색 값만 따로 uniform으로 받아온 값으로 지정
"}\0";

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
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);	// vertex shader를 생성하고, 그 키 값을 저장한다.
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);		// 위에서 작성한 쉐이더 코드로 쉐이더를 생성한다. 1은 문자열의 개수이다(소스코드 문자열 하나).
	glCompileShader(vertexShader);									// 쉐이더 코드를 컴파일한다.
	// 컴파일 에러 확인 코드 (cmd에 출력됨)
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// fragment shader 또한 같은 방식으로 생성한다.
	// fragment shader
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// 컴파일 에러 확인
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// 두 쉐이더를 link 한다.
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);		// glAttachShader로 각각의 쉐이더를 지정하고,
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);						// glLinkProgram으로 링킹을 한다.
	// 링킹 에러 확인
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);	// Shaderiv와 다른 함수임에 유의한다.
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);	// 마찬가지로 쉐이더 때와 다르게 ProgramInfo를 가져와야한다.
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	// Program에 링킹이 완료된 쉐이더는 Delete 해준다.
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// positions			// colors
		 0.5f, -0.5f, 0.0f,		1.0f, 0.0f, 0.0f,	// bottom right
		-0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,	// bottom left
		 0.0f,  0.5f, 0.0f,		0.0f, 0.0f, 1.0f	// top
	};

	unsigned int VBO, VAO;
	// VAO와 VBO를 각각의 방식대로 생성한다.
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// VAO -> VBO로 연결되는 방식. 따라서 VAO를 먼저 bind 한 후, VBO를 bind 해 데이터를 지정한다.
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// glBufferData의 마지막 인자는 Draw 방식이다. [GL_DYNAMIC_DRAW, GL_STREAM_DRAW, GL_STATIC_DRAW, ...]

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);	// (index, num, type, normalize, stride, offset)
	glEnableVertexAttribArray(0);	// (index)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// 이번에는 Position, Color를 연속되게 나열하여 Stride와 Offset을 조절해본다.
	// Stride는 position 세 개, Color 세 개이므로 6개 만큼 준다: 6 * sizeof(float)
	// Offset은 앞 position이 먼저 나오므로 position 뒤부터 읽을 수 있도록 지정한다: 3 * sizeof(float)

	// unbind (for safe)
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// 와이어프레임으로 볼 수 있게 해주는 함수
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
		glUseProgram(shaderProgram);

		// update the uniform color
		float timeValue = glfwGetTime();												// 시간 값을 가져온다.
		float greenValue = sin(timeValue) / 2.0f + 0.5f;								// 시간 값을 사인파로 만든 뒤, [0, 1] 사이로 진폭을 가둔다.
		int vertexColorLocation = glGetUniformLocation(shaderProgram, "greenValue");	// greenValue라는 uniform 타입의 쉐이더 변수의 주소를 가져온다.
		glUniform1f(vertexColorLocation, greenValue);									// 가져온 주소에 값을 넣어준다.
		// 즉, uniform 변수 지정 후, 쉐이더 in out 지정하여 처리

		glBindVertexArray(VAO);				// 위에서 정의한 VAO 대로 입력을 넣고,
		glDrawArrays(GL_TRIANGLES, 0, 3);	// 그린다.
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