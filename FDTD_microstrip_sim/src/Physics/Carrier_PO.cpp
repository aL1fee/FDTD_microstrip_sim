#include "Carrier_PO.h"

void Carrier_PO::init()
{
	updatePropertyMap();
}

Carrier_PO::Carrier_PO()
{
	init();
}

void Carrier_PO::updatePropertyMap()
{
	propertyMap->push_back(std::make_pair("X position", &origin.x));
	propertyMap->push_back(std::make_pair("Z position", &origin.z));
	propertyMap->push_back(std::make_pair("Y position", &origin.y));
	propertyMap->push_back(std::make_pair("Length", getLength()));
	propertyMap->push_back(std::make_pair("Width", getWidth()));
	propertyMap->push_back(std::make_pair("Height", getHeight()));
	propertyMap->push_back(std::make_pair("Permittivity", &permittivity));
	propertyMap->push_back(std::make_pair("Conductivity", &conductivity));


	//propertyMap->insert(std::make_pair("X position", &origin.x));
	//propertyMap->insert(std::make_pair("Z position", &origin.z));
	//propertyMap->insert(std::make_pair("Y position", &origin.y));
	//propertyMap->insert(std::make_pair("Length", getLength()));
	//propertyMap->insert(std::make_pair("Width", getWidth()));
	//propertyMap->insert(std::make_pair("Height", getHeight()));
	//propertyMap->insert(std::make_pair("Permittivity", &permittivity));
	//propertyMap->insert(std::make_pair("Conductivity", &conductivity));
}

std::vector<std::pair<std::string, float*>>* Carrier_PO::getPropertyMap()
{
	return propertyMap;
}
