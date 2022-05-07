#pragma once
#include <iostream>
#include "genetic_lab.h"
#include "base_genetic_functions.h"


class Positions;

class Rules {
public:
	static const int gameFieldSize;

	static const std::vector<int> shipTypes;

	static void printShips(Positions ships);
};

class SinglePosition : public Gene {
public:
	int x = 0;
	int y = 0;
	bool isVertical = 0;

	SinglePosition() {}

	SinglePosition(SinglePosition* other);

	SinglePosition(const SinglePosition& other);

	SinglePosition& operator=(const SinglePosition& other);

	virtual bool operator==(SinglePosition& other);

	virtual bool operator!=(SinglePosition& other);

	virtual void randomGenerate();
	
	virtual Gene& operator=(Gene& other);

private:
};


class Positions : public Individual {
public:
	Positions() {
		this->chromosome.resize(Rules::shipTypes.size(), NULL);
	}

	Positions(const Positions& other);

	Individual& operator=(const Individual& other);

	virtual void randomFillChromosome() override;

	virtual void setChromosome(std::vector<Gene*>& chromosome);

	virtual void updateSurvivalRate() override;

	virtual double getSurvivalRate() const override;

	virtual Individual* getChild(Individual* other);

private:
	

	void fillMatrix(std::vector< std::vector<double> >& matrix);

	bool inSquare(int x, int y, int squareBegin, int squareEnd);
};

