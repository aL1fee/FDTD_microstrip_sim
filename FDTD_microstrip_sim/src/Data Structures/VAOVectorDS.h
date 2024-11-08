#pragma once

#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class VAOVectorDS
{
private:
	std::vector<unsigned int>* data;
	std::vector<unsigned int>* VBOs;

public:
	VAOVectorDS();
	~VAOVectorDS();

	void push(unsigned int ui);
	void pop(int i);
	void clear();

	void setAt(int i, unsigned int ui);
	unsigned int at(int i) const;
	int getSize() const;

	void addVBO(unsigned int ui);
	void clearVBOs();


};

