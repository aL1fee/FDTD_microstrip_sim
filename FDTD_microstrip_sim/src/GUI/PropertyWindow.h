#pragma once

#include <vector>
#include <string>

#include "../Physics/PhysicalObject.h"

class PropertyWindow
{
private:
	//std::map<std::string, float*>* properties;
	std::vector<std::pair<std::string, float*>>* properties;

	PhysicalObject* activeObject;


public:
	PropertyWindow();

	void addPropertyMap(std::vector<std::pair<std::string, float*>>* p);
	void setDefaultPropertyMap();

	std::vector<std::pair<std::string, float*>>* getProperties() { return properties; }

	void updateActiveObject(PhysicalObject* obj);
	
	PhysicalObject* getActiveObject() { return activeObject; }
};