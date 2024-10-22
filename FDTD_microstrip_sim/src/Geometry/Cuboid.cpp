#include "Cuboid.h"

Cuboid::Cuboid()
{
    origin = glm::vec3(0);
    length = 0;
    width = 0;
    height = 0;
    vertices = new VertexVectorDS();
    VAOs = new VAOVectorDS();
}

Cuboid::Cuboid(glm::vec3 o, float l, float w, float h)
{
    origin = o;
    length = l;
    width = w;
    height = h;
}

void Cuboid::build()
{

}
