#include "TuningPadArray_PO.h"

void TuningPadArray::updatePropertyMap()
{
	propertyMap->push_back(std::make_pair("X position (mm)", getOriginX()));
	propertyMap->push_back(std::make_pair("Z position (mm)", getOriginZ()));
	propertyMap->push_back(std::make_pair("Y position (mm)", getOriginY()));
	propertyMap->push_back(std::make_pair("X pad size (mm)", getSizeX()));
	propertyMap->push_back(std::make_pair("Z pad size (mm)", getSizeZ()));
	propertyMap->push_back(std::make_pair("X pad separ. (mm)", getPadSeparationX()));
	propertyMap->push_back(std::make_pair("Z pad separ. (mm)", getPadSeparationZ()));
	propertyMap->push_back(std::make_pair("Height (mm)", getHeight()));
	propertyMap->push_back(std::make_pair("Permittivity (F/m)", &permittivity));
	propertyMap->push_back(std::make_pair("Conductivity (S/m)", &conductivity));
	propertyMapInt->push_back(std::make_pair("X number of pads", getNumPadsX()));
	propertyMapInt->push_back(std::make_pair("Z number of pads", getNumPadsZ()));
}
