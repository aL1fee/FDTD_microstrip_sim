#pragma once

#include <iostream>
#include <vector>
#include <array>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <gtx/string_cast.hpp>

#include "../PhysicalObject.h"

class CoordinateSystem_PO : public PhysicalObject
{
private:
	float helperLineSeparation;
	glm::vec3 mainColor;
	glm::vec3 helperColor;

	void buildVertices() override;
	void buildVAOs() override;
	void buildEdges() override;

public:
	CoordinateSystem_PO(float l, float mls, Shader* sh);

	void build() override;
	void draw() override;
	bool intersectionCheck(glm::vec3 v) override;
	void clear() override {}
};