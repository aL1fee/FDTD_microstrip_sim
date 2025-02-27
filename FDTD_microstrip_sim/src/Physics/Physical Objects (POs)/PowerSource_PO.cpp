#include "PowerSource_PO.h"

void PowerSource_PO::updatePropertyMap()
{
	propertyMap->push_back(std::make_pair("X position (mm)", getOriginX()));
	propertyMap->push_back(std::make_pair("Z position (mm)", getOriginZ()));
	propertyMap->push_back(std::make_pair("Y position (mm)", getOriginY()));
	propertyMap->push_back(std::make_pair("Radius (mm)", getRadius()));
	propertyMap->push_back(std::make_pair("Length (mm)", getLength()));
	propertyMap->push_back(std::make_pair("Rotation angle (deg)", getRotationAngle()));
	propertyMap->push_back(std::make_pair("Permittivity (F/m)", &permittivity));
	propertyMap->push_back(std::make_pair("Conductivity (S/m)", &conductivity));



	propertyMap->push_back(std::make_pair("Power Level (dBm)", &power));
	propertyMap->push_back(std::make_pair("CW Freq. (GHz)", &frequency));
	propertyMap->push_back(std::make_pair("Wavelength (mm)", &wavelength));


}
