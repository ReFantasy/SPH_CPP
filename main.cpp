#ifdef _WIN32
#define GLEW_STATIC
#endif
#include "sph.h"
#include <GLFW/glfw3.h>
// key and mouse callback
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
//void MOUSE_BUTTON_CALLBACK(GLFWwindow *window, int button, int action, int mods);
//void SCROLL_CALLBACK(GLFWwindow *window, double xoffset, double yoffset);
//void CURSOR_POS_CALLBACK(GLFWwindow *window, double xpos, double ypos);

int main(int argc, char *argv[])
{
	/* Initialize the library */
	if (!glfwInit())
		return -1;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // GLFW_OPENGL_CORE_PROFILE
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	glfwWindowHint(GLFW_SAMPLES, 4);

	/* Create a windowed mode window and its OpenGL context */
	GLFWwindow* window = glfwCreateWindow(640, 640, "Base SPH", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	// event callback
	glfwSetKeyCallback(window, key_callback);

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync

	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		fprintf(stderr, "GLEW Error: %s\n", glewGetErrorString(err));
		exit(EXIT_FAILURE);
	}

	glDisable(GL_CULL_FACE);
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		//glfwSetWindowTitle(window, ("Base SPH  fps:" + std::to_string(glfwGetTimerValue())).c_str());

		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		glClearColor(0.067f, 0.184f, 0.255f, 1.0f);

		display();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}