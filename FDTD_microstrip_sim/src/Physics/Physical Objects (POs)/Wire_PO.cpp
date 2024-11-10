#include "Wire_PO.h"

void Wire_PO::updatePropertyMap()
{
	propertyMap->push_back(std::make_pair("Diameter", &diameter));
	propertyMap->push_back(std::make_pair("Permittivity", &permittivity));
	propertyMap->push_back(std::make_pair("Conductivity", &conductivity));
}
