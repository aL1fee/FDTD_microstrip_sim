#pragma once

#include <map>
#include <string>

#include "../Physics/Carrier_PO.h"

class PropertyWindow
{
private:
	std::map<std::string, float&>* properties;

	//TODO no hardcoding
	Carrier_PO* object;


public:
	PropertyWindow();

	void addPropertyMap(std::map<std::string, float&>* p);

	std::map<std::string, float&>* getProperties() { return properties; }

	void updateActiveObject();

};