#pragma once
#include "Cell.h"

#include <cassert>


class CellFactory
{
public:
	static CellFactory& getInstance();
	void registerCell(const CellCreator* crt);
	Cell* createCell(const std::string& str);

private:
	CellFactory();
	~CellFactory() = default;
	CellFactory(const CellFactory& other) = delete;
	CellFactory& operator=(const CellFactory& other) = delete;

	const CellCreator* getCreator(int type) const;
	int getDataType(const std::string& str) const;

private:
	static const int MAX_CELL_CREATORS = 10;
	const CellCreator* creators[MAX_CELL_CREATORS];
	size_t size;

	bool isText(const std::string& str) const;
	bool isFacultyNumber(const std::string& str) const;
	bool isDate(const std::string& str) const;
	bool isNumber(const std::string& str) const;

};
