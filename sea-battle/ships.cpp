#include "ships.h"
#include <iostream>

const std::vector<int> Rules::shipTypes = { 5, 4, 4, 3, 3, 3, 2, 2, 2, 2, 1, 1, 1, 1, 1 };
const int Rules::gameFieldSize = 10;
Positions::Positions(const Positions& other)
{
	this->chromosome.resize(other.chromosome.size(), NULL);
	for (int i = 0; i < this->chromosome.size(); i++) {
		Gene* cpy = new SinglePosition();
		*cpy = *other.chromosome[i];
		this->chromosome[i] = cpy;
	}
	this->survivalRate = other.survivalRate;
}
void Positions::randomFillChromosome(){
	this->chromosome.resize(Rules::shipTypes.size(), NULL);
	for (int i = 0; i < chromosome.size(); i++) {
		chromosome[i] = new SinglePosition();
		chromosome[i]->randomGenerate();
	}
}

void Positions::setChromosome(std::vector<Gene*>& chromosome) {
	this->chromosome.resize(chromosome.size(), NULL);
	for (int i = 0; i < this->chromosome.size(); i++) {
		this->chromosome[i] = new SinglePosition(dynamic_cast<SinglePosition*>(chromosome[i]));
	}
}

void Positions::updateSurvivalRate()
{
	std::vector< std::vector<double> > matrix;
	fillMatrix(matrix);
	for (int gene_it = 0; gene_it < this->chromosome.size(); gene_it++) {
		SinglePosition* current = new SinglePosition;
		*current = *dynamic_cast<SinglePosition*>(this->chromosome[gene_it]);
		if (current == NULL) {
			std::cout << "\nfail dynamic cast\n";
			this->survivalRate = DBL_MAX;
			return;
		}
		for (int i = 0; i < Rules::shipTypes[gene_it]; i++) {
			if (current->isVertical == 0) { //horisontal
				matrix[current->y][current->x + i] += 1;
				matrix[current->y + 1][current->x + i] += 0.2;
				matrix[current->y - 1][current->x + i] += 0.2;
				if (i == 0) {
					matrix[current->y][current->x - 1] += 0.2;
					matrix[current->y - 1][current->x - 1] += 0.2;
					matrix[current->y + 1][current->x - 1] += 0.2;
				}
				if (i == Rules::shipTypes[gene_it] - 1) {
					matrix[current->y][current->x + i + 1] += 0.2;
					matrix[current->y - 1][current->x + i + 1] += 0.2;
					matrix[current->y + 1][current->x + i + 1] += 0.2;
				}
			}
			else { //vertical
				matrix[current->y + i][current->x] += 1;
				matrix[current->y + i][current->x + 1] += 0.2;
				matrix[current->y + i][current->x - 1] += 0.2;
				if (i == 0) {
					matrix[current->y - 1][current->x] += 0.2;
					matrix[current->y - 1][current->x + 1] += 0.2;
					matrix[current->y - 1][current->x - 1] += 0.2;
				}
				if (i == Rules::shipTypes[gene_it] - 1) {
					matrix[current->y + i + 1][current->x] += 0.2;
					matrix[current->y + i + 1][current->x + 1] += 0.2;
					matrix[current->y + i + 1][current->x - 1] += 0.2;
				}
			}
		}
		delete current;
	}
	double genSum = 0;
	for (auto row : matrix) {
		for (auto elem : row) {
			if (elem < 999 && elem > 1) {
				genSum += elem;
			}
			if (elem >= 1001) {
				genSum += elem;
			}
		}
	}
	this->survivalRate = genSum;
}

double Positions::getSurvivalRate() const
{
	return survivalRate;
}

Individual* Positions::getChild(Individual* other) {
	Individual* child = new Positions();
	std::vector<Gene*> childChromosome;
	int breakPoint = rand() % this->chromosome.size();
	bool from1 = rand() % 2;
	for (int i = 0; i < this->chromosome.size(); i++) {
		bool isMutation = getValueByProbability(mutationChance);
		if (isMutation) {
			childChromosome.push_back(new SinglePosition());
			childChromosome[childChromosome.size() - 1]->randomGenerate();
			continue;
		}
		if (from1 && i <= breakPoint || !from1 && i > breakPoint) {
			childChromosome.push_back(new SinglePosition());
			*childChromosome[childChromosome.size() - 1] = *dynamic_cast<SinglePosition*>(this->chromosome[i]);
		}
		else {
			childChromosome.push_back(new SinglePosition());
			*childChromosome[childChromosome.size() - 1] = *dynamic_cast<SinglePosition*>(other->getGene(i));
		}
	}
	child->setChromosome(childChromosome);
	for (int i = 0; i < childChromosome.size(); i++) {
		delete childChromosome[i];
	}
	return child;
}

Individual& Positions::operator=(const Individual& other)
{
	if (this == &other) return *this;
	this->chromosome.resize(other.getChromosomeLength(), NULL);
	for (int i = 0; i < this->chromosome.size(); i++) {
		Gene* cpy = new SinglePosition();
		*cpy = *other.getGene(i);
		this->chromosome[i] = cpy;
	}
	this->survivalRate = other.getSurvivalRate();
	return *this;
}

void Positions::fillMatrix(std::vector< std::vector<double> >& matrix) {
	matrix.resize(Rules::gameFieldSize + Rules::shipTypes[0] + 1);
	for (auto& row : matrix) {
		row.resize(Rules::gameFieldSize + Rules::shipTypes[0] + 1);
	}
	for (int i = 0; i < matrix.size(); i++) {
		for (int j = 0; j < matrix[i].size(); j++) {
			if (inSquare(i, j, 1, Rules::gameFieldSize)) {
				matrix[i][j] = 0;
				continue;
			}
			matrix[i][j] = 1000;
		}
	}
}

bool Positions::inSquare(int x, int y, int squareBegin, int squareEnd) {
	return x >= squareBegin && x <= squareEnd && y >= squareBegin && y <= squareEnd;
}



SinglePosition::SinglePosition(SinglePosition* other) {
	if (other == NULL) {
		std::cout << "\ntrying to create SinglePosition with null pointer in constructor args\n";
		x = 0;
		y = 0;
		isVertical = 0;
		return;
	}
	this->x = other->x;
	this->y = other->y;
	this->isVertical = other->isVertical;
}

SinglePosition::SinglePosition(const SinglePosition& other)
{
	this->x = other.x;
	this->y = other.y;
	this->isVertical = other.isVertical;
}

SinglePosition& SinglePosition::operator=(const SinglePosition& other)
{
	if (this == &other)
		return *this;
	this->x = other.x;
	this->y = other.y;
	this->isVertical = other.isVertical;
	// TODO: вставьте здесь оператор return
}

bool SinglePosition::operator==(SinglePosition& other) {
	return this->x == other.x && this->y == other.y && this->isVertical == other.isVertical;
}

bool SinglePosition::operator!=(SinglePosition& other) {
	return this->operator==(other) == false;
}

void SinglePosition::randomGenerate() {
	x = rand() % Rules::gameFieldSize + 1;
	y = rand() % Rules::gameFieldSize + 1;
	isVertical = rand() % 2;
}

Gene& SinglePosition::operator=(Gene& other)
{
	if (this == &other)
		return *this;
	SinglePosition* otherSP = dynamic_cast<SinglePosition*>(&other);
	if (otherSP == NULL) {
		std::cout << "\nfail cast to SinglePostion\n";
		return *this;
	}
	this->x = otherSP->x;
	this->y = otherSP->y;
	this->isVertical = otherSP->isVertical;
	return *this;

	// TODO: вставьте здесь оператор return
}

void Rules::printShips(Positions ships) {
	std::vector< std::vector<bool> > matrix;
	matrix.resize(gameFieldSize);
	for (auto& row : matrix) {
		row.resize(gameFieldSize, 0);
	}
	for (int gene_it = 0; gene_it < ships.getChromosomeLength(); gene_it++) {
		SinglePosition* current = new SinglePosition;
		*current = *dynamic_cast<SinglePosition*>(ships.getGene(gene_it));
		if (current == NULL) {
			std::cout << "\nfail dynamic cast\n";
			return;
		}
		for (int i = 0; i < Rules::shipTypes[gene_it]; i++) {
			if (current->isVertical == 0) { //horisontal
				matrix[current->y - 1][current->x + i - 1] = 1;
			}
			else { //vertical
				matrix[current->y + i - 1][current->x - 1] = 1;
			}
		}
		delete current;
	}
	for (int i = 0; i < matrix.size(); i++) {
		for (int j = 0; j < matrix[i].size(); j++) {
			std::cout << (matrix[i][j] == true ? "@" : "-") << " ";
		}
		std::cout << std::endl;
	}
}
