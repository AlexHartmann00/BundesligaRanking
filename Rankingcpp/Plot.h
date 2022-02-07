#pragma once
#include<vector>
#include "Calc.h"
#include <iostream>
#include <string>

class Plot {
private:
	std::string title;
	std::string line(int);
public:
	Plot();
	void histogram(std::vector<float>,int);
	void setTitle(std::string);
};