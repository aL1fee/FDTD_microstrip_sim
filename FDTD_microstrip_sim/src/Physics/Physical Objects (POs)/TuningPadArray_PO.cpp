#include "TuningPadArray_PO.h"

void TuningPadArray::updatePropertyMap()
{
	propertyMap->push_back(std::make_pair("X position", getOriginX()));
	propertyMap->push_back(std::make_pair("Z position", getOriginZ()));
	propertyMap->push_back(std::make_pair("Y position", getOriginY()));
	propertyMap->push_back(std::make_pair("X pad size", getSizeX()));
	propertyMap->push_back(std::make_pair("Z pad size", getSizeZ()));
	propertyMap->push_back(std::make_pair("X pad separation", getPadSeparationX()));
	propertyMap->push_back(std::make_pair("Z pad separation", getPadSeparationZ()));
	propertyMap->push_back(std::make_pair("Height", getHeight()));
	propertyMap->push_back(std::make_pair("Permittivity", &permittivity));
	propertyMap->push_back(std::make_pair("Conductivity", &conductivity));
	propertyMapInt->push_back(std::make_pair("X number of pads", getNumPadsX()));
	propertyMapInt->push_back(std::make_pair("Z number of pads", getNumPadsZ()));
}
