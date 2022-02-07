#pragma once

#include <tgmath.h>
#include <vector>
#include <cmath>
#include <string>

class Calc
{
public:
	static float calc(float, float);
	static int sign(float);
	static float Max(float, float);
	static float Max(std::vector<float>);
	static float Min(std::vector<float>);
	static float abs(float);
	static float mean(std::vector<float>);
	static float sd(std::vector<float>);
	static int round(float, float = 0.5f);
	static float clampRoot(float);
	static float incrementAvg(float,float,int);
	static float xGfromProbability(float,float,float,float);
	static float ProbabilityfromxG(float,float,float,float);
	static int getSortedArrayIndices(std::vector<float>,float);
	static int getSortedArrayIndices(std::vector<std::string>,std::string);
	static float getAveragefromHeader(std::vector<float>, int);
	static float oddsfromProbability(float);
	static float Clamp(float, float, float);
	static float wonPossessions(float);
	static float lostPossessions(float);
};

