#pragma once

#include <vector>

class VAOVectorDS
{
private:
	std::vector<unsigned int>* data;

public:
	VAOVectorDS();
	~VAOVectorDS();

	void push(unsigned int ui);
	void pop(int i);
	void clear();

	void setAt(int i, unsigned int ui);
	unsigned int at(int i) const;
	int getSize() const;



};

