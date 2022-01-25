#include "Analysis.h"

float probChange(float prob) {
	return prob * (prob - (1 - prob));
}

float mean(std::vector<float> data) {
	float sum = 0;
	int size = data.size();
	for (int i = 0; i < size; i++) {
		sum += data[i];
	}
	return sum / (float)size;
}

float rmse(std::vector<float> real, std::vector<float> mu) {
	std::vector<float> res;
	float dif = 0;
	int size = real.size();
	for (int i = 0; i < size; i++) {
		dif = abs(real[i] - mu[i]);
		res.push_back(dif);
	}
	return mean(res);
}

float bias(std::vector<float> real, std::vector<float> mu) {
	std::vector<float> res;
	int size = real.size();
	for (int i = 0; i < size; i++) {
		res.push_back(real[i] - mu[i]);
	}
	return mean(res);
}

float Analysis::getPerformanceIndex(std::vector<float> probabilities[3], std::vector<int> result, int type = 0)
{
	int size = probabilities[0].size();
	float mu;
	float real;
	std::vector<float> Mu;
	std::vector<float> Real;
	for (int i = 0; i < size; i++) {
		Mu.push_back(probChange(probabilities[0][i]) + probChange(probabilities[1][i]) + probChange(probabilities[2][i]));
		Real.push_back(probChange(probabilities[result[i]][i]));
	}
	mu = mean(Mu) * size;
	real = mean(Real) * size;
	if (type == 0) {
		return rmse(Real, Mu);

	}
	else if (type == 1){
		return bias(Real, Mu);

	}
}

float Analysis::getPerformanceIndex(std::vector<float> prob1, std::vector<float> prob2, std::vector<float> prob3, std::vector<int> result, int type = 0)
{
	std::vector<float> probabilities[3];
	probabilities[0] = prob1;
	probabilities[1] = prob2;
	probabilities[2] = prob3;
	int size = probabilities[0].size();
	float mu;
	float real;
	std::vector<float> Mu;
	std::vector<float> Real;
	for (int i = 0; i < size; i++) {
		Mu.push_back(probChange(probabilities[0][i]) + probChange(probabilities[1][i]) + probChange(probabilities[2][i]));
		Real.push_back(probChange(probabilities[result[i]][i]));
	}
	mu = mean(Mu) * size;
	real = mean(Real) * size;
	if (type == 0) {
		return rmse(Real, Mu);

	}
	else if (type == 1) {
		return bias(Real, Mu);

	}
}

float Analysis::getCorrect1X2Percentage(std::vector<float> xg1, std::vector<float> xg2, std::vector<int> result)
{
	int size = xg1.size();
	std::vector<int> predictedResult;
	for (int i = 0; i < size; i++) {
		if (round(xg1[i]) > round(xg2[i])) {
			predictedResult.push_back(0);
		}
		else if (round(xg1[i]) == round(xg2[i])) {
			predictedResult.push_back(1);
		}
		else {
			predictedResult.push_back(2);
		}
	}
	int correct = 0;
	for (int i = 0; i < size; i++) {
		if (predictedResult[i] == result[i]) {
			correct++;
		}
	}
	return correct / (float)size;

}

float Analysis::getCorrect1X2Percentage(std::vector<int> predictedResult, std::vector<int> result)
{
	int size = predictedResult.size();
	int correct = 0;
	for (int i = 0; i < size; i++) {
		if (predictedResult[i] == result[i]) {
			correct++;
		}
	}
	return correct / (float)size;
}

float Analysis::getCorrect1X2PercentageWithoutGoalless(std::vector<int> predictedResult, std::vector<int> goals1, std::vector<int> goals2) {
	int count = 0;
	int correct = 0;
	int size = goals1.size();
	std::vector<int> result;
	std::vector<int> Presult;
	for (int i = 0; i < size; i++) {
		count++;
		Presult.push_back(predictedResult[i]);
		if (goals1[i] > goals2[i]) {
			result.push_back(0);
		}
		else if (goals2[i] > goals1[i]) {
			result.push_back(2);
		}
		else if (goals1[i] != 0) {
			result.push_back(1);
		}
		else {
			Presult.pop_back();
			count--;
		}
	}
	for (int i = 0; i < size; i++) {
		if (Presult[i] == result[i]) {
			correct++;
		}
	}
	return correct / (float)count;
}

float Analysis::getCorrectDoublePercentage(std::vector<float> probs[3], std::vector<int> result)
{
	int size = result.size();
	int incorrect = 0;
	int wrong;
	int count = 0;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < 3; j++) {
			count = 0;
			for (int k = 0; k < 3; k++) {
				if (probs[j][i] > probs[k][i]) {
					count++;
				}
			}
			if (count == 0) {
				wrong = j;
			}
		}
		if (result[i] == wrong) {
			incorrect++;
		}
	}
	return 1.0f - (incorrect / (float)size);
}


float Analysis::getAveragePercentageToWin(std::vector<float> prob1, std::vector<float> prob2, std::vector<float> prob3, std::vector<int> result)
{
	std::vector<float> prob[3] = { prob1, prob2, prob3 };
	float sum = 0;
	int size = prob1.size();
	for (int i = 0; i < size; i++) {
		sum += prob[result[i]][i];
	}
	return sum / (float)size;
}

float Analysis::getAveragePercentageToWin(std::vector<float> prob[3], std::vector<int> result)
{
	float sum = 0;
	int size = prob[0].size();
	for (int i = 0; i < size; i++) {
		sum += prob[result[i]][i];
	}
	return sum / (float)size;

}

float Analysis::getAverageDeviationFromRealGoals(std::vector<float> xg1, std::vector<float> xg2, std::vector<int> goals1, std::vector<int> goals2)
{
	int size = xg1.size();
	float sum = 0;
	for (int i = 0; i < size; i++) {
		sum += xg1[i] - (float)goals1[i] + xg2[i] - (float)goals2[i];
	}
	return 0.5f * sum / (float)size;
}

float Analysis::getCorrectGoalsPercentage(std::vector<float> xg1, std::vector<float> xg2, std::vector<int> goals1, std::vector<int> goals2, int type, float threshold)
{
	int size = xg1.size();
	int sum = 0;
	int sumexact = 0;
	for (int i = 0; i < size; i++) {
		if (Calc::round(xg1[i], threshold) == goals1[i] && Calc::round(xg2[i], threshold) == goals2[i]) {
			sum += 2;
			sumexact++;
		}
		else if ((Calc::round(xg1[i], threshold) == goals1[i] || Calc::round(xg2[i], threshold) == goals2[i])) {
			sum++;
		}

	}
	if (type == 0) {
		return sum / (2.0f * (float)size);

	}
	else {
		return sumexact / (float)size;
	}
}

float Analysis::getCorrectFloorGoalsPercentage(std::vector<float> xg1, std::vector<float> xg2, std::vector<int> goals1, std::vector<int> goals2, int type)
{
	int size = xg1.size();
	int sum = 0;
	int sumexact = 0;
	for (int i = 0; i < size; i++) {
		if (floor(xg1[i]) == goals1[i] && floor(xg2[i]) == goals2[i]) {
			sum += 2;
			sumexact++;
		}
		else if ((floor(xg1[i]) == goals1[i] || floor(xg2[i]) == goals2[i])) {
			sum++;
		}
	}
	if (type == 0) {
		return sum / (2.0f * (float)size);

	}
	else {
		return sumexact / (float)size;
	}
}

float Analysis::getCorrectCeilingGoalsPercentage(std::vector<float> xg1, std::vector<float> xg2, std::vector<int> goals1, std::vector<int> goals2, int type)
{
	int size = xg1.size();
	int sum = 0;
	int sumexact = 0;
	for (int i = 0; i < size; i++) {
		if (ceil(xg1[i]) == goals1[i] && ceil(xg2[i]) == goals2[i]) {
			sum += 2;
			sumexact++;
		}
		else if ((ceil(xg1[i]) == goals1[i] || ceil(xg2[i]) == goals2[i])) {
			sum++;
		}

	}
	if (type == 0) {
		return sum / (2.0f * (float)size);

	}
	else {
		return sumexact / (float)size;
	}
}
