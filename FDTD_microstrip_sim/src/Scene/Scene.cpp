#include "Scene.h"

Scene::Scene()
{
	window = nullptr;
	aspect = 0.0f;
}

//Scene::Scene(GLFWwindow* w, GUI* g, Input* in)
//Scene::Scene(GLFWwindow* w, Input* in)
Scene::Scene(GLFWwindow* w)
{
	window = w;
	//gui = g;
	//input = in;
	aspect = 0.0f;
}
