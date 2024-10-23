#pragma once

#include <vector>
#include <cmath>
#include <limits>

#include <iostream>

#include "glm.hpp"
#include <gtx/string_cast.hpp>

class VertexVectorDS
{
private:
	unsigned int lastIndex;
	std::vector<std::vector<glm::vec3>*>* data;

public:
	VertexVectorDS();
	~VertexVectorDS();

	void clear();

	void pushToExistingArray(glm::vec3 v);
	void pushToNewArray(glm::vec3 v);
	void pushToIndArray(int i, glm::vec3 v);
	void allocateNewArray();

	std::vector<glm::vec3>* at(int i) const;

	int getSize() const;

	glm::vec3 peekAtLast() const;


};

