#include "PropertyWindow.h"

PropertyWindow::PropertyWindow()
{
	properties = nullptr;
	activeObject = nullptr;
}

void PropertyWindow::addPropertyMap(std::vector<std::pair<std::string, float*>>* p)
{
	properties = p;
}

void PropertyWindow::setDefaultPropertyMap()
{
	//if (properties != nullptr) {
	//	properties->clear();
	//}
	properties = nullptr;
}

void PropertyWindow::updateActiveObject(PhysicalObject* obj)
{
	activeObject = obj;
}
