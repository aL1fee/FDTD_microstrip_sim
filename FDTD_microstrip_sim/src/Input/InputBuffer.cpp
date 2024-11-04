#include "InputBuffer.h"

InputBuffer::InputBuffer(GLFWwindow* w)
{
    window = w;
	initializeKeys();
}

void InputBuffer::initializeKeys()
{
    keyStateMap[GLFW_KEY_T] = false;
    keyStateMap[GLFW_KEY_Y] = false;
}

void InputBuffer::processKeyStates()
{
    for (const auto& key : keyStateMap)
    {
        int currentState = glfwGetKey(window, key.first);
        if (currentState == GLFW_RELEASE)
        {
            keyStateMap[key.first] = false;
        }
    }
}

bool InputBuffer::checkKeyState(int key)
{
    return keyStateMap[key];
}

void InputBuffer::setKeyState(int key, bool val)
{
    keyStateMap[key] = val;
}




