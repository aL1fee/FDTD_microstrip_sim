#include "Cuboid.h"

Cuboid::Cuboid()
{
    origin = glm::vec3(0);
    length = 0;
    width = 0;
    height = 0;
    vertices = new std::vector<glm::vec3>();
    VAOs = new std::vector<unsigned int>();
}

Cuboid::Cuboid(glm::vec3 o, float l, float w, float h)
{
    origin = o;
    length = l;
    width = w;
    height = h;
}

std::vector<glm::vec3>* Cuboid::build()
{
    return nullptr;
}
