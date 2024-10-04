#include "Utility.h"

void window_reshape_callback(GLFWwindow* window, int newWidth, int newHeight)
{
	//extern float _aspectRatio;
	//extern glm::mat4 pMat;
	//_aspectRatio = (float)newWidth / (float)newHeight; // new width&height provided by the callback
	// //glViewport(0, 0, newWidth, newHeight);
	//pMat = glm::perspective(1.0472f, _aspectRatio, 0.1f, 1000.0f);
}

void Utility::checkGLError(const char* function)
{
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR) {
		std::cerr << "OpenGL error in " << function << ": " << std::hex << err << std::endl;
	}
}

//void window_refresh_callback(GLFWwindow* window) {
//	glfwSwapBuffers(window);
//}