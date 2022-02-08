#include "Plot.h"

std::string Plot::line(int length) {
	std::string ret = "";
	for (int i = 0; i < length; i++) {
		ret += "-";
	}
	return ret;
}

std::string Plot::empty(int length) {
	std::string ret = "";
	for (int i = 0; i < length; i++) {
		ret += " ";
	}
	return ret;
}

Plot::Plot() {
	title = "Plot";
}

void Plot::histogram(std::vector<float> x, int bins) {
	float max = Calc::Max(x);
	float mean = Calc::mean(x);
	float min = Calc::abs(Calc::Min(x)) < max-mean ? 2*mean - max : Calc::Min(x);
	max = max - mean > Calc::abs(Calc::Min(x)) ? max : 2 * mean - min;
	float range = max - min;
	float binwidth = range / (float)bins;
	std::vector<float> counts;
	for (float i = 0; i < bins; i++) {
		int count = 0;
		for (int j = 0; j < x.size(); j++){
			count += x[j] >= min + i * binwidth && x[j] < min + (i + 1) * binwidth ? 1 : 0;
		}
		counts.push_back(count);
	}
	std::cout << line(2 * bins) << std::endl << title << std::endl << line(2 * bins) << std::endl;
	float maxCount = Calc::Max(counts);
	float ystep = Calc::Max(counts) / (float)20;
	float ycut = Calc::Max(counts);
	char box = (char)254u;
	while (ycut > 0) {
		for (int i = 0; i < bins; i++) {
			if (counts[i] > ycut) {
				std::cout << box;
			}
			else{
				std::cout << " ";
			}
			std::cout << " ";
		}
		std::cout << std::endl;
		ycut -= ystep;
	}
	int low = Calc::round(mean - (mean - min) / 2);
	int hi = Calc::round(mean + (max - mean) / 2);
	char tick = 'X';
	std::string tickSep = line(Calc::round(bins / 2 - 1));
	std::cout << tick << tickSep << tick << tickSep << tick << tickSep << tick << tickSep << tick << std::endl;
	int valLen = std::to_string(Calc::round(min)).length();
	int betweenTicks = Calc::round(bins / 2 - valLen);
	std::cout << Calc::round(min) << empty(betweenTicks) << low << empty(betweenTicks) << Calc::round(mean) << empty(betweenTicks) << hi << empty(betweenTicks) << Calc::round(max) << std::endl;
}

void Plot::setTitle(std::string t)
{
	title = t;
}

