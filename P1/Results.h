#pragma once
#include <string>
#include <list>
#include <tuple>
#include <iostream>
#include <unordered_map>

class Results { // was taken from lab excersize 

public:
	enum ResultValue { hitEnemy, fellToHigh, finished, lose, noResult};

private:
	static constexpr int DEFAULT_ADDITIONAL_DATA = 0;
	std::string fileName;
	std::string resultsForPrinting;
	std::list<std::tuple<size_t, ResultValue, int>> results; // pair: iteration, result, additional data (e.g score)
	std::unordered_map<ResultValue, std::string> resultToString = {
		{ hitEnemy, "hitEnemy" },
		{ fellToHigh, "fellToHigh" },
		{ finished, "finished" },
		{ lose, "lose" },
		{ noResult, "noResult" }
	};

public:
	
	// Load results from a result file
	static Results load(const std::string& filename);
	
	// Init the list of the resuts
	void init() { results.clear(); }

	// Getters
	std::string& getFileName() { return fileName; }
	std::string getResultsForPrinting() const { return resultsForPrinting; }
	
	// Setters
	void setFileName(std::string& filename) { fileName = filename; }
	
	// Save the results to a result file by a given file name
	void saveResults(std::string& fileName) const;

	// Add a result to the results list
	void addResult(size_t iteration, ResultValue result, int additionalData = DEFAULT_ADDITIONAL_DATA) {
		results.push_back(std::make_tuple(iteration, result , additionalData));
	}

	// Pop the first result from the results list
	std::tuple<size_t, ResultValue, int> popResult();
	
	// Prepare the results for printing
	void prepareForPrinting();

	// Convert a result to a string
	std::string convertResultToString(Results::ResultValue resCode) { return resultToString.find(resCode)->second; }
};
