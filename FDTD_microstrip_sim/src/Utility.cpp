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


glm::vec3 Utility::screenToNDC(GLFWwindow* window, float x, float y, float z)
{
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    extern int _initialWindowWidth;
    extern int _initialWindowHeight;
    float y_delta_ndc_scaled = (float) height / _initialWindowHeight * 2.0f;
    return glm::vec3(((float) width / _initialWindowWidth) * (2.0f * x) / width - 1.0f, 
        -1.0f + (height - y) * y_delta_ndc_scaled / height,
        z);
    //return glm::vec3((2.0f * x) / width - 1.0f, 1.0f - (2.0f * y) / height, z);
}

glm::vec4 Utility::NDCToHCC(glm::vec3 v)
{
    return glm::vec4(v.x, v.y, -1.0f, 1.0f);
}

glm::vec4 Utility::clipToEyeC(glm::vec4 v)
{
    extern MainScene* _scene_main;;
    glm::vec4 rayEye = glm::inverse(_scene_main->getProjMatrix()) * v;
    //rayEye /= rayEye.w;
    return glm::vec4(rayEye.x, rayEye.y, -1.0f, 0.0f);
}


glm::vec3 Utility::eyeToWorldC(glm::vec4 v)
{
    extern MainScene* _scene_main;
    glm::vec3 rayWorld = glm::vec3(glm::inverse(_scene_main->getViewMatrix()) * v);
    return glm::normalize(rayWorld);
}


