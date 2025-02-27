 #include "Substrate_PO.h"

void Substrate_PO::updatePropertyMap()
{
	propertyMap->push_back(std::make_pair("X position (mm)", getOriginX()));
	propertyMap->push_back(std::make_pair("Z position (mm)", getOriginZ()));
	propertyMap->push_back(std::make_pair("Y position (mm)", getOriginY()));
	propertyMap->push_back(std::make_pair("Length (mm)", getLength()));
	propertyMap->push_back(std::make_pair("Width (mm)", getWidth()));
	propertyMap->push_back(std::make_pair("Height (mm)", getHeight()));
	propertyMap->push_back(std::make_pair("Rotation angle (deg)", getRotationAngle()));
	propertyMap->push_back(std::make_pair("Permittivity (F/m)", &permittivity));
	propertyMap->push_back(std::make_pair("Conductivity (S/m)", &conductivity));
}
