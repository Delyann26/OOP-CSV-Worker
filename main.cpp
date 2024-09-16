#include <iostream>
#include <string>
#include <vector>


#include "Program.h"


int main()
{
	try
	{
		Program& program = Program::getInstance();
		program.startProgram();
	}
	catch (const std::exception& e)
	{
		std::cout << e.what();
	}
}
