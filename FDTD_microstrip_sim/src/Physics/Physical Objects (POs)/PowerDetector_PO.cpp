#include "PowerDetector_PO.h"

void PowerDetector_PO::updatePropertyMap()
{
	propertyMap->push_back(std::make_pair("X position", getOriginX()));
	propertyMap->push_back(std::make_pair("Z position", getOriginZ()));
	propertyMap->push_back(std::make_pair("Y position", getOriginY()));
	propertyMap->push_back(std::make_pair("Radius", getRadius()));
	propertyMap->push_back(std::make_pair("Length", getLength()));
	propertyMap->push_back(std::make_pair("Permittivity", &permittivity));
	propertyMap->push_back(std::make_pair("Conductivity", &conductivity));
	propertyMap->push_back(std::make_pair("Sensitivity", &sensitivity));
}
