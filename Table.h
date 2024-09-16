#pragma once
#include "Row.h"
#include <fstream>

class Table
{
public:
	Table();
	Table(const Table& other);
	Table& operator=(const Table& other);
	~Table();

	void print() const;

	void writeToCSVFile(std::ofstream& ofs) const;
	void readFromCSVFile(std::ifstream& ifs);

	void sortByHeaderOfColInAscendingOrder(const std::string& header);
	void sortByHeaderOfColInDescendingOrder(const std::string& header);

	void sortByIndexOfColInAscendingOrder(size_t index);
	void sortByIndexOfColInDescendingOrder(size_t index);

	void filterByHeader(const std::string& header, const std::string& wayOfFiltering, const std::string& condition);
	void filterByColumnIndex(size_t index, const std::string& wayOfFiltering, const std::string& condition);

	void changeColumnsSequence(const std::string& str);

	void removeDuplicates();

	void removeCol(const std::string& header);
	void removeCol(size_t index);

	void copyRow(size_t index);

	void addRowWithMinValues();
	void addRowWithMaxValues();

	void addRowFromMostFrequentValues();

	size_t getColumnsSize() const;

private:
	Row* rows;
	size_t rowsCount;
	size_t rowsCapacity;

	bool hasHeaders;
	std::vector<std::string> headers;

private:
	void addRow(const Row& toAdd);

	bool isValidFile(std::ifstream& ifs) const;
	bool hasValidChars(const std::string& str)const;

	unsigned countCommas(const std::string& str)const;
	bool isSync(const Row& row) const;

	void setColumns(const std::string& line);

	void free();
	void resize(size_t newCapacity);

	const std::vector<Row> getNullRows(size_t index) const;
	void removeNullRows(size_t index, const std::vector<Row>& nullRows);
	void addNullRows(const std::vector<Row>& nullRows);

	void removeRow(size_t index);

	size_t getHeaderIndex(const std::string& str) const;

	bool checkForHeaders(const std::string& line) const;
	void fillArray(size_t* delimitersPosition, size_t size) const;
	int getWayOfFiltering(const std::string& str) const;
	const Cell* getTypeOfCondition(const std::string& str) const;

	void permutateHeaders(const std::vector<unsigned>& permutation);
	const std::vector<unsigned> getPermutation(const std::string& str) const;
	bool isValidPermutation(const std::vector<unsigned>& permutation) const;

	const Cell* getMinInColumn(size_t index) const;
	const Cell* getMaxInColumn(size_t index) const;
	struct Pair
	{
		Cell* value = nullptr;
		unsigned amount = 0;
	};
	Cell* getMostFrequent(size_t index) const;
	int isInPairs(const std::vector<Pair>& pairs, const Cell* cell, int& index) const;
	unsigned getMostFrequentAmount(const std::vector<Pair>& pairs) const;
	void sortPairsByAmount(std::vector<Pair>& pairs) const;
	size_t getFirstDifferentAmount(const std::vector<Pair>& pairs, unsigned max) const;
};

