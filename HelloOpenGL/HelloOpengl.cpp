#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height) { glViewport(0, 0, width, height); }
void processInput(GLFWwindow* window) {
	// esc �Է½� ������ ����
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

int main() {
	// glfw �ʱ� ����
	// �ڼ��� ������ [http://www.glfw.org/docs/latest/window.html#window_hints] ����
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// ������ �ʱ�ȭ
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL) {
		std::cout << "Falied to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	// â�� ũ�Ⱑ ����� ������ �ݹ��Լ��� ������ ������ ũ�� ����
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// GLAD �ʱ�ȭ
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// render loop
	while (!glfwWindowShouldClose(window)) {	// GLFW�� �����ϵ��� ���õǾ����� Ȯ��
		/* �Է� */
		processInput(window);					// �Է� ó��

		/* ������ ��� */
		glClearColor(.2f, .3f, .3f, 1.0f);		// �÷� ���� ���� �� � �÷��� ������ ����
		glClear(GL_COLOR_BUFFER_BIT);			// �÷� ���� ����� (GL_COLOR_BUFFER_BIT�� ���� ��Ʈ ����)

		/* �̺�Ʈ Ȯ�� �� ���� ��ü */
		glfwSwapBuffers(window);				// �÷� ���� ��ü (�÷� ���۴� �ݺ� �߿� �̹����� �׸��� ȭ�� ���)
		glfwPollEvents();						// �̺�Ʈ�� �߻��ߴ��� Ȯ���ϰ� ������ ���� ������Ʈ�Ͽ� ������ �Լ� ȣ��
	}

	// ��� �ڿ� �����ϰ� ����
	glfwTerminate();
	return 0;
}