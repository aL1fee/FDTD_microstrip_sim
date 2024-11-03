#include "PropertyWindow.h"

PropertyWindow::PropertyWindow()
{
	properties = nullptr;
	propertiesInt = nullptr;
	activeObject = nullptr;
}

void PropertyWindow::addPropertyMap(std::vector<std::pair<std::string, float*>>* p)
{
	properties = p;
}

void PropertyWindow::addPropertyMapInt(std::vector<std::pair<std::string, int*>>* p)
{
	propertiesInt = p;
}

void PropertyWindow::setDefaultPropertyMap()
{
	//if (properties != nullptr) {
	//	properties->clear();
	//}
	properties = nullptr;
	propertiesInt = nullptr;
}

void PropertyWindow::updateActiveObject(PhysicalObject* obj)
{
	activeObject = obj;
}

void PropertyWindow::nullify()
{
	properties = nullptr;
	propertiesInt = nullptr;
	activeObject = nullptr;
}
