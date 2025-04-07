#pragma once
#include <string>
#include <list>

class Steps { // was taken from lab excercise
	std::string fileName;
	long randomSeed = 0;
	std::list<std::pair<size_t, char>> steps; // pair: iteration, step

public:
	
	// Init the list of the steps
	void init() { steps.clear(); }

	// Load the steps from a steps file by a given file name
	static Steps load(const std::string& filename);

	// Check if the steps list is empty
	bool isEmpty() const { return steps.empty(); }

	// Save the steps to a steps file by a given file name
	void saveSteps(std::string& fileName) const;

	// Set a new file name for the steps
	void setFileName(std::string& filename) { fileName = filename; }

	// Get the steps file name
	std::string getFileName() const { return fileName; }

	// Get the random seed
	long getRandomSeed() const {
		return randomSeed;
	}

	// Set the random seed
	void setRandomSeed(long seed) {
		randomSeed = seed;
	}

	// Add new steps to the steps list 
	void addStep(size_t iteration, char step) {
		steps.push_back({ iteration, step });
	}

	// Pop the first step from the steps list
	char popStep() {
		char step = steps.front().second;
		steps.pop_front();
		return step;
	}

	// Get the current iteration number
	size_t getCurrIteration() { return !isEmpty() ? steps.front().first : -1; } // -1 for empty


};
