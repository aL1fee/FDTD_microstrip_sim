#pragma once

#include <vector>
#include <string>

#include "../Physics/PhysicalObject.h"

class PropertyWindow
{
private:
	//std::map<std::string, float*>* properties;
	std::vector<std::pair<std::string, float*>>* properties;
	std::vector<std::pair<std::string, int*>>* propertiesInt;

	PhysicalObject* activeObject;


public:
	PropertyWindow();

	void addPropertyMap(std::vector<std::pair<std::string, float*>>* p);
	void addPropertyMapInt(std::vector<std::pair<std::string, int*>>* p);
	void setDefaultPropertyMap();

	std::vector<std::pair<std::string, float*>>* getProperties() { return properties; }
	std::vector<std::pair<std::string, int*>>* getPropertiesInt() { return propertiesInt; }
	
	
	void updateActiveObject(PhysicalObject* obj);
	
	PhysicalObject* getActiveObject() { return activeObject; }

	void nullify();
};