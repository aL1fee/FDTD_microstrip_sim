#include "Input.h"

extern Camera* cam;

extern int _initialWindowWidth;
extern int _initialWindowHeight;
extern int _currentWindowWidth;
extern int _currentWindowHeight;

bool _firstMouseMovement = true;
float _lastX = INITIAL_OPENGL_CONTEXT_SCREEN_WIDTH / 2.0f;
float _lastY = INITIAL_OPENGL_CONTEXT_SCREEN_HEIGHT / 2.0f;
//float _lastX = 1.0f;
//float _lastY = 1.0f;

extern bool _testingLinePressed;
extern bool _testingLineExpected;
extern bool _selectingObjectExpected;
extern bool _selectingObjectPressed;
extern bool _resizing;
extern bool _rayExpected;

Input::Input(GLFWwindow* w)
{
	window = w;
    potentialPressedGLFWButtons = new std::vector<int>();
    inputBuffer = new InputBuffer();
}

void Input::processInput()
{
    //if (_resizing) {
    //    std::cout << "hgey" << std::endl;
    //    return;
    //}
    extern float _deltaTime;
    extern bool _cameraTranslationalMotionOn;
    _cameraTranslationalMotionOn = false;
    const float cameraSpeed = 0.05f; // adjust accordingly
    //if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE) {
    //    std::cout << "a" << std::endl;
    //}
    //if (_resizing) {
    //    return;
    //}



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
    extern MainScene* _scene_main;
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && _testingLineExpected)
    {
        _testingLinePressed = true;
        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);
        //std::cout << "Here: " << mouseX << ", " << mouseY << std::endl;
        extern int _preserveLeftClickNum;

        glm::vec3 rayNDC = Utility::screenToNDC(window, mouseX, mouseY, 1.0f);
        glm::vec4 rayClip = Utility::NDCToHCC(rayNDC);
        glm::vec4 rayEye = Utility::clipToEyeC(rayClip);
        glm::vec3 rayWorld = Utility::eyeToWorldC(rayEye);


        _scene_main->getTestingLine()->addPoint(cam->getPos() + rayWorld);
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE && _testingLinePressed)
    {
        _scene_main->getTestingLine()->terminateLine();
        _testingLineExpected = false;
        _testingLinePressed = false;
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && _rayExpected)
    {
        _rayExpected = false;
        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);

        glm::vec3 rayNDC = Utility::screenToNDC(window, mouseX, mouseY, 1.0f);
        glm::vec4 rayClip = Utility::NDCToHCC(rayNDC);
        glm::vec4 rayEye = Utility::clipToEyeC(rayClip);
        glm::vec3 rayWorld = Utility::eyeToWorldC(rayEye);

        _scene_main->generateRay(cam->getPos(), rayWorld);
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && _selectingObjectExpected)
    {
        _selectingObjectExpected = false;
        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);

        glm::vec3 rayNDC = Utility::screenToNDC(window, mouseX, mouseY, 1.0f);
        glm::vec4 rayClip = Utility::NDCToHCC(rayNDC);
        glm::vec4 rayEye = Utility::clipToEyeC(rayClip);
        glm::vec3 rayWorld = Utility::eyeToWorldC(rayEye);

        _scene_main->selectObject(cam->getPos(), rayWorld);
    }
    _selectingObjectExpected = true;



}


int screenWidth;
int screenHeight;

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    extern bool _mouseRightButtonPressed;
    extern bool _cameraTranslationalMotionOn;
    float posXF = static_cast<float>(xpos);
    float posYF = static_cast<float>(ypos);

    glfwGetWindowSize(window, &screenWidth, &screenHeight);

    if (posXF < INITIAL_OPENGL_CONTEXT_POSITION_X || posYF < INITIAL_OPENGL_CONTEXT_POSITION_Y || posYF > (screenHeight - 160)) {
        _firstMouseMovement = true;
        return;
    }

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

    //extern bool _mouseLeftButtonPressed;
    //if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
    //    //_mouseLeftButtonPressed = true;
    //    //double mouseX, mouseY;
    //    //glfwGetCursorPos(window, &mouseX, &mouseY);
    //    ////std::cout << "Here: " << mouseX << ", " << mouseY << std::endl;
    //    extern int _preserveLeftClickNum;


    //    //glm::vec3 rayNDC = Utility::screenToNDC(window, mouseX, mouseY, 1.0f);
    //    //glm::vec4 rayClip = Utility::NDCToHCC(rayNDC);
    //    //glm::vec4 rayEye = Utility::clipToEyeC(rayClip);
    //    //glm::vec3 rayWorld = Utility::eyeToWorldC(rayEye);

    //    //std::cout << "X: " << rayWorld.x << std::endl;
    //    //std::cout << "Y: " << rayWorld.y << std::endl;
    //    //std::cout << "Z: " << rayWorld.z << std::endl;

    //    //extern MainScene* _scene_main;
    //    //_scene_main->getTestingLine()->addPoint(rayWorld);




    //    if (_preserveLeftClickNum > 0) {
    //        _preserveLeftClickNum--;

    //    }

    //} else {
    //    _mouseLeftButtonPressed = false;
    //}




}
