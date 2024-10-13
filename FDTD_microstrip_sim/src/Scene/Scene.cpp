#include "Scene.h"

Scene::Scene(GLFWwindow* w, GUI* g, Input* in)
{
	window = w;
	gui = g;
	input = in;
	aspect = 0.0f;
}
