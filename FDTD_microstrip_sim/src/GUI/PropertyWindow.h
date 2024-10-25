#pragma once

#include <vector>
#include <string>

#include "../Physics/Carrier_PO.h"

class PropertyWindow
{
private:
	//std::map<std::string, float*>* properties;
	std::vector<std::pair<std::string, float*>>* properties;

	//TODO no hardcoding
	Carrier_PO* object;


public:
	PropertyWindow();

	void addPropertyMap(std::vector<std::pair<std::string, float*>>* p);

	std::vector<std::pair<std::string, float*>>* getProperties() { return properties; }

	void updateActiveObject();

};