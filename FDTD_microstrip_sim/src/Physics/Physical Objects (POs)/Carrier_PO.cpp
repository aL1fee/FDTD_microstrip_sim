#include "Carrier_PO.h"

void Carrier_PO::updatePropertyMap()
{
	propertyMap->push_back(std::make_pair("X position", getOriginX()));
	propertyMap->push_back(std::make_pair("Z position", getOriginZ()));
	propertyMap->push_back(std::make_pair("Y position", getOriginY()));
	propertyMap->push_back(std::make_pair("Length", getLength()));
	propertyMap->push_back(std::make_pair("Width", getWidth()));
	propertyMap->push_back(std::make_pair("Height", getHeight()));
	propertyMap->push_back(std::make_pair("Permittivity", &permittivity));
	propertyMap->push_back(std::make_pair("Conductivity", &conductivity));
}