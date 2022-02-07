#pragma once
#pragma warning(disable : 4996)

#include <tgmath.h>
#include <cmath>
#include <string>
#include <iostream>
#include <iomanip>
#include <vector>
#include <ctime>
#include "Calc.h"

enum sort_type {
	ASCENDING,
	DESCENDING,
};

class Table
{
private:
	bool showRankColumn;
	int padding;
	int totalWidth;
	std::string title;
	std::vector<int> columnWidths;
	std::vector<std::vector<std::string>> data;
	std::vector<std::string> colnames;
public:
	Table();
	void setTitle(std::string);
	void addColumn(std::vector<std::string>);
	void addColumn(std::vector<float>);
	void addColumn(std::vector<int>);
	void setColnames(std::vector<std::string>);
	void sort_by(int,sort_type);
	void setRankColumn(bool);
	Table subset(int, int);
	std::string getStringRepresentation();
};



