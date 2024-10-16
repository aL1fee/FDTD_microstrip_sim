#include "Carrier_PO.h"

void Carrier_PO::init()
{
	extern int _preserveLeftClickNum;
	_preserveLeftClickNum = 4;
}

Carrier_PO::Carrier_PO()
{
	init();
}
