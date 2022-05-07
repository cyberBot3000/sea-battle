#pragma once
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>
#include <iostream>
#include "base_genetic_functions.h"


class Gene {
public:
	virtual bool operator==(Gene& other);
	virtual bool operator!=(Gene& other);
	virtual void randomGenerate() = 0;

	virtual Gene& operator=(Gene& other) = 0;
private:

};

class Individual {
public:

	~Individual();
	virtual Individual& operator=(const Individual& other) = 0;

	bool operator==(Individual& other);

	bool operator!=(Individual& other);

	bool operator<(Individual& other);

	bool operator<=(Individual& other);

	bool operator>(Individual& other);

	bool operator>=(Individual& other);

	virtual void randomFillChromosome() = 0;
	
	virtual void setChromosome(std::vector<Gene*>& chromosome) = 0;

	virtual Gene* getGene(int index) const;

	virtual int getChromosomeLength() const;

	static void setMutationChance(double chance);

	virtual void updateSurvivalRate() = 0;

	virtual double getSurvivalRate() const = 0;

	virtual Individual* getChild(Individual* other) = 0;

protected:
	static double mutationChance;
	std::vector<Gene*> chromosome;
	double survivalRate;
};


static bool indPtrComp(Individual* left, Individual* right) {
	return left->getSurvivalRate() < right->getSurvivalRate();
}


template<typename T>
class Incubator {
public:
	Incubator() {
		generationCount = 3;
		mutationChance = 0.1;
	}

	void nextGeneration() {
		double averageRate = 0;
		for (auto& ind : generation) {
			averageRate += ind->getSurvivalRate() / (double)generation.size();
		}
		//
		//
		// 
		// 
		//std::cout << averageRate << std::endl;
		//
		//
		// 
		//
		std::vector<Individual*> crossing;
		for (auto& ind : generation) {
			double fitness = averageRate / ind->getSurvivalRate();
			int crossColumn = ((int)fitness > 3 ? 3 : (int)fitness);
			bool makeAnotherCross = (bool)getValueByProbability(fitness - (int)fitness, 1);
			for (int i = 0; i < crossColumn + makeAnotherCross; i++) {
				crossing.push_back(new T());
				*crossing[crossing.size() - 1] = *ind;
			}
		}
		std::vector<Individual*> intermidiate;
		intermidiate.resize((crossing.size() * (crossing.size() - 1) / 2), NULL);
		int counter = 0;
		for (int i = 0; i < crossing.size() - 1; i++) {
			for (int j = i + 1; j < crossing.size(); j++) {
				intermidiate[counter] = crossing[i]->getChild(crossing[j]);
				intermidiate[counter]->updateSurvivalRate();
				counter++;
			}
		}
		std::sort(intermidiate.begin(), intermidiate.end(), indPtrComp);
		for (int i = 0; i < generationCount; i++) {
			delete generation[i];
			generation[i] = new T();
			*generation[i] = *intermidiate[i % intermidiate.size()];
		}
		for (int i = 0; i < intermidiate.size(); i++) {
			delete intermidiate[i];
		}
		for (int i = 0; i < crossing.size(); i++) {
			delete crossing[i];
		}
	}

	T getBestIndividual() {
		Individual::setMutationChance(mutationChance);
		generation.resize(generationCount, NULL);
		for (int i = 0; i < generation.size(); i++) {
			generation[i] = new T();
			generation[i]->randomFillChromosome();
			generation[i]->updateSurvivalRate();
		}
		if (best != NULL) {
			delete best;
		}
		best = new T();
		bool ready = false;
		while (!ready) {
			for (auto& ind : generation) {
				if (ind->getSurvivalRate() == 0.0) {
					*best = *ind;
					ready = true;
					break;
				}
			}
			if (ready) continue;
			this->nextGeneration();
		}
		for (int i = 0; i < generation.size(); i++) {
			delete generation[i];
		}

		return *dynamic_cast<T*>(best);
	}

	void setGenerationCount(int num) {
		if (num < 3) {
			return;
		}
		generationCount = num;
	}
	void setMutationChance(double chance) {
		if (chance < 0 || chance > 1) {
			return;
		}
		mutationChance = chance;
		Individual::setMutationChance(mutationChance);
	}
private:
	int generationCount;
	double mutationChance;
	std::vector<Individual*> generation;
	Individual* best = NULL;
};

