#pragma once

#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <gtx/string_cast.hpp>

#include "../PhysicalObject.h"

#include "../../Macros.h"

enum ModyfingVectorType {
	Translation,
	Scaling,
	Rotation
};

class ModifyingVectors_PO : public PhysicalObject
{
private:
	float size;
	ModyfingVectorType type;

	glm::vec3 colorX;
	glm::vec3 colorZ;
	glm::vec3 colorY;

	float arrowAngle;

	void buildVertices() override;
	void buildVAOs() override;
	void buildEdges() override;

	void constructTranslationArrows();
	void constructScalingArrows();

	void rebuild() override;


public:
	ModifyingVectors_PO(float s, ModyfingVectorType t, glm::vec3 c, Shader* sh);

	void build() override;
	void draw() override;
	bool intersectionCheck(glm::vec3 v) override;
	void clear() override {}

	glm::vec3 intersectionDirection(glm::vec3 v);

	ModyfingVectorType getType() const { return type; }
};