#include "Ribbon_PO.h"

void Ribbon_PO::updatePropertyMap()
{
	propertyMap->push_back(std::make_pair("X position (mm)", getOriginX()));
	propertyMap->push_back(std::make_pair("Z position (mm)", getOriginZ()));
	propertyMap->push_back(std::make_pair("Y position (mm)", getOriginY()));
	propertyMap->push_back(std::make_pair("Curve length (mm)", getLength()));
	propertyMap->push_back(std::make_pair("Tail length (mm)", getTailLength()));
	propertyMap->push_back(std::make_pair("Width (mm)", getWidth()));
	propertyMap->push_back(std::make_pair("Thickness (mm)", getThickness()));
	//propertyMap->push_back(std::make_pair("Ribbon height", getHeight()));
	propertyMap->push_back(std::make_pair("Curve height (mm)", getCurveHeight()));
	propertyMap->push_back(std::make_pair("Permittivity (F/m)", &permittivity));
	propertyMap->push_back(std::make_pair("Conductivity (S/m)", &conductivity));
}
