#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height) { glViewport(0, 0, width, height); }
void processInput(GLFWwindow* window) {
	// esc 입력시 윈도우 종료
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

int main() {
	// glfw 초기 설정
	// 자세한 내용은 [http://www.glfw.org/docs/latest/window.html#window_hints] 참고
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// 윈도우 초기화
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL) {
		std::cout << "Falied to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	// 창의 크기가 변경될 때마다 콜백함수로 렌더링 윈도우 크기 지정
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// GLAD 초기화
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// render loop
	while (!glfwWindowShouldClose(window)) {	// GLFW가 종료하도록 지시되었는지 확인
		/* 입력 */
		processInput(window);					// 입력 처리

		/* 렌더링 명령 */
		glClearColor(.2f, .3f, .3f, 1.0f);		// 컬러 버퍼 지울 때 어떤 컬러로 지울지 지정
		glClear(GL_COLOR_BUFFER_BIT);			// 컬러 버퍼 지우기 (GL_COLOR_BUFFER_BIT로 버퍼 비트 전달)

		/* 이벤트 확인 및 버퍼 교체 */
		glfwSwapBuffers(window);				// 컬러 버퍼 교체 (컬러 버퍼는 반복 중에 이미지를 그리고 화면 출력)
		glfwPollEvents();						// 이벤트가 발생했는지 확인하고 윈도우 상태 업데이트하여 정해진 함수 호출
	}

	// 모든 자원 정리하고 삭제
	glfwTerminate();
	return 0;
}