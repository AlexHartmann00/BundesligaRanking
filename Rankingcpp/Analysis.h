#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <cmath>
#include "Calc.h"


class Analysis
{
public:
	static float getPerformanceIndex(std::vector<float>[3], std::vector<int>, int);
	static float getPerformanceIndex(std::vector<float>, std::vector<float>, std::vector<float>, std::vector<int>, int);
	static float getCorrect1X2Percentage(std::vector<float>, std::vector<float>, std::vector<int>);
	static float getCorrect1X2Percentage(std::vector<int>, std::vector<int>);
	static float getCorrect1X2PercentageWithoutGoalless(std::vector<int> predictedResult, std::vector<int> goals1, std::vector<int> goals2);
	static float getCorrectDoublePercentage(std::vector<float>[3], std::vector<int>);
	static float getCorrectGoalsPercentage(std::vector<float>, std::vector<float>, std::vector<int>, std::vector<int>, int, float = 0.5f);
	static float getCorrectFloorGoalsPercentage(std::vector<float>, std::vector<float>, std::vector<int>, std::vector<int>, int);
	static float getCorrectCeilingGoalsPercentage(std::vector<float>, std::vector<float>, std::vector<int>, std::vector<int>, int);
	static float getAveragePercentageToWin(std::vector<float>, std::vector<float>, std::vector<float>, std::vector<int>);
	static float getAveragePercentageToWin(std::vector<float>[3], std::vector<int>);
	static float getAverageDeviationFromRealGoals(std::vector<float>, std::vector<float>, std::vector<int>, std::vector<int>);
};

