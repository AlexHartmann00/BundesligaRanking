#include "Table.h"

void Table::displayTable(std::vector<std::string> names, std::vector<float> values, int amount, std::string title) {
	std::vector<int> p;
	std::vector<std::string> snames;
	std::vector<float> svalues;

	int size = values.size();

	for (int i = 0; i < size; i++) {
		p.push_back(Calc::getSortedArrayIndices(values,values[i]));
		//std::cout << "entry " << i << " is the " << p[i] << " highest value" << std::endl;
	}
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (p[j] == i) {
				if (snames.size() < amount) {
					snames.push_back(names[j]);
					svalues.push_back(values[j]);

				}
			}
		}
	}
	/*for (int i = 0; i < fmin(amount,size); i++) {
		std::cout << (i+1) << ". " << snames[i] << " " << svalues[i] << std::endl;
	}*/
	std::vector<std::string> labels;
	labels.push_back("");
	labels.push_back("Name");
	labels.push_back("Wert");
	tabular(snames, svalues, true, labels, title);
}

std::string Table::writeableTable(std::vector<std::string> names, std::vector<float> values, int amount, std::string title)
{
	std::vector<int> p;
	std::vector<std::string> snames;
	std::vector<float> svalues;

	int size = values.size();

	for (int i = 0; i < size; i++) {
		p.push_back(Calc::getSortedArrayIndices(values, values[i]));
		//std::cout << "entry " << i << " is the " << p[i] << " highest value" << std::endl;
	}
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (p[j] == i) {
				if (snames.size() < amount) {
					snames.push_back(names[j]);
					svalues.push_back(values[j]);
				}
			}
		}
	}
	/*for (int i = 0; i < fmin(amount,size); i++) {
		std::cout << (i+1) << ". " << snames[i] << " " << svalues[i] << std::endl;
	}*/
	std::vector<std::string> labels;
	labels.push_back("");
	labels.push_back("Name");
	labels.push_back("Wert");
	return sTabular(snames, svalues, true, labels, title);
}

std::string Table::writeableTable(std::vector<std::string> names, std::vector<float> values, std::vector<std::string> teamNames , int amount, std::string title)
{
	std::vector<int> p;
	std::vector<std::string> snames;
	std::vector<float> svalues;
	std::vector<std::string> sTeamNames;

	int size = values.size();

	for (int i = 0; i < size; i++) {
		p.push_back(Calc::getSortedArrayIndices(values, values[i]));
		//std::cout << "entry " << i << " is the " << p[i] << " highest value" << std::endl;
	}
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (p[j] == i) {
				if (snames.size() < amount) {
					snames.push_back(names[j]);
					svalues.push_back(values[j]);
					sTeamNames.push_back(teamNames[j]);
				}
			}
		}
	}
	/*for (int i = 0; i < fmin(amount,size); i++) {
		std::cout << (i+1) << ". " << snames[i] << " " << svalues[i] << std::endl;
	}*/
	std::vector<std::string> labels;
	labels.push_back("");
	labels.push_back("Name");
	labels.push_back("Wert");
	return AsTable(snames, svalues, sTeamNames, title);
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

void Table::tabular(std::vector<std::string> names, std::vector<float> values, bool ranked, std::vector<std::string> labels, std::string title)
{
	std::string output = "";
	std::string endl = "\n";
	std::string sepLine = "----------------------------------------------------";
	std::cout << std::endl << sepLine << std::endl << title << std::endl;
	output += endl + sepLine + endl + title + endl;
	std::vector<std::string> ranks;
	int length[3] = { 4, 0, 10 };
	int MAX_length = 0;
	int size = names.size();
	for (int i = 0; i < size; i++) {
		if (names[i].length() > MAX_length) {
			MAX_length = names[i].length();
			length[1] = MAX_length + 3;
		}
		if (ranked) {
			ranks.push_back(std::to_string(i + 1) + ".");
		}
		else {
			ranks.push_back("");
		}
	}
	for (int i = 0; i < labels.size(); i++) {
		std::cout << std::setw(length[i] + 1) << std::left << labels[i];
		output += labels[i] + empty(length[i] + 1 - labels[i].length());
	}
	std::cout << std::endl << sepLine << std::endl;
	output += endl + sepLine + endl;
	for (int i = 0; i < names.size(); i++) {
		std::cout << std::setw(length[0]) << std::left << ranks[i] << "|" << std::setw(length[1]) << std::left << names[i] << "|" << std::setw(length[2]) << std::left << values[i] << std::endl;
		output += ranks[i] + empty(length[0] - ranks[i].length()) + "|" + names[i] + empty(length[1] - names[i].length()) + "|" + std::to_string(values[i]) + empty(length[2] - std::to_string(values[i]).length()) + endl;
	}
	std::cout << sepLine << std::endl;
	output += sepLine + endl;

}

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

std::string Table::sTabular(std::vector<std::string> names, std::vector<float> values, bool ranked, std::vector<std::string> labels, std::string title)
{
	// current date/time based on current system
	time_t now = time(0);

	// convert now to string form
	char* dt = ctime(&now);	std::string output = "";
	std::string endl = "\n";
	std::string sepLine = "----------------------------------------------------";
	output += endl + sepLine + endl + title + endl;
	std::vector<std::string> ranks;
	int length[3] = { 4, 0, 10 };
	int MAX_length = 0;
	int size = names.size();
	for (int i = 0; i < size; i++) {
		if (names[i].length() > MAX_length) {
			MAX_length = names[i].length();
			length[1] = MAX_length + 3;
		}
		if (ranked) {
			ranks.push_back(std::to_string(i + 1) + ".");
		}
		else {
			ranks.push_back("");
		}
	}
	for (int i = 0; i < labels.size(); i++) {
		output += labels[i] + empty(length[i] + 1 - labels[i].length());
	}
	output += endl + sepLine + endl;
	for (int i = 0; i < names.size(); i++) {
		output += ranks[i] + empty(length[0] - ranks[i].length()) + "|" + names[i] + empty(length[1] - names[i].length()) + "|" + std::to_string(values[i]) + empty(length[2] - std::to_string(values[i]).length()) + endl;
	}
	output += sepLine + endl + dt + endl;
	return output;
}

std::string Table::AsTable(std::vector<std::string> v1, std::vector<std::string> v2, std::vector<std::string> v3, std::vector<std::string> labels = { "v1", "v2", "v3" })
{
	std::string out = "";
	std::string endl = "\n";
	int length[3];
	int maxlab = getMaxStringLength(labels);
	length[0] = getMaxStringLength(v1) + labels[0].length();
	length[1] = getMaxStringLength(v2) + labels[1].length();
	length[2] = getMaxStringLength(v3) + labels[2].length();
	int size = v1.size();
	std::string sepLine = line(length[0] + length[1] + length[2]);
	out += endl + sepLine + endl;
	out += labels[0] + empty(length[0] - labels[0].length()) + "|" + labels[1] + empty(length[1] - labels[1].length()) + "|" + labels[2] + empty(length[2] - labels[2].length()) + endl + sepLine + endl;
	for (int i = 0; i < size; i++) {
		out += v1[i] + empty(length[0] - v1[i].length()) + "|" + v2[i] + empty(length[1] - v2[i].length()) + "|" + v3[i] + empty(length[2] - v3[i].length()) + endl;
	}
	out += sepLine + endl;
	return out;
}


std::string Table::AsTable(std::vector<std::string> v1, std::vector<float> V2, std::vector<float> V3, std::vector<std::string> labels = { "v1", "v2", "v3" })
{
	int size = v1.size();
	std::vector<std::string> v2, v3;
	for (int i = 0; i < size; i++)
	{
		v2.push_back(std::to_string(V2[i]));
		v3.push_back(std::to_string(V3[i]));
	}
	std::string out = "";
	std::string endl = "\n";
	int length[3];
	int maxlab = getMaxStringLength(labels);
	length[0] = getMaxStringLength(v1) + labels[0].length();
	length[1] = getMaxStringLength(v2) + labels[1].length();
	length[2] = getMaxStringLength(v3) + labels[2].length();
	std::string sepLine = line(length[0] + length[1] + length[2]);
	out += endl + sepLine + endl;
	out += labels[0] + empty(length[0] - labels[0].length()) + "|" + labels[1] + empty(length[1] - labels[1].length()) + "|" + labels[2] + empty(length[2] - labels[2].length()) + endl + sepLine + endl;
	for (int i = 0; i < size; i++) {
		out += v1[i] + empty(length[0] - v1[i].length()) + "|" + v2[i] + empty(length[1] - v2[i].length()) + "|" + v3[i] + empty(length[2] - v3[i].length()) + endl;
	}
	out += sepLine + endl;
	return out;
}


std::string Table::AsTable(std::vector<std::string> v1, std::vector<float> V2, std::vector<float> V3, std::vector<float> V4, std::vector<std::string> labels = { "v1", "v2", "v3" })
{
	int size = v1.size();
	std::vector<std::string> v2, v3, v4;
	for (int i = 0; i < size; i++)
	{
		v2.push_back(std::to_string(V2[i]));
		v3.push_back(std::to_string(V3[i]));
		v4.push_back(std::to_string(V4[i]));
	}
	std::string out = "";
	std::string endl = "\n";
	int length[4];
	int maxlab = getMaxStringLength(labels);
	length[0] = getMaxStringLength(v1) + labels[0].length();
	length[1] = getMaxStringLength(v2) + labels[1].length();
	length[2] = getMaxStringLength(v3) + labels[2].length();
	length[3] = getMaxStringLength(v4) + labels[3].length();
	std::string sepLine = line(length[0] + length[1] + length[2] + length[3]);
	out += endl + sepLine + endl;
	out += labels[0] + empty(length[0] - labels[0].length()) + "|" + labels[1] + empty(length[1] - labels[1].length()) + "|" + labels[2] + empty(length[2] - labels[2].length()) + "|" + labels[3] + empty(length[3] - labels[3].length()) + endl + sepLine + endl;
	for (int i = 0; i < size; i++) {
		out += v1[i] + empty(length[0] - v1[i].length()) + "|" + v2[i] + empty(length[1] - v2[i].length()) + "|" + v3[i] + empty(length[2] - v3[i].length()) + "|" + v4[i] + empty(length[3] - v4[i].length()) + endl;
	}
	out += sepLine + endl;
	return out;
}

std::string Table::AsTable(std::vector<int> V1, std::vector<std::string> V2, std::vector<float> V3, std::vector<std::string> labels = { "ID", "Name", "Wert" }, std::string title = "Spieler")
{
	int size = V1.size();
	std::vector<std::string> v1, v2, v3;
	for (int i = 0; i < size; i++)
	{
		v1.push_back(std::to_string(V1[i]));
		v2.push_back(V2[i]);
		v3.push_back(std::to_string(V3[i]));
	}
	std::string out = "";
	std::string endl = "\n";
	int length[3];
	int maxlab = getMaxStringLength(labels);
	length[0] = getMaxStringLength(v1) + labels[0].length();
	length[1] = getMaxStringLength(v2) + labels[1].length();
	length[2] = getMaxStringLength(v3) + labels[2].length();
	std::string sepLine = line(length[0] + length[1] + length[2]);
	out += endl + sepLine + endl;
	out += labels[0] + empty(length[0] - labels[0].length()) + "|" + labels[1] + empty(length[1] - labels[1].length()) + "|" + labels[2] + empty(length[2] - labels[2].length()) + endl + sepLine + endl;
	for (int i = 0; i < size; i++) {
		out += v1[i] + empty(length[0] - v1[i].length()) + "|" + v2[i] + empty(length[1] - v2[i].length()) + "|" + v3[i] + empty(length[2] - v3[i].length()) + endl;
	}
	out += sepLine + endl;
	return out;
}

std::string Table::AsTable(std::vector<std::string> V1, std::vector<float> V2, std::vector<std::string> V3, std::string title)
{
	int size = V1.size();
	std::vector<std::string> v1, v2, v3, labels = { "Name", "Wert", "Team" };
	for (int i = 0; i < size; i++)
	{
		v1.push_back(V1[i]);
		v2.push_back(std::to_string(V2[i]));
		v3.push_back(V3[i]);
	}
	std::string out = "";
	std::string endl = "\n";
	int length[4];
	int maxlab = getMaxStringLength(labels);
	length[0] = getMaxStringLength(v1) + labels[0].length();
	length[1] = getMaxStringLength(v2) + labels[1].length();
	length[2] = getMaxStringLength(v3) + labels[2].length();
	std::string sepLine = line(length[0] + length[1] + length[2]);
	out += endl + sepLine + endl;
	out += empty(5) + "|" + labels[0] + empty(length[0] - labels[0].length()) + "|" + labels[1] + empty(length[1] - labels[1].length()) + "|" + labels[2] + empty(length[2] - labels[2].length()) + endl + sepLine + endl;
	for (int i = 0; i < size; i++) {
		out += std::to_string((i+1)) + "." + empty(5 - std::to_string((i + 1)).length()) + "|" + v1[i] + empty(length[0] - v1[i].length()) + "|" + v2[i] + empty(length[1] - v2[i].length()) + "|" + v3[i] + empty(length[2] - v3[i].length()) + endl;
	}
	out += sepLine + endl;
	return out;
}

std::string Table::AsTable(std::vector<int> V1, std::vector<std::string> V2, std::vector<float> V3, std::vector<std::string> V4, std::vector<std::string> labels = { "v1", "v2", "v3", "v4" }, std::string title = "Tabelle")
{
	int size = V1.size();
	std::vector<std::string> v1, v2, v3, v4;
	for (int i = 0; i < size; i++)
	{
		v1.push_back(std::to_string(V1[i]));
		v2.push_back(V2[i]);
		v3.push_back(std::to_string(V3[i]));
		v4.push_back(V4[i]);
	}
	std::string out = "";
	std::string endl = "\n";
	int length[4];
	int maxlab = getMaxStringLength(labels);
	length[0] = getMaxStringLength(v1) + labels[0].length();
	length[1] = getMaxStringLength(v2) + labels[1].length();
	length[2] = getMaxStringLength(v3) + labels[2].length();
	length[3] = getMaxStringLength(v4) + labels[3].length();
	std::string sepLine = line(length[0] + length[1] + length[2] + length[3]);
	out += endl + sepLine + endl + title + endl;
	out += labels[0] + empty(length[0] - labels[0].length()) + "|" + labels[1] + empty(length[1] - labels[1].length()) + "|" + labels[2] + empty(length[2] - labels[2].length()) + "|" + labels[3] + empty(length[3] - labels[3].length()) + endl + sepLine + endl;
	for (int i = 0; i < size; i++) {
		out += v1[i] + empty(length[0] - v1[i].length()) + "|" + v2[i] + empty(length[1] - v2[i].length()) + "|" + v3[i] + empty(length[2] - v3[i].length()) + "|" + v4[i] + empty(length[3] - v4[i].length()) + endl;
	}
	out += sepLine + endl;
	return out;
}

std::string Table::AsTable(std::vector<std::string> V1, std::vector<float> V2, std::string title)
{
	int size = V1.size();
	std::vector<std::string> v1, v2, labels = { "Name", "Wert"};
	for (int i = 0; i < size; i++)
	{
		v1.push_back(V1[i]);
		v2.push_back(std::to_string(V2[i]));
	}
	std::string out = "";
	std::string endl = "\n";
	int length[2];
	int maxlab = getMaxStringLength(labels);
	length[0] = getMaxStringLength(v1) + labels[0].length();
	length[1] = getMaxStringLength(v2) + labels[1].length();
	std::string sepLine = line(length[0] + length[1]);
	out += title + endl + sepLine + endl;
	out += empty(5) + "|" + labels[0] + empty(length[0] - labels[0].length()) + "|" + labels[1] + empty(length[1] - labels[1].length()) + endl + sepLine + endl;
	for (int i = 0; i < size; i++) {
		out += std::to_string((i + 1)) + "." + empty(5 - std::to_string((i + 1)).length()) + "|" + v1[i] + empty(length[0] - v1[i].length()) + "|" + v2[i] + empty(length[1] - v2[i].length()) + endl;
	}
	out += sepLine + endl;
	return out;
}

