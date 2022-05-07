#include "genetic_lab.h"
double Individual::mutationChance = 0;

bool Gene::operator==(Gene& other) {
	return false;
}

bool Gene::operator!=(Gene& other) {
	return false;
}

Individual::~Individual() {
	for (int i = 0; i < this->chromosome.size(); i++) {
		delete chromosome[i];
	}
}

bool Individual::operator==(Individual& other) {
	for (int i = 0; i < this->chromosome.size(); i++) {
		if (this->chromosome[i] != other.chromosome[i])
			return false;
	}
	return true;
}

bool Individual::operator!=(Individual& other) {
	return this->operator==(other) == false;
}

bool Individual::operator<(Individual& other) {
	return this->survivalRate < other.survivalRate;
}

bool Individual::operator<=(Individual& other) {
	return this->survivalRate <= other.survivalRate;
}

bool Individual::operator>(Individual& other) {
	return this->survivalRate > other.survivalRate;
}

bool Individual::operator>=(Individual& other) {
	return this->survivalRate >= other.survivalRate;
}

Gene* Individual::getGene(int index) const {
	return chromosome[index];
}

int Individual::getChromosomeLength() const {
	return this->chromosome.size();
}

void Individual::setMutationChance(double chance) {
	mutationChance = chance;
}