#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "NumberCell.hpp"
#include "CellFactory.h"
#include "Cell.h"

class Row
{
public:
	Row();
	Row(const Row& other);
	Row& operator=(const Row& other);
	~Row();

	void addCell(const std::string& str);
	void addCell(Cell* cell);

	void removeCell(size_t index);

	size_t getSize() const;

	void printRow() const;

	Cell* operator[](size_t index);
	const Cell* operator[](size_t index) const;

	void writeToCSVFile(std::ofstream& ofs) const;
	void readFromCSVFile(std::ifstream& ifs);
	void permutateRow(const std::vector<unsigned>& permutation);

	friend bool operator==(const Row& lhs, const Row& rhs);

private:
	Cell** rowData;
	size_t size;
	size_t capacity;


	void free();
	void resize(size_t newCapacity);
	const std::vector<std::string> tokenize(const std::string& line) const;
};



