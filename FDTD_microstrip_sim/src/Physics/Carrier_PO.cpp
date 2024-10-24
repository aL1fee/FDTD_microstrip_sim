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
	propertyMap["Length"] = getLength();


	float& l = getLength();
	propertyMap->insert(std::make_pair("hey", getLength()));

	propertyMap->insert();


	propertyMap->insert(std::make_pair("Length", l));
	propertyMap->insert(std::make_pair("Width", getWidth()));
	propertyMap->insert(std::make_pair("Height", getHeight()));
	propertyMap->insert(std::make_pair("Permittivity", permittivity));


}

std::map<std::string, float&>* Carrier_PO::getPropertyMap()
{
	return propertyMap;
}
