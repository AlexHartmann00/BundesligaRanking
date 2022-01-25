#include "Calc.h"


float Calc::calc(float a, float b) {
    float temp1 = b - a;
    float temp2 = temp1 / 150.0;
    return 1.0f / (1.0f + pow(10.0f, temp2));
}

int Calc::sign(float a) {
    if (a >= 0) {
        return 1;
    }
    else {
        return -1;
    }
}

float Calc::Max(float a, float b)
{
    if (a > b) {
        return a;
    }
    else {
        return b;
    }
}

float Calc::abs(float a)
{
    return a * sign(a);
}

int Calc::round(float a, float threshold)
{
    int b = floor(a);
    float c = a - b;
    if (c >= threshold) {
        return b + 1;
    }
    else {
        return b;
    }
}

float Calc::clampRoot(float a) {
    if (abs(a) < 1.0f) {
        return a;
    }
    else {
        return pow(abs(a), 0.75f) * sign(a);
    }
}

float Calc::incrementAvg(float oldAverage, float increment, int newCount) {
    float oldProportion = 1.0f - 1.0f / (float)newCount;
    float tempOld = oldAverage * oldProportion;
    float tempNew = increment / (float)newCount;
    return tempOld + tempNew;
}

float Calc::xGfromProbability(float prob,float a, float k, float bias) {
    return a * expf(k * prob) + bias;
}

float Calc::ProbabilityfromxG(float xg,float a, float k, float bias) {
    return logf((xg-bias) / a) / k;
}

int Calc::getSortedArrayIndices(std::vector<float> data, float value) {
    int size = data.size();
    int greaterItems = 0;
    for (int j = 0; j < size; j++) {
        if (data[j] > value) {
            greaterItems++;
        }
    }
    return greaterItems;
}

int Calc::getSortedArrayIndices(std::vector<std::string> data,std::string value) {
    int size = data.size();
    int lesserItems = 0;
    for (int j = 0; j < size; j++) {
        if (data[j] < value) {
            lesserItems++;
        }
    }

   
    return lesserItems;
}

float Calc::getAveragefromHeader(std::vector<float> values, int amount)
{
    int size = values.size();
    std::vector<int> index;
    std::vector<float>svalues;
    float sum = 0;
    for (int i = 0; i < size; i++) {
        index.push_back(getSortedArrayIndices(values, values[i]));
    }
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (index[j] == i) {
                svalues.push_back(values[j]);
            }
        }
    }
    for (int i = 0; i < amount; i++) {
        sum += svalues[i];
    }
    return sum / (float)amount;
}

float Calc::oddsfromProbability(float prob) {
    return 1 / (float)prob;
}

float Calc::Clamp(float value, float lower, float upper)
{
    if (value < lower) {
        return lower;
    }
    else if (value > upper) {
        return upper;
    }
    else {
        return value;
    }
}

float VectorMultiplicationForMatrixCalculations(std::vector<float> a, std::vector<float> b) {
    float sum = 0;
    for (int i = 0; i < a.size(); i++) {
        sum += a[i] * b[i];
    }
    return sum;
}

float S = 200.0f, a = 1.17f, n = 10.0f, m = 1.35f, k = 0.0019f;

float Calc::wonPossessions(float X) {
    float x = logf(X);
    return (a*S)/(a + (S-a) * expf(-S * k * n * (x + m)));
}

float Calc::lostPossessions(float X) {
    float x = logf(X);
    return S - (a * S) / (a + (S - a) * expf(-S * k * n * (x + m)));
}

