#include "Plot.h"

std::string Plot::line(int length) {
	std::string ret = "";
	for (int i = 0; i < length; i++) {
		ret += "-";
	}
	return ret;
}

Plot::Plot() {
	title = "Plot";
}

void Plot::histogram(std::vector<float> x, int bins) {
	float max = Calc::Max(x);
	float min = Calc::Min(x);
	float range = max - min;
	float binwidth = range / (float)bins;
	std::vector<float> counts;
	for (float i = 0; i < bins; i++) {
		int count = 0;
		std::cout << min + i * binwidth << ", " << min + (i + 1) * binwidth << std::endl;
		for (int j = 0; j < x.size(); j++){
			count += x[j] >= min + i * binwidth && x[j] < min + (i + 1) * binwidth ? 1 : 0;
		}
		counts.push_back(count);
	}
	for (int i = 0; i < counts.size(); i++) {
		std::cout << counts[i] << std::endl;
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
	std::cout << line(2 * bins) << std::endl;
}

void Plot::setTitle(std::string t)
{
	title = t;
}

