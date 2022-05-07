#pragma once
#include <iostream>;
static int getValueByProbability(double prob, int value = 1) {
	if (prob > 1 || prob < 0) {
		std::cout << "\nattention, probability isn`t around 0 and 1\n";
		return 0;
	}
	double r = (double)(rand() % 100) / 100.0;
	if (r <= prob)
		return value;
	return 0;
}