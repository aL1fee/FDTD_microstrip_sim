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
	std::vector<std::vector<unsigned int>*>* indices;

public:
	VertexVectorDS();
	~VertexVectorDS();

	void clear();

	void pushToExistingArray(glm::vec3 v);
	void pushToNewArray(glm::vec3 v);
	void pushToIndArray(int i, glm::vec3 v);
	void allocateNewArray();
	void allocateNewEmptyArray();
	void allocateNEmptyArrays(int n);

	std::vector<glm::vec3>* at(int i) const;

	std::vector<unsigned int>* indicesAt(int i) const;
	void pushIndices(std::vector<unsigned int>* inds);
	void addIndicesToInd(int i, std::vector<unsigned int>* inds);
	void allocateNewIndices();

	int getSize() const;
	int getLastIndex() const { return lastIndex; }

	glm::vec3 peekAtLast() const;




};

