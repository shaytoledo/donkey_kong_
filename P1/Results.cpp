#include <fstream>

#include "Results.h"

Results Results::load(const std::string& filename) {
	std::ifstream results_file(filename);
	Results results;
	size_t size;
	results_file >> size;
	while (!results_file.eof() && size-- != 0) {
		size_t iteration;
		int result;
		int additionalData;
		results_file >> iteration >> result >> additionalData;
		results.addResult(iteration, static_cast<ResultValue>(result), additionalData);
	}
	return results;
}

std::tuple<size_t, Results::ResultValue, int> Results::popResult() {
	if (results.empty()) return std::make_tuple(0, Results::noResult, DEFAULT_ADDITIONAL_DATA);
	auto result = results.front();
	results.pop_front();
	return result;
}

void Results::saveResults(std::string& fileName) const {
	std::ofstream results_file(fileName);
	results_file << results.size();
	for (const auto& result : results) {
		results_file << '\n' << std::get<0>(result) << ' ' << std::get<1>(result) << ' ' << std::get<2>(result);
	}
	results_file.close();
}

void Results::prepareForPrinting() {
	
	resultsForPrinting += "Results for " + getFileName() + ":\n\n";
	for (const auto& result : results) {
		resultsForPrinting += std::string("\t\t  Iteration: ") +
			std::to_string(std::get<0>(result)) +
			std::string(" Result: ") + convertResultToString(std::get<1>(result)) +
			std::string(" Score: ") +
			std::to_string(std::get<2>(result)) + std::string("\n");
	}
}
