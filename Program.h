#pragma once
#include <iostream>
#include "Table.h"
class Program
{
public:
	static Program& getInstance();
	void startProgram();

private:
	Table table;
	Table previousState;
	std::string lastCommand;
	std::string pathOfFile;
	bool isUsedUndo;
	mutable bool isSaved;

private:
	Program();
	Program(const Program& other) = delete;
	Program& operator=(const Program& other) = delete;

	void executionOfLastOperation();
	void executeCommand(const std::string& command, unsigned commandType);

	void open(const std::string& path);
	void save() const;
	void saveAs(const std::string& filePath) const;
	void print() const;
	void exit();
	void undo();

	void sort(Table& table,const std::string& argument);
	void filter(Table& table, const std::string& argument);
	void removeDuplicates(Table& table);
	void removeColumn(Table& table, const std::string& argument);
	void permutate(Table& table, const std::string& argument);
	void copy(Table& table, const std::string& argument);
	void minimum(Table& table);
	void maximum(Table& table);
	void mostFreqent(Table& table);

	unsigned getAction(const std::string& command) const;
	const std::string getFileName(const std::string& path) const;
	const std::vector<std::string> tokenize(const std::string& str) const;
	int stringToNumber(const std::string& str) const;

	void removeSpacesBeforeAndAfter(std::string& str) const;
};