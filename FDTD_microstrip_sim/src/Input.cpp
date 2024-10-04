#include "Input.h"

extern Camera* cam;

bool _firstMouseMovement = true;
float _lastX = INITIAL_OPENGL_CONTEXT_SCREEN_WIDTH / 2.0f;
float _lastY = INITIAL_OPENGL_CONTEXT_SCREEN_HEIGHT / 2.0f;
//float _lastX = 1.0f;
//float _lastY = 1.0f;

Input::Input(GLFWwindow* w)
{
	window = w;
}

void Input::processInput()
{
    extern float _deltaTime;
    extern bool _cameraTranslationalMotionOn;
    _cameraTranslationalMotionOn = false;
    const float cameraSpeed = 0.05f; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cam->moveForward(_deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cam->moveBackward(_deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        cam->moveLeft(_deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        cam->moveRight(_deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        extern bool _mouseRightButtonPressed;
        if (_mouseRightButtonPressed) {
            _cameraTranslationalMotionOn = true;
        }
        else {
            _cameraTranslationalMotionOn = false;
        }
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    extern bool _mouseRightButtonPressed;
    extern bool _cameraTranslationalMotionOn;
    float posXF = static_cast<float>(xpos);
    float posYF = static_cast<float>(ypos);
    if (_firstMouseMovement) 
    {
        _lastX = posXF;
        _lastY = posYF;
        _firstMouseMovement = false;
    }
    float offsetX = posXF - _lastX;
    float offsetY = _lastY - posYF; // reversed since y-coordinates go from bottom to top
    _lastX = posXF;
    _lastY = posYF;
    if (_cameraTranslationalMotionOn) {
        extern float _deltaTime;
        cam->processMouseTranslation(offsetX, offsetY, _deltaTime);
    } else if (_mouseRightButtonPressed) {
        cam->processMouseRotation(offsetX, offsetY);
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    extern float _deltaTime;
    cam->processMouseScroll(static_cast<float>(yoffset), _deltaTime);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    extern bool _mouseRightButtonPressed;
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
        _mouseRightButtonPressed = true;
    } else {
        _mouseRightButtonPressed = false;
    }
}

