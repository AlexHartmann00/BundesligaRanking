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

class Table
{
private:
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
	std::string getStringRepresentation();
	std::string entryNames;
	float entryValues;
	static void displayTable(std::vector<std::string>, std::vector<float>,int, std::string);
	static std::string writeableTable(std::vector<std::string>, std::vector<float>, int, std::string);
	static std::string writeableTable(std::vector<std::string>, std::vector<float>, std::vector<std::string>, int, std::string);
	static void tabular(std::vector<std::string>, std::vector<float>, bool, std::vector<std::string>, std::string);
	static std::string sTabular(std::vector<std::string> names, std::vector<float> values, bool ranked, std::vector<std::string> labels, std::string title);
	static std::string AsTable(std::vector<std::string>, std::vector<std::string>, std::vector<std::string>, std::vector<std::string>);
	static std::string AsTable(std::vector<std::string>, std::vector<float>, std::vector<float>, std::vector<std::string>);
	static std::string AsTable(std::vector<std::string>, std::vector<float>, std::vector<float>, std::vector<float>, std::vector<std::string>);
	static std::string AsTable(std::vector<int>, std::vector<std::string>, std::vector<float>, std::vector<std::string>, std::string);
	static std::string AsTable(std::vector<std::string>, std::vector<float>, std::vector<std::string>, std::string);
	static std::string AsTable(std::vector<int>, std::vector<std::string>, std::vector<float>, std::vector<std::string>, std::vector<std::string>, std::string);
	static std::string AsTable(std::vector<std::string>, std::vector<float>, std::string);
};

