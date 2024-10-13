#include "InputBuffer.h"

InputBuffer::InputBuffer()
{
	leftClickBuffer = new std::vector<glm::vec3>();
}

void InputBuffer::addLeftClick(glm::vec3 v)
{
	leftClickBuffer->push_back(v);
}

void InputBuffer::clearLeftClickBuffer()
{
	leftClickBuffer->clear();
}

glm::vec3 InputBuffer::getLeftClick(int i) const
{
	return leftClickBuffer->at(i);
}
