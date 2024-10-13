#include "Cuboid.h"

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
