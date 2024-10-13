#include "StatusWindow.h"

StatusWindow::StatusWindow(std::string m)
{
	message = m;
}

void StatusWindow::setMessage(std::string& m)
{
	message = m;
}

std::string StatusWindow::getMessage() const
{
	return message;
}
