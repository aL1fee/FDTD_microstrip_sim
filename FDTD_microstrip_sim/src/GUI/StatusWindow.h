#pragma once

#include <iostream>
#include <string>

class StatusWindow 
{
private:
	std::string message;

public:
	StatusWindow(std::string m);

	void setMessage(std::string& m);
	std::string getMessage() const;


};