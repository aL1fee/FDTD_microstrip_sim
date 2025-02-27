#include "Wire_PO.h"

void Wire_PO::updatePropertyMap()
{
	propertyMap->push_back(std::make_pair("Diameter (mm)", &diameter));
	propertyMap->push_back(std::make_pair("Permittivity (F/m)", &permittivity));
	propertyMap->push_back(std::make_pair("Conductivity (S/m)", &conductivity));
}
