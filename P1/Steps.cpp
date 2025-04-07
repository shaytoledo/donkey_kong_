#include <fstream>

#include "Steps.h"

Steps Steps::load(const std::string& filename) { // was taken from lab excercise
	Steps steps;
	size_t size;
	
	std::ifstream steps_file(filename);
	steps_file >> steps.randomSeed;
	steps_file >> size;

	while (!steps_file.eof() && size-- != 0) {
		size_t iteration;
		char step;
		steps_file >> iteration >> step;
		steps.addStep(iteration, step);
	}
	steps_file.close();
	return steps;
}

void Steps::saveSteps(std::string& fileName) const { // was taken from lab excercise
	std::ofstream steps_file(fileName);
	steps_file << getRandomSeed() << '\n' << steps.size();
	for (const auto& step : steps) {
		steps_file << '\n' << step.first << ' ' << step.second;
	}
	steps_file.close();
}
