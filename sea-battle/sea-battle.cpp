#include <iostream>
#include <algorithm>
#include <vector>
#include "genetic_lab.h"
#include "ships.h"


int main() {
	srand(time(0));

	Incubator<Positions> inc;
	inc.setGenerationCount(40);
	inc.setMutationChance(0.15);
	char again;
	do {
		system("cls");
		Rules::printShips(inc.getBestIndividual());
		std::cin >> again;
	} while (again == 'a');
	return 0;
}