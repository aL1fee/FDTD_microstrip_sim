#include "VertexVectorDS.h"

VertexVectorDS::VertexVectorDS()
{
	data = new std::vector<std::vector<glm::vec3>*>();
	lastIndex = 0;
}

VertexVectorDS::~VertexVectorDS()
{
	for (int i = 0; i < data->size(); i++) {
		delete data->at(i);
	}
	delete data;
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
	std::vector<glm::vec3>* vec = new std::vector<glm::vec3>();
	data->push_back(vec);
	lastIndex++;
}

std::vector<glm::vec3>* VertexVectorDS::at(int i) const
{
	return data->at(i);
}

int VertexVectorDS::getSize() const
{
	return data->size();
}
