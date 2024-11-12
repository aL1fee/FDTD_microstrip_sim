#include "VertexVectorDS.h"

VertexVectorDS::VertexVectorDS()
{
	data = new std::vector<std::vector<glm::vec3>*>();
	indices = new std::vector<std::vector<unsigned int>*>();
	lastIndex = 0;
}

VertexVectorDS::~VertexVectorDS()
{
	for (int i = 0; i < data->size(); i++) {
		delete data->at(i);
	}
	delete data;
	for (int i = 0; i < indices->size(); i++) {
		delete indices->at(i);
	}
	delete indices;
}

void VertexVectorDS::clear()
{
	//std::cout << "cleaning vertexvectorDS" << std::endl;
	for (int i = 0; i < data->size(); i++) {
		delete data->at(i);
	}
	lastIndex = 0;
	data->clear();
	for (int i = 0; i < indices->size(); i++) {
		delete indices->at(i);
	}
	indices->clear();
}

void VertexVectorDS::pushToExistingArray(glm::vec3 v)
{
	if (getSize() == 0) {
		std::vector<glm::vec3>* vec = new std::vector<glm::vec3>();
		data->push_back(vec);
	}
	data->at(lastIndex)->push_back(v);
}

void VertexVectorDS::pushToNewArray(glm::vec3 v)
{
	std::vector<glm::vec3>* vec = new std::vector<glm::vec3>();
	data->push_back(vec);
	data->at(lastIndex++)->push_back(v);
}

void VertexVectorDS::pushToIndArray(int i, glm::vec3 v)
{
	if (i < 0 || i >= getSize()) {
		return;
	}
	data->at(i)->push_back(v);
}

void VertexVectorDS::allocateNewArray()
{
	if (getSize() != 0) {
		std::vector<glm::vec3>* vec = new std::vector<glm::vec3>();
		data->push_back(vec);
		lastIndex++;
	}
}

void VertexVectorDS::allocateNewEmptyArray()
{
	if (getSize() != 0) {
		lastIndex++;
	}
	std::vector<glm::vec3>* vec = new std::vector<glm::vec3>();
	data->push_back(vec);
}

//bugs. be careful using it.
void VertexVectorDS::allocateNEmptyArrays(int n)
{
	if (getSize() != 0) {
		lastIndex++;
	}
	for (int i = 0; i < n; i++)
	{
		std::vector<glm::vec3>* vec = new std::vector<glm::vec3>();
		data->push_back(vec);
	}
}

std::vector<glm::vec3>* VertexVectorDS::at(int i) const
{
	return data->at(i);
}

std::vector<unsigned int>* VertexVectorDS::indicesAt(int i) const
{
	return indices->at(i);
}

void VertexVectorDS::pushIndices(std::vector<unsigned int>* inds)
{
	indices->push_back(inds);
}

void VertexVectorDS::addIndicesToInd(int i, std::vector<unsigned int>* inds)
{
	if (indices->size() > i + 1)
	{
		std::cout << "VertexVectorDS Indicies out of bounds error!" << std::endl;
		return;
	}
	std::vector<unsigned int>* oldInds = indices->at(i);
	indices->at(i) = inds;
	delete oldInds;
}

void VertexVectorDS::allocateNewIndices()
{
	std::vector<unsigned int>* newInds = new std::vector<unsigned int>();
	indices->push_back(newInds);
}

int VertexVectorDS::getSize() const
{
	return static_cast<int>(data->size());
}

glm::vec3 VertexVectorDS::peekAtLast() const
{
	return data->at(getSize() - 1)->at(data->at(getSize() - 1)->size() - 1);
}
