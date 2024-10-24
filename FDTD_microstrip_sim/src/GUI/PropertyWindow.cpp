#include "PropertyWindow.h"

PropertyWindow::PropertyWindow()
{
	properties = nullptr;
	object = nullptr;
}

void PropertyWindow::addPropertyMap(std::map<std::string, float&>* p)
{
	properties = p;
}

void PropertyWindow::updateActiveObject()
{

}
