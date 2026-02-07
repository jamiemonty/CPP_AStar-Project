/*
 `....
`..   `..   `..      `..
`..         `..      `..
`..     `...`.....`...`.....
`..         `..      `..
`..   `..   `..      `..
 `....

Michelle Lynch
*/

#include <iostream>			//	cout
#include <string>			//	string
#include "Programmer.h"		//	Programmer

int Programmer::programmer_count_ = 0;

Programmer::Programmer() : name_{ "Anonymous" }
{
#if VERBOSE
	std::cout << "Programmer Default constructor" << std::endl;
#endif

	programmer_count_++;
}

Programmer::Programmer(std::string name) : name_{ name }
{
#if VERBOSE
	std::cout << "Programmer 1-arg constructor" << std::endl;
#endif

	programmer_count_++;
}

Programmer::Programmer(const Programmer& p) :
	name_{ p.name_ }, languages_{ p.languages_ }
{
#if VERBOSE
	std::cout << "Programmer Copy constructor" << std::endl;
#endif

	programmer_count_++;
}

Programmer::~Programmer()
{
#if VERBOSE
	std::cout << "Programmer Destructor" << std::endl;
#endif
	programmer_count_--;
}

void Programmer::SetName(const std::string& name)
{
	name_ = name;
}

std::ostream& operator<<(std::ostream& output, const Programmer& p)
{
	output << "Name: " << p.name_ << std::endl;
	output << "Languages: ";
	for (auto& r : p.languages_)
		output << r << " ";
	output << std::endl;

	return output;
}
