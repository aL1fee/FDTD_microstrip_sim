#include "Ribbon_PO.h"

void Ribbon_PO::updatePropertyMap()
{
	propertyMap->push_back(std::make_pair("X position", getOriginX()));
	propertyMap->push_back(std::make_pair("Z position", getOriginZ()));
	propertyMap->push_back(std::make_pair("Y position", getOriginY()));
	propertyMap->push_back(std::make_pair("Curve length", getLength()));
	propertyMap->push_back(std::make_pair("Tail lengths", getTailLength()));
	propertyMap->push_back(std::make_pair("Width", getWidth()));
	propertyMap->push_back(std::make_pair("Ribbon height", getHeight()));
	propertyMap->push_back(std::make_pair("Curve height", getCurveHeight()));
	propertyMap->push_back(std::make_pair("Permittivity", &permittivity));
	propertyMap->push_back(std::make_pair("Conductivity", &conductivity));
}
