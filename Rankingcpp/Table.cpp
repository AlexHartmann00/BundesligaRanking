#include "Table.h"

int getMaxStringLength(std::vector<std::string> vec) {
	int size = vec.size();
	int max = 0;
	for (int i = 0; i < size; i++) {
		if (vec[i].length() > max) {
			max = vec[i].length();
		}
	}
	return max;
}

std::string empty(int length) {
	std::string out = "";
	for (int i = 0; i < length; i++) {
		out += " ";
	}
	return out;
}

std::string line(int length) {
	std::string out = "";
	for (int i = 0; i < length; i++) {
		out += "-";
	}
	return out;
}

Table::Table()
{
	title = "Tabelle";
	totalWidth = 0;
	padding = 1;
}

void Table::addColumn(std::vector<std::string> x)
{
	data.push_back(x);
	columnWidths.push_back(getMaxStringLength(x) + padding);
	totalWidth += getMaxStringLength(x) + 1 + padding;
}

void Table::setTitle(std::string x) {
	title = x;
}

void Table::addColumn(std::vector<float> x)
{
	std::vector<std::string> strx;
	for (int i = 0; i < x.size(); i++) {
		strx.push_back(std::to_string(x[i]));
	}
	addColumn(strx);
}

void Table::addColumn(std::vector<int> x)
{
	std::vector<std::string> strx;
	for (int i = 0; i < x.size(); i++) {
		strx.push_back(std::to_string(x[i]));
	}
	addColumn(strx);
}

void Table::setColnames(std::vector<std::string> x)
{
	colnames = x;
	totalWidth = 0;
	for (int i = 0; i < data.size(); i++) {
		columnWidths[i] = Calc::Max(x[i].length(), columnWidths[i]);
		totalWidth += columnWidths[i] + 1 + padding;
	}
}

std::string Table::getStringRepresentation() {
	std::string ret = "";
	totalWidth += showRankColumn ? 4 + padding + 1 : 0;
	ret += line(totalWidth) + "\n";
	ret += title + "\n" + line(totalWidth) + "\n";
	if (showRankColumn) {
		ret += "Rang" + empty(padding) +  "|";
	}
	for (int i = 0; i < colnames.size(); i++) {
		int len = colnames[i].length();
		int blank = columnWidths[i] - len + padding;
		ret += colnames[i] + empty(blank) + "|";
	}
	ret += "\n" + line(totalWidth) + "\n";
	for (int i = 0; i < data[0].size(); i++) {
		if (showRankColumn) {
			int blank = 4 + padding - std::to_string(i).length();
			ret += std::to_string(i+1) + "." + empty(blank-1) + "|";
		}
		for (int j = 0; j < data.size(); j++) {
			int len = data[j][i].length();
			int blank = columnWidths[j] - len + padding;
			ret += data[j][i] + empty(blank) + "|";
		}
		ret += "\n";
	}
	ret += line(totalWidth) + "\n";
	return ret;
}

void Table::sort_by(int column, sort_type type) {
	bool descending = type == sort_type::DESCENDING;
	std::vector<std::vector<std::string>> dataCopy = data;
	std::vector<int> indices;
	for (int i = 0; i < data[column].size(); i++) {
		int index = descending ? data[column].size() - Calc::getSortedArrayIndices(data[column], data[column][i]) - 1 : Calc::getSortedArrayIndices(data[column], data[column][i]);
		indices.push_back(index);
	}
	for (int i = 0; i < indices.size(); i++) {
		for (int j = 0; j < indices.size(); j++) {
			if (i == indices[j]) {
				for (int k = 0; k < data.size(); k++) {
					data[k][i] = dataCopy[k][j];
				}
			}
		}
	}
	showRankColumn = true;
}

Table Table::subset(int from, int to) {
	while (data[0].size() > to+1) {
		for (int k = 0; k < data.size(); k++) {
			data[k].pop_back();
		}
	}
	return *this;
}

void Table::setRankColumn(bool arg) {
	showRankColumn = arg;
}

