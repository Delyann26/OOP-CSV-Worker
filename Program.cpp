#include "Program.h"
#include <fstream>
Program::Program() :isUsedUndo(true), pathOfFile(""), isSaved(true), lastCommand("")
{
}

Program& Program::getInstance()
{
	static Program instance;
	return instance;
}

void Program::startProgram()
{
	std::string command;
	bool isProgramAtBeginning = true;
	unsigned action;
	do
	{
		std::cout << '>';
		std::getline(std::cin, command);
		removeSpacesBeforeAndAfter(command);
		action = getAction(command);

		if (isProgramAtBeginning == true)
		{
			if (action == 0)
			{
				executeCommand(command, action);
			}
			else if (action == 4)
			{
				executeCommand(command, action); break;
			}
			else
			{
				std::cout << "Open the table first!" << std::endl;
				continue;
			}
		}
		else if (isProgramAtBeginning == false && action == 0)
		{
			std::cout << "There is an already opened file!" << std::endl;
		}
		else if (action == 1 || action == 2 || action == 3 || action == 14)//standart
		{
			executeCommand(command, action);
		}
		else if (action == 10 || action == 11 || action == 12 || action == 13)//adding
		{
			executeCommand(command, action);
		}
		else if (action == 4)//exit
		{
			executeCommand(command, action);
			break;
		}
		else if (action == 5 || action == 6 || action == 7 || action == 8 || action == 9)
		{
			executeCommand(command, action);
		}
		else
		{
			std::cout << "Unknown command!" << std::endl;
		}
		if (pathOfFile != "")
		{
			isProgramAtBeginning = false;
		}
	} while (true);
}

void Program::executeCommand(const std::string& command, unsigned commandType)
{
	std::string argument = command.substr(command.find_first_of(' ') + 1, command.size());
	try
	{
		switch (commandType)
		{
		case 0:
			open(argument);
			break;
		case 1:
			save();
			break;
		case 2:
			saveAs(argument);
			break;
		case 3:
			print();
			break;
		case 4:
			exit();
			break;
		case 5:
			sort(table, argument);
			if (lastCommand != "")
			{
				executionOfLastOperation();
			}
			lastCommand = command;
			isSaved = false;
			isUsedUndo = false;
			break;
		case 6:
			filter(table, argument);
			if (lastCommand != "")
			{
				executionOfLastOperation();
			}
			lastCommand = command;
			isSaved = false;
			isUsedUndo = false;
			break;
		case 7:
			removeDuplicates(table);
			if (lastCommand != "")
			{
				executionOfLastOperation();
			}
			lastCommand = command;
			isSaved = false;
			isUsedUndo = false;
			break;
		case 8:
			removeColumn(table, argument);
			if (lastCommand != "")
			{
				executionOfLastOperation();
			}
			lastCommand = command;
			isSaved = false;
			isUsedUndo = false;
			break;
		case 9:
			permutate(table, argument);
			if (lastCommand != "")
			{
				executionOfLastOperation();
			}
			lastCommand = command;
			isSaved = false;
			isUsedUndo = false;
			break;
		case 10:
			copy(table, argument);
			if (lastCommand != "")
			{
				executionOfLastOperation();
			}
			lastCommand = command;
			isSaved = false;
			isUsedUndo = true;
			break;
		case 11:
			minimum(table);
			if (lastCommand != "")
			{
				executionOfLastOperation();
			}
			lastCommand = command;
			isSaved = false;
			isUsedUndo = true;
			break;
		case 12:
			maximum(table);
			if (lastCommand != "")
			{
				executionOfLastOperation();
			}
			lastCommand = command;
			isSaved = false;
			isUsedUndo = true;
			break;
		case 13:
			mostFreqent(table);
			if (lastCommand != "")
			{
				executionOfLastOperation();
			}
			lastCommand = command;
			isSaved = false;
			isUsedUndo = true;
			break;
		case 14:
			undo();
		default:
			break;
		}
	}
	catch (const std::bad_alloc&)
	{
		save();
		throw;
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}

void Program::executionOfLastOperation()
{
	std::string argument = lastCommand.substr(lastCommand.find_first_of(' ') + 1, lastCommand.size());
	unsigned commandType = getAction(lastCommand);

	switch (commandType)
	{
	case 5:
		sort(previousState, argument);
		break;
	case 6:
		filter(previousState, argument);
		break;
	case 7:
		removeDuplicates(previousState);
		break;
	case 8:
		removeColumn(previousState, argument);
		break;
	case 9:
		permutate(previousState, argument);
		break;
	case 10:
		copy(previousState, argument);
		break;
	case 11:
		minimum(previousState);
		break;
	case 12:
		maximum(previousState);
		break;
	case 13:
		mostFreqent(previousState);
		break;
	default:
		break;
	}
}


void Program::open(const std::string& path)
{
	std::string filename = getFileName(path);
	std::ifstream ifs(path);
	if (!ifs.is_open())
	{
		std::cout << "File not found!" << std::endl;
		saveAs(path);
		return;
	}
	try
	{
		table.readFromCSVFile(ifs);
	}
	catch (const std::bad_alloc&)
	{
		ifs.close();
		throw;
	}
		
	ifs.close();
	previousState = table;
	pathOfFile = path;
	std::cout << "Successfully opened " << filename << std::endl;
}

void Program::save() const
{
	std::ofstream ofs(pathOfFile);
	if (!ofs.is_open())
	{
		throw std::runtime_error("Saving failed!");
	}
	table.writeToCSVFile(ofs);
	std::cout << "Successfully saved " << getFileName(pathOfFile) << std::endl;
	isSaved = true;
	ofs.close();
}

void Program::saveAs(const std::string& filePath) const
{
	std::ofstream ofs(filePath);
	if (!ofs.is_open())
	{
		throw std::runtime_error("Saving failed!");
	}
	table.writeToCSVFile(ofs);
	std::cout << "Successfully saved another " << getFileName(filePath) << std::endl;
	isSaved = true;
	ofs.close();
}

void Program::print() const
{
	table.print();
}

void Program::exit()
{
	if (isSaved == false)
	{
		char c;
		std::cout << "Save your changes to this file? y/n" << std::endl;
		std::cin >> c;
		if (c == 'y')
		{
			save();
		}
		else if (c == 'n')
		{
			return;
		}
		else
		{
			exit();
		}
	}
}

void Program::undo()
{
	if (isUsedUndo == true)
	{
		std::cout << "Cannot use undo!" << std::endl;
		return;
	}

	table = previousState; 

	isSaved = false;
	isUsedUndo = true;
	lastCommand = "";
}

void Program::sort(Table& table, const std::string& argument)
{
	std::vector<std::string> tokens = tokenize(argument);
	if (tokens.size() != 2)
	{
		throw std::invalid_argument("Sorting failed: Wrong syntax!");
	}
	std::string sortBy = tokens[0];//header or index
	std::string sortingWay = tokens[1]; //ascending descending

	if (sortBy.size() == 1)
	{
		unsigned number = stringToNumber(sortBy);
		if (sortingWay == "ascending")
			table.sortByIndexOfColInAscendingOrder(number);
		else if (sortingWay == "descending")
			table.sortByIndexOfColInDescendingOrder(number);
		else
			throw std::invalid_argument("Sorting failed: Wrong syntax!");
	}
	else if (sortBy.size() > 1)
	{
		if (sortingWay == "ascending")
			table.sortByHeaderOfColInAscendingOrder(sortBy);
		else if (sortingWay == "descending")
			table.sortByHeaderOfColInDescendingOrder(sortBy);
		else
			throw std::invalid_argument("Sorting failed: Wrong syntax!");
	}
}

void Program::filter(Table& table, const std::string& argument)
{
	std::vector<std::string> tokens = tokenize(argument);
	if (tokens.size() != 3)
	{
		throw std::invalid_argument("Filtering failed: Wrong syntax!");
	}
	std::string sortBy = tokens[0];
	std::string wayOfFiltering = tokens[1];
	std::string condition = tokens[2];

	if (sortBy.size() == 1)
	{
		unsigned number = stringToNumber(sortBy);
		table.filterByColumnIndex(number, wayOfFiltering, condition);
	}
	else if (sortBy.size() > 1)
	{
		table.filterByHeader(sortBy, wayOfFiltering, condition);
	}
	else
	{
		throw std::invalid_argument("Filtering failed: Wrong syntax!");
	}
}

void Program::removeDuplicates(Table& table)
{	
	table.removeDuplicates();
}

void Program::removeColumn(Table& table, const std::string& argument)
{
	std::vector<std::string> tokens = tokenize(argument);
	if (tokens.size() != 1)
	{
		throw std::invalid_argument("Removing failed: Wrong syntax!");
	}
	if (tokens[0].size() == 1)
	{
		unsigned number = stringToNumber(tokens[0]);
		table.removeCol(number);
	}
	else if (tokens[0].size() > 1)
	{
		table.removeCol(tokens[0]);
	}
	else
	{
		throw std::invalid_argument("Removing failed: Wrong syntax!");
	}
}

void Program::permutate(Table& table, const std::string& argument)
{
	table.changeColumnsSequence(argument);
}

void Program::copy(Table& table, const std::string& argument)
{
	unsigned number = stringToNumber(argument);
	table.copyRow(number);
}

void Program::minimum(Table& table)
{
	table.addRowWithMinValues();
}

void Program::maximum(Table& table)
{
	table.addRowWithMaxValues();
}

void Program::mostFreqent(Table& table)
{
	table.addRowFromMostFrequentValues();
}

unsigned Program::getAction(const std::string& command) const
{
	std::string action = command.substr(0, command.find_first_of(' '));

	if (action == "open")
	{
		return 0;
	}
	else if (action == "save")
	{
		return 1;
	}
	else if (action == "save_as")
	{
		return 2;
	}
	else if (action == "print")
	{
		return 3;
	}
	else if (action == "exit")
	{
		return 4;
	}
	else if (action == "sort")
	{
		return 5;
	}
	else if (action == "filter")
	{
		return 6;
	}
	else if (action == "remove_duplicates")
	{
		return 7;
	}
	else if (action == "remove")
	{
		return 8;
	}
	else if (action == "permutate")
	{
		return 9;
	}
	else if (action == "copy")
	{
		return 10;
	}
	else if (action == "minimum")
	{
		return 11;
	}
	else if (action == "maximum")
	{
		return 12;
	}
	else if (action == "most_frequent")
	{
		return 13;
	}
	else if (action == "undo")
	{
		return 14;
	}
	else
	{
		return -1;
	}
}

const std::string Program::getFileName(const std::string& path) const
{
	std::string copy = path;
	size_t index = copy.find_last_of('\\');
	std::string result;
	if (index != std::string::npos)
	{
		copy = path.substr(index + 1, path.size());
	}
	size_t indexOfFormat = copy.find_last_of('.');
	if (indexOfFormat == std::string::npos || copy.substr(indexOfFormat, copy.size()) != ".csv")
	{
		throw std::runtime_error("Invalid file format!");
	}
	return copy;
}

const std::vector<std::string> Program::tokenize(const std::string& str) const
{
	std::vector<std::string> tokens;
	std::string token;
	bool insideQuotes = false;
	for (char c : str)
	{
		if (c == '"' || c == '\'')
		{
			insideQuotes = !insideQuotes;
			continue;
		}
		if (c == ' ' && insideQuotes == false)
		{
			tokens.push_back(token);
			token.clear();
		}
		else
		{
			token += c;
		}
	}
	tokens.push_back(token);

	return tokens;
}

int Program::stringToNumber(const std::string& str) const
{
	int result = 0;
	for (size_t i = 0; i < str.size(); i++)
	{
		if (!isdigit(str[i]))
		{
			throw std::invalid_argument("Invalid string!");
		}
		result = result * 10 + str[i] - '0';
	}
	return result;
}

void Program::removeSpacesBeforeAndAfter(std::string& str) const
{
	size_t start = str.find_first_not_of(' ');
	if (start == std::string::npos) {
		str.clear();
		return;
	}
	str.erase(0, start);

	size_t end = str.find_last_not_of(' ');

	str.erase(end + 1);
}
