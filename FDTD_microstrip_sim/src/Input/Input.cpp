#include "Input.h"

extern Camera* cam;
extern MainScene* _scene_main;

extern int _initialWindowWidth;
extern int _initialWindowHeight;
extern int _currentWindowWidth;
extern int _currentWindowHeight;

bool _middleMouseReleaseExpected = false;

bool _firstMouseMovement = true;
float _lastX = INITIAL_OPENGL_CONTEXT_SCREEN_WIDTH / 2.0f;
float _lastY = INITIAL_OPENGL_CONTEXT_SCREEN_HEIGHT / 2.0f;

extern bool _testingLinePressed;
extern bool _testingLineExpected;
extern bool _selectingObjectExpected;
extern bool _selectingObjectPressed;
extern bool _resizing;
extern bool _rayExpected;
extern bool _rayRendered;

extern bool _wireInputExpected;
extern bool _wireInputPressed;
extern bool _wireFirstXZPlanePoint;
extern glm::vec3 _wireFirstPoint;
extern bool _wireZYplane;
extern bool _wireXYplane;

extern bool _ribbonInputExpected;
extern bool _ribbonInputPressed;

extern bool _windowS1On;
extern bool _windowS2On;

int screenWidth;
int screenHeight;

extern bool _modifyingVectorsActivated;
extern glm::vec3 _modifyingVectorsDirection;

extern bool _cameraVerticalRotationAboutPointOn;

bool _modyfingVectorsMovementStart = false;
glm::vec3 _modyfingVectorsMovementStartOrigin = glm::vec3(0.0f);
bool _modyfingVectorsScalingStart = false;
glm::vec3 _modyfingVectorsScalingStartOrigin = glm::vec3(0.0f);
bool _modyfingVectorsRotationStart = false;
glm::vec3 _modyfingVectorsRotationStartOrigin = glm::vec3(0.0f);
float _modyfingVectorsRotationStartOriginAngle = 0.0f;
float _prevRotationAngle = 0.0f;

glm::vec3 cameraRotationPoint = glm::vec3(0.0f, 0.0f, 0.0f);

glm::vec3 curveTerminationPoint = glm::vec3(0.0f);

Input::Input(GLFWwindow* w)
{
	window = w;
    potentialPressedGLFWButtons = new std::vector<int>();
    inputBuffer = new InputBuffer(window);
}

void Input::processInput()
{
    extern float _deltaTime;
    extern bool _cameraTranslationalMotionOn;
    extern bool _cameraRotationAboutPointOn;
    _cameraRotationAboutPointOn = false;
    _cameraVerticalRotationAboutPointOn = false;
    _cameraTranslationalMotionOn = false;

    inputBuffer->processKeyStates();

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
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS &&
        !inputBuffer->checkKeyState(GLFW_KEY_R)) {
        //    _scene_main->deleteModifyingVectors();
        _scene_main->showModifyingVectors(_scene_main->getActiveObject(), ModyfingVectorType::Rotation);
        inputBuffer->setKeyState(GLFW_KEY_R, true);
    }
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS && 
        !inputBuffer->checkKeyState(GLFW_KEY_T)) {
    //    _scene_main->deleteModifyingVectors();
        _scene_main->showModifyingVectors(_scene_main->getActiveObject(), ModyfingVectorType::Translation);
        inputBuffer->setKeyState(GLFW_KEY_T, true);
    }
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS &&
        !inputBuffer->checkKeyState(GLFW_KEY_Y)) {
    //    _scene_main->deleteModifyingVectors();
        _scene_main->showModifyingVectors(_scene_main->getActiveObject(), ModyfingVectorType::Scaling);
        inputBuffer->setKeyState(GLFW_KEY_Y, true);
    }
    if (glfwGetKey(window, GLFW_KEY_DELETE) == GLFW_PRESS) {
        _scene_main->deleteActiveObject();
    }
    //TODO inefficiencies
    if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS) {
        cam->modifyCameraSpeed(CAMERA_MAX_SPEED);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_RELEASE) {
        cam->modifyCameraSpeed(CAMERA_SPEED);
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
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS
        && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        _cameraVerticalRotationAboutPointOn = true;
    } else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE)
        == GLFW_PRESS) 
    {
        _cameraRotationAboutPointOn = true;
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_RELEASE) {
        _middleMouseReleaseExpected = false;
    }








    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS &&
        _ribbonInputExpected)
    {
        _ribbonInputPressed = true;

        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);

        glm::vec3 rayNDC = Utility::screenToNDC(window,
            static_cast<float>(mouseX), static_cast<float>(mouseY), 1.0f);
        glm::vec4 rayClip = Utility::NDCToHCC(rayNDC);
        glm::vec4 rayEye = Utility::clipToEyeC(rayClip);
        glm::vec3 rayWorld = Utility::eyeToWorldC(rayEye);

        //calculate intersection of the mouse with x-z plane
        glm::vec3 rayOrigin = cam->getPos();
        glm::vec3 rayDir = glm::vec3(rayWorld);

        glm::vec3 planeNormal = glm::vec3(1.0f, 0.0f, 0.0f);
        glm::vec3 planePoint = glm::vec3(0.0f);


        _scene_main->updateHighestClickedObjPoint(rayOrigin, rayDir);
        planeNormal = glm::vec3(0.0f, 1.0f, 0.0f);
        planePoint = _scene_main->getHighestClickedObjPoint();

        if (_scene_main->getActiveRibbon()->firstPointSelected())
        {
            planePoint = _scene_main->getActiveRibbon()->getFirstPoint();
        }


        float denom = glm::dot(planeNormal, rayDir);
        glm::vec3 intersectionPoint = glm::vec3(0.0f);
        if (fabs(denom) > 1e-6) {
            float t = glm::dot(planeNormal, (planePoint - rayOrigin)) / denom;
            intersectionPoint = rayOrigin + t * rayDir;
        }
        


        if (_scene_main->getActiveRibbon() == nullptr) 
        {
            _ribbonInputPressed = false;
            _ribbonInputExpected = false;
        }
        else
        {
            if (!_scene_main->getActiveRibbon()->isTerminated())
            {
                if (!_scene_main->getActiveRibbon()->firstPointSelected())
                {
                    _scene_main->getActiveRibbon()->setFirstPoint(intersectionPoint);
                    _scene_main->getActiveRibbon()->setOrigin(intersectionPoint);
                }
                else
                {
                    _scene_main->getActiveRibbon()->setSecondPotentialPoint(intersectionPoint);
                    _scene_main->getActiveRibbon()->setSecondPointSelected(true);
                }
            }
        }
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE && _ribbonInputPressed)
    {
        if (_scene_main->getActiveRibbon() != nullptr)
        {
            _scene_main->getActiveRibbon()->terminate();
            _scene_main->getActiveRibbon()->setBeingDrawn(false);
        }
        _ribbonInputExpected = false;
        _ribbonInputPressed = false;
        //_ribbonFirstXZPlanePoint = true;
        //_ribbonFirstPoint = glm::vec3(0.0f);
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS &&
        _wireInputExpected)
    {
        _wireInputPressed = true;

        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);

        glm::vec3 rayNDC = Utility::screenToNDC(window,
            static_cast<float>(mouseX), static_cast<float>(mouseY), 1.0f);
        glm::vec4 rayClip = Utility::NDCToHCC(rayNDC);
        glm::vec4 rayEye = Utility::clipToEyeC(rayClip);
        glm::vec3 rayWorld = Utility::eyeToWorldC(rayEye);

        //calculate intersection of the mouse with x-z plane
        glm::vec3 rayOrigin = cam->getPos();
        glm::vec3 rayDir = glm::vec3(rayWorld);


        glm::vec3 planeNormal = glm::normalize(glm::vec3(rayDir.x, 0.0f, rayDir.z));
        if (_wireZYplane)
        {
            planeNormal = glm::vec3(1.0f, 0.0f, 0.0f);
        }
        if (_wireXYplane)
        {
            planeNormal = glm::vec3(0.0f, 0.0f, 1.0f);
        }

        glm::vec3 planePoint = _wireFirstPoint;

        if (_wireFirstXZPlanePoint) 
        {
            _scene_main->updateHighestClickedObjPoint(rayOrigin, rayDir);
            planeNormal = glm::vec3(0.0f, 1.0f, 0.0f);
            planePoint = _scene_main->getHighestClickedObjPoint();
        }

        float denom = glm::dot(planeNormal, rayDir);
        glm::vec3 intersectionPoint = glm::vec3(0.0f);
        if (fabs(denom) > 1e-6) {
            float t = glm::dot(planeNormal, (planePoint - rayOrigin)) / denom;
            intersectionPoint = rayOrigin + t * rayDir;
        }
        if (_wireFirstXZPlanePoint)
        {
            _wireFirstXZPlanePoint = false;
            _wireFirstPoint = intersectionPoint;
        }

        PhysicalObject* higherPointObject = _scene_main->higherPointObject(intersectionPoint - glm::vec3(
            0.0f, CURVE_CHECK_HIGHER_POINT_OBJ_MARGIN , 0.0f), _scene_main->getActiveWire()->getMaxPoint());

        if (higherPointObject == nullptr && !_scene_main->getActiveWire()->isTerminated())
        {
            _scene_main->getActiveWire()->updateUnprocessedVertex(intersectionPoint);
        }
        else if (!_scene_main->getActiveWire()->isTerminated()) 
        {
            if (!_scene_main->getActiveWire()->isLastPointEntered())
            {
                curveTerminationPoint = glm::vec3(intersectionPoint.x,
                    *higherPointObject->getOriginY() + *higherPointObject->getHeight(),
                    intersectionPoint.z);

                _scene_main->getActiveWire()->setLastVertex(curveTerminationPoint);
                _scene_main->getActiveWire()->updateUnprocessedVertex(curveTerminationPoint);

                _scene_main->getActiveWire()->lastPointEntered(true);
            }
        }
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE && _wireInputPressed)
    {
        //BUG next line?
        //_scene_main->getTestingLine()->terminateLine();
        _wireInputExpected = false;
        _wireInputPressed = false;
        _wireZYplane = false;
        _wireXYplane = false;
        _wireFirstXZPlanePoint = true;
        _scene_main->terminateAllWires();

    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && _testingLineExpected)
    {
        _testingLinePressed = true;
        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);
        //std::cout << "Here: " << mouseX << ", " << mouseY << std::endl;
        extern int _preserveLeftClickNum;

        glm::vec3 rayNDC = Utility::screenToNDC(window, 
            static_cast<float>(mouseX), static_cast<float>(mouseY), 1.0f);
        glm::vec4 rayClip = Utility::NDCToHCC(rayNDC);
        glm::vec4 rayEye = Utility::clipToEyeC(rayClip);
        glm::vec3 rayWorld = Utility::eyeToWorldC(rayEye);


        _scene_main->getTestingLine()->addPoint(cam->getPos() + rayWorld * 5.0f);
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE && _testingLinePressed)
    {
        _scene_main->getTestingLine()->terminateLine();
        _testingLineExpected = false;
        _testingLinePressed = false;
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && _rayExpected)
    {
        _rayRendered = true;
        _rayExpected = false;
        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);

        glm::vec3 rayNDC = Utility::screenToNDC(window, 
            static_cast<float>(mouseX), static_cast<float>(mouseY), 1.0f);
        glm::vec4 rayClip = Utility::NDCToHCC(rayNDC);
        glm::vec4 rayEye = Utility::clipToEyeC(rayClip);
        glm::vec3 rayWorld = Utility::eyeToWorldC(rayEye);

        _scene_main->generateRay(cam->getPos(), rayWorld);
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
    {
        _selectingObjectExpected = true;
        _modyfingVectorsMovementStart = false;
        _modyfingVectorsScalingStart = false;
        _modyfingVectorsRotationStart = false;
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && _selectingObjectExpected)
    {
        _selectingObjectExpected = false;
        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);
        glfwGetWindowSize(window, &screenWidth, &screenHeight);
        if (mouseX > 260 && mouseY > 83 && mouseY < screenHeight - 160) 
        {
            int rightPaneCheckY = 115;
            if (_windowS1On || _windowS2On)
            {
                rightPaneCheckY = (screenHeight - 160 - 83 - 16) / 2 + 115;
            }
            if (_windowS1On && _windowS2On)
            {
                rightPaneCheckY = screenHeight;
            }
            if (mouseX < screenWidth - 400 || mouseY > rightPaneCheckY)
            {
                glm::vec3 rayNDC = Utility::screenToNDC(window,
                    static_cast<float>(mouseX), static_cast<float>(mouseY), 1.0f);
                glm::vec4 rayClip = Utility::NDCToHCC(rayNDC);
                glm::vec4 rayEye = Utility::clipToEyeC(rayClip);
                glm::vec3 rayWorld = Utility::eyeToWorldC(rayEye);
                _scene_main->selectObject(cam->getPos(), rayWorld);
            }
            else {
                //_scene_main->deleteModifyingVectors();
                _scene_main->hideModifyingVectors();
            }
        }
        else {
            //_scene_main->deleteModifyingVectors();
            _scene_main->hideModifyingVectors();
        }
    }
}

float lastScaleValue = 0.0f;
float previousAngle = 0.0f;

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    extern float _deltaTime;
    extern bool _mouseRightButtonPressed;
    extern bool _cameraTranslationalMotionOn;
    extern bool _cameraRotationAboutPointOn;
    float posXF = static_cast<float>(xpos);
    float posYF = static_cast<float>(ypos);
    double mouseX, mouseY;

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
        cam->processMouseTranslation(offsetX, offsetY, _deltaTime);
    } else if (_mouseRightButtonPressed) {
        cam->processMouseRotation(offsetX, offsetY);
    }
    if (_cameraVerticalRotationAboutPointOn) {
        if (_scene_main->getActiveObject() != nullptr)
        {
            //TODO shouldn't be just an active object: one has to be pointing at it
            //---> point of collision w/ the object, not its center!
            cameraRotationPoint = _scene_main->getActiveObject()->getCenterLocation();
            _middleMouseReleaseExpected = true;
        }
        // confusing nameings (_middleMouseReleaseExpected)
        else if (!_middleMouseReleaseExpected)
        {
            _middleMouseReleaseExpected = true;
            glfwGetCursorPos(window, &mouseX, &mouseY);
            glm::vec3 rayNDC = Utility::screenToNDC(window,
                static_cast<float>(mouseX), static_cast<float>(mouseY), 1.0f);
            glm::vec4 rayClip = Utility::NDCToHCC(rayNDC);
            glm::vec4 rayEye = Utility::clipToEyeC(rayClip);
            glm::vec3 rayWorld = Utility::eyeToWorldC(rayEye);
            cameraRotationPoint = cam->getPos() + rayWorld * 10.0f;
        }
        cam->processMouseRotationAboutPoint(offsetX, offsetY,
            _deltaTime, cameraRotationPoint, VERTICAL);
    } else if (_cameraRotationAboutPointOn) 
    {
        if (_scene_main->getActiveObject() != nullptr)
        {
        // same comments as above
            cameraRotationPoint = _scene_main->getActiveObject()->getCenterLocation();
            _middleMouseReleaseExpected = true;
        }
        else if (!_middleMouseReleaseExpected) 
        {
            _middleMouseReleaseExpected = true;
            glfwGetCursorPos(window, &mouseX, &mouseY);
            glm::vec3 rayNDC = Utility::screenToNDC(window,
                static_cast<float>(mouseX), static_cast<float>(mouseY), 1.0f);
            glm::vec4 rayClip = Utility::NDCToHCC(rayNDC);
            glm::vec4 rayEye = Utility::clipToEyeC(rayClip);
            glm::vec3 rayWorld = Utility::eyeToWorldC(rayEye);
            cameraRotationPoint = cam->getPos() + rayWorld * 10.0f;
        }
        cam->processMouseRotationAboutPoint(offsetX, offsetY,
            _deltaTime, cameraRotationPoint, HORIZONTAL);
    }
    
    Input::updateModifyingVectors(window, xpos, ypos);
}

void Input::updateModifyingVectors(GLFWwindow* window, double xpos, double ypos)
{
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && _modifyingVectorsActivated)
    {
        if (_scene_main->getModifyingVectors()->isBeingRendered() && 
            _scene_main->getModifyingVectors() != nullptr && 
            _scene_main->getActiveObject() != nullptr)
        {
            ModyfingVectorType type = _scene_main->getModifyingVectors()->getType();
            double mouseX, mouseY;
            glfwGetCursorPos(window, &mouseX, &mouseY);

            glm::vec3 rayNDC = Utility::screenToNDC(window,
                static_cast<float>(mouseX), static_cast<float>(mouseY), 1.0f);
            glm::vec4 rayClip = Utility::NDCToHCC(rayNDC);
            glm::vec4 rayEye = Utility::clipToEyeC(rayClip);
            glm::vec3 rayWorld = Utility::eyeToWorldC(rayEye);
            glm::vec3 activeObjOrigin = *_scene_main->getActiveObject()->getOrigin();

            glm::vec3 rayOrigin = cam->getPos();
            glm::vec3 rayDir = glm::vec3(rayWorld);
            glm::vec3 planeNormal = __zero_vec3;
            glm::vec3 planePoint = __zero_vec3;

            glm::vec3 intersectionPoint = __zero_vec3;

            float currentRotAngle = *_scene_main->getActiveObject()->getRotationAngle();
            
            if (type == Translation)
            {
                if (_modifyingVectorsDirection == __x_norm_vec3)
                {
                    planePoint = glm::vec3(0.0f, activeObjOrigin.y, 0.0f);
                    planeNormal = __y_norm_vec3;
                }
                else if (_modifyingVectorsDirection == __z_norm_vec3)
                {
                    planePoint = glm::vec3(0.0f, activeObjOrigin.y, 0.0f);
                    planeNormal = __y_norm_vec3;
                }
                else if (_modifyingVectorsDirection == __y_norm_vec3) {
                    planePoint = glm::vec3(activeObjOrigin.x, 0.0f, activeObjOrigin.z);
                    planeNormal = glm::normalize(rayDir);
                }
                else {
                    return;
                }
                float denom = glm::dot(planeNormal, rayDir);
                if (fabs(denom) > 1e-6) {
                    float t = glm::dot(planeNormal, (planePoint - rayOrigin)) / denom;
                    intersectionPoint = rayOrigin + t * rayDir;
                    if (!_modyfingVectorsMovementStart) {
                        _modyfingVectorsMovementStartOrigin = intersectionPoint - activeObjOrigin;
                        _modyfingVectorsMovementStart = true;
                    }
                    intersectionPoint -= _modyfingVectorsMovementStartOrigin;
                }
                if (_modifyingVectorsDirection == __x_norm_vec3)
                {
                    intersectionPoint.y = activeObjOrigin.y;
                    glm::vec3 rotationDir = glm::vec3(
                        cos(glm::radians(currentRotAngle)),
                        0.0f,
                        -sin(glm::radians(currentRotAngle))
                    );
                    glm::vec3 displacement = intersectionPoint - activeObjOrigin;
                    float distance = glm::dot(displacement, rotationDir);
                    intersectionPoint = activeObjOrigin + rotationDir * distance;
                }
                else if (_modifyingVectorsDirection == __z_norm_vec3)
                {
                    intersectionPoint.y = activeObjOrigin.y;
                    glm::vec3 rotationDir = glm::vec3(
                        cos(glm::radians(currentRotAngle - 90.0f)),
                        0.0f,
                        -sin(glm::radians(currentRotAngle - 90.0f))
                    );
                    glm::vec3 displacement = intersectionPoint - activeObjOrigin;
                    float distance = glm::dot(displacement, rotationDir);
                    intersectionPoint = activeObjOrigin + rotationDir * distance;
                }
                else {
                    intersectionPoint.z = activeObjOrigin.z;
                    intersectionPoint.x = activeObjOrigin.x;
                }
                _scene_main->getActiveObject()->setOrigin(intersectionPoint);
                _scene_main->getActiveObject()->generateModelMatrix();
                _scene_main->getModifyingVectors()->setOrigin
                (_scene_main->getActiveObject()->getCenterLocation());
                _scene_main->getModifyingVectors()->setRebuiltExpected(true);
            }
            else if (type == Scaling)
            {
                if (_modifyingVectorsDirection == __x_norm_vec3)
                {
                    planePoint = glm::vec3(0.0f, activeObjOrigin.y, 0.0f);
                    planeNormal = __y_norm_vec3;
                    lastScaleValue = *_scene_main->getActiveObject()->getLength();
                }
                else if (_modifyingVectorsDirection == __z_norm_vec3)
                {
                    planePoint = glm::vec3(0.0f, activeObjOrigin.y, 0.0f);
                    planeNormal = __y_norm_vec3;
                    lastScaleValue = *_scene_main->getActiveObject()->getWidth();
                }
                else if (_modifyingVectorsDirection == __y_norm_vec3) {
                    planePoint = glm::vec3(activeObjOrigin.x, 0.0f, activeObjOrigin.z);
                    //planeNormal = glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f));
                    planeNormal = glm::normalize(rayDir);
                    lastScaleValue = *_scene_main->getActiveObject()->getHeight();
                }
                else {
                    return;
                }
                float denom = glm::dot(planeNormal, rayDir);
                if (fabs(denom) > 1e-6) {
                    float t = glm::dot(planeNormal, (planePoint - rayOrigin)) / denom;
                    intersectionPoint = rayOrigin + t * rayDir;
                    if (!_modyfingVectorsMovementStart) {
                        _modyfingVectorsScalingStartOrigin = glm::vec3(*_scene_main->getActiveObject()->getLength(),
                            *_scene_main->getActiveObject()->getHeight(), *_scene_main->getActiveObject()->getWidth());
                        _modyfingVectorsScalingStart = true;
                        _modyfingVectorsMovementStartOrigin = intersectionPoint;
                        _modyfingVectorsMovementStart = true;
                    }
                    intersectionPoint -= _modyfingVectorsMovementStartOrigin;
                }
                float value = 0.0f;
                if (_modifyingVectorsDirection == __x_norm_vec3)
                {
                    intersectionPoint.y = activeObjOrigin.y;
                    glm::vec3 rotationDir = glm::vec3(
                        cos(glm::radians(currentRotAngle)),
                        0.0f,
                        -sin(glm::radians(currentRotAngle))
                    );
                    glm::vec3 displacement = intersectionPoint - activeObjOrigin;
                    float distance = glm::dot(intersectionPoint, rotationDir);
                    intersectionPoint = activeObjOrigin + rotationDir * distance;

                    value = distance + _modyfingVectorsScalingStartOrigin.x;

                    if (value < MIN_OBJECT_SIZE) {
                        value = MIN_OBJECT_SIZE;
                    }
                    if (value > MAX_OBJECT_SIZE) {
                        value = MAX_OBJECT_SIZE;
                    }

                    if (fabs(lastScaleValue - value) > MAX_DISTANCE_RESIZE_JUMP) {
                        return;
                    }
                    _scene_main->getActiveObject()->setScaleL(value);
                }
                else if (_modifyingVectorsDirection == __z_norm_vec3)
                {
                    intersectionPoint.y = activeObjOrigin.y;
                    glm::vec3 rotationDir = glm::vec3(
                        cos(glm::radians(currentRotAngle - 90.0f)),
                        0.0f,
                        -sin(glm::radians(currentRotAngle - 90.0f))
                    );
                    glm::vec3 displacement = intersectionPoint - activeObjOrigin;
                    float distance = glm::dot(intersectionPoint, rotationDir);
                    intersectionPoint = activeObjOrigin + rotationDir * distance;

                    value = distance + _modyfingVectorsScalingStartOrigin.z;
                    
                    if (value < MIN_OBJECT_SIZE) {
                        value = MIN_OBJECT_SIZE;
                    }
                    if (value > MAX_OBJECT_SIZE) {
                        value = MAX_OBJECT_SIZE;
                    }

                    if (fabs(lastScaleValue - value) > MAX_DISTANCE_RESIZE_JUMP) {
                        return;
                    }

                    _scene_main->getActiveObject()->setScaleW(value);
                }
                else if (_modifyingVectorsDirection == __y_norm_vec3)
                {
                    intersectionPoint.z = activeObjOrigin.z;
                    intersectionPoint.x = activeObjOrigin.x;
                    value = intersectionPoint.y + _modyfingVectorsScalingStartOrigin.y;
                    if (value < MIN_OBJECT_SIZE) {
                        value = MIN_OBJECT_SIZE;
                    }
                    if (value > MAX_OBJECT_SIZE) {
                        value = MAX_OBJECT_SIZE;
                    }
                    if (_scene_main->getActiveObject()->getObjectType() == RIBBON)
                    {
                        if (value > RIBBON_MAX_CURVE_HEIGHT)
                        {
                            value = RIBBON_MAX_CURVE_HEIGHT;
                        }
                    }

                    if (fabs(lastScaleValue - value) > MAX_DISTANCE_RESIZE_JUMP) {
                        return;
                    }
                    _scene_main->getActiveObject()->setScaleH(value);
                }
                // i don't understand why i have to do this
                if (_scene_main->getActiveObject()->getObjectType() == CUBOID_ARRAY)
                {
                    _scene_main->getActiveObject()->setInstancePositionsChanged(true);
                    _scene_main->getActiveObject()->setRebuiltExpected(true);
                }
                else
                {
                    _scene_main->getActiveObject()->generateModelMatrix();
                }
                _scene_main->getModifyingVectors()->setOrigin
                (_scene_main->getActiveObject()->getCenterLocation());
                _scene_main->getModifyingVectors()->setRebuiltExpected(true);
            }
            else if (type == Rotation)
            {
                planeNormal = __y_norm_vec3;
                planePoint = activeObjOrigin;
                float denom = glm::dot(planeNormal, rayDir);
                //intersectionPoint = glm::vec3(0.0f);

                if (fabs(denom) > 1e-8) {
                    float t = glm::dot(planeNormal, (planePoint - rayOrigin)) / denom;
                    intersectionPoint = rayOrigin + t * rayDir;
                    if (!_modyfingVectorsRotationStart) {
                        _modyfingVectorsMovementStartOrigin = intersectionPoint - activeObjOrigin;
                        _modyfingVectorsRotationStart = true;
                        _prevRotationAngle = 0.0f;
                    }
                }
                else
                {
                    std::cout << "Rotation not completed in Input.cpp" << std::endl;
                    return;
                }

                glm::vec3 currIntersecVec = intersectionPoint - activeObjOrigin;

                float dotProd = glm::dot(currIntersecVec, _modyfingVectorsMovementStartOrigin);
                float magnitudeCIV = glm::length(currIntersecVec);
                float magnitudeMVMSO = glm::length(_modyfingVectorsMovementStartOrigin);

                float cosTheta = glm::clamp(dotProd / (magnitudeCIV * magnitudeMVMSO), -1.0f, 1.0f);
                float newAngle = acos(cosTheta);

                glm::vec3 crossProd = glm::cross(_modyfingVectorsMovementStartOrigin, currIntersecVec);

                float sign = glm::dot(crossProd, __y_norm_vec3);

                if (sign < 0) {
                    newAngle = -newAngle;
                }
                

                float finalAngle = currentRotAngle + glm::degrees(newAngle - _prevRotationAngle);
                _prevRotationAngle = newAngle;


                if (finalAngle < 0.0f) {
                    finalAngle = std::fmod(finalAngle, 360.0f);
                    finalAngle += 360.0f;
                }
                if (finalAngle > 360.0f) {
                    finalAngle = std::fmod(finalAngle, 360.0f);
                }

                _scene_main->getActiveObject()->setRotationAngle(finalAngle);
                _scene_main->getActiveObject()->generateModelMatrix();
                _scene_main->getModifyingVectors()->setOrigin(_scene_main->getActiveObject()
                    ->getCenterLocation());
                _scene_main->getModifyingVectors()->setRotationAngle(*_scene_main->
                    getActiveObject()->getRotationAngle());
                _scene_main->getModifyingVectors()->generateModelMatrix();
            }
        }
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);
    glfwGetWindowSize(window, &screenWidth, &screenHeight);

    if (mouseX < INITIAL_OPENGL_CONTEXT_POSITION_X || mouseY < INITIAL_OPENGL_CONTEXT_POSITION_Y || mouseY >(screenHeight - 160)) {
        return;
    }

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
