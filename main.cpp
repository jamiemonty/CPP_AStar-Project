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

#include <iostream>		    //	cerr, endl
#include <stdexcept>	    //	out_of_range
#include "Programmer.h"		//	Programmer
#include "TestProgrammer.h"	//	Test functions

int main(int argc, char** argv)
{
	try {
		RunTests(argc, argv);
	}
	catch (std::out_of_range& e) {
		std::cerr << std::endl << e.what() << std::endl;
		return -1;
	}
	return 0;
}
