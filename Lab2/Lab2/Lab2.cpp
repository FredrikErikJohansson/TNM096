#include <iostream>
#include <unordered_set>
#include <vector>
#include <algorithm>
#include <string>
#include <time.h>

const std::vector<std::string> scheduleItems = { "MT101", "MT102", "MT103",
		"MT104", "MT105", "MT106",
		"MT107", "MT201", "MT202",
		"MT203", "MT204", "MT205",
		"MT206", "MT301", "MT302",
		"MT303", "MT304", "MT401",
		"MT402", "MT403", "MT501",
		"MT502", "aaaaa", "bbbbb" };


struct Csp {
	std::vector<std::string> schedule{};
	std::unordered_set<int> conflicts{};
	int score = 25;

	void initialize() { // Random initial scheduele
		std::unordered_set<int> indices;
		srand(time(NULL));
		schedule.clear();
		while (indices.size() < scheduleItems.size()) {
			int ind = rand() % scheduleItems.size();
			if (indices.insert(ind).second)
				schedule.push_back(scheduleItems[ind]);
		}
	}

	void calcConflicts() {
		conflicts.clear();
		for (size_t i = 0; i < schedule.size(); i+=3) {
			std::string class1 = schedule[i];
			std::string class2 = schedule[i + 1];
			std::string class3 = schedule[i + 2];
			if (class1[2] == class2[2]) {
				if (class1[2] != '5') { // Special case
					conflicts.insert(i);
					conflicts.insert(i + 1);
				}		
			}
			if (class1[2] == class3[2]) {
				if (class1[2] != '5') { // Special case
					conflicts.insert(i);
					conflicts.insert(i + 2);
				}
			}
			if (class2[2] == class3[2]) {
				if (class2[2] != '5') { // Special case
					conflicts.insert(i + 1);
					conflicts.insert(i + 2);
				}
			}
		}
	}

	int pickRandomConflict(int prev) {
		int ind = rand() % conflicts.size();
		while(ind == prev) // Never pick the same ind as prev
			ind = rand() % conflicts.size();
		return ind;
	}

	int findMinConflict(size_t var) {
		size_t prevNumConflicts = conflicts.size();
		std::vector<int> bestSwaps;
		for (size_t i = 0; i < schedule.size(); i++) {
			if (i == var) continue;
			std::swap(schedule[i], schedule[var]); // Try swap
			calcConflicts(); // Calculate conflicts

			if (conflicts.size() == prevNumConflicts) {
				bestSwaps.push_back(i);
			}
			else if (conflicts.size() < prevNumConflicts) {
				prevNumConflicts = conflicts.size();
				bestSwaps.clear();
				bestSwaps.push_back(i);
			}
			std::swap(schedule[var], schedule[i]); // Swap back
			calcConflicts();
		}
		return bestSwaps[rand() % bestSwaps.size()];
	}

	void print() {
		std::cout << "   TP51   SP34   K3" << std::endl;
		std::cout << "   ----   ----   ----" << std::endl;
		std::replace(schedule.begin(), schedule.end(), "aaaaa", "     "); // Make it pretty
		std::replace(schedule.begin(), schedule.end(), "bbbbb", "     ");

		int time = 9;
		for (size_t i = 0; i < schedule.size(); i+=3) {
			std::cout << schedule[i] << "   " << schedule[i + 1] << "   " << schedule[i + 2] << "   " << time << std::endl;
			time++;
			if (time == 13) time = 1;
		}

		std::replace(schedule.begin(), schedule.end(), "     ", "aaaaa"); // Make it functional again
		*std::find(schedule.begin(), schedule.end(), "aaaaa") = "bbbbb";
		std::cout << std::endl << "Number of conflicts: " << conflicts.size() << std::endl;
		if (score < 25) std::cout << "Unsatisfied preferences: " << score << std::endl;
		std::cout << std::endl;
	}
};

int calcScore(std::vector<std::string> schedule) {
	int score = 24;
	int blank1 = std::distance(schedule.begin(), std::find(schedule.begin(), schedule.end(), "aaaaa"));
	int blank2 = std::distance(schedule.begin(), std::find(schedule.begin(), schedule.end(), "bbbbb"));
	int MT501 = std::distance(schedule.begin(), std::find(schedule.begin(), schedule.end(), "MT501"));
	int MT502 = std::distance(schedule.begin(), std::find(schedule.begin(), schedule.end(), "MT502"));

	for (int i = 0; i < 3; i++) { // 9 AM
		if (blank1 == i) score--;
		if (blank2 == i) score--;
	}
	for (int i = 9; i < 12; i++) { // 12 AM
		if (blank1 == i) score--;
		if (blank2 == i) score--;
	}
	for (int i = 12; i < 15; i++) { // 1 PM
		if (MT501 == i) score--;
		if (MT502 == i) score--;
	}
	for (int i = 15; i < 18; i++) { // 2 PM
		if (MT501 == i) score--;
		if (MT502 == i) score--;
	}
	for (int i = 21; i < 24; i++) { // 4 PM
		if (blank1 == i) score--;
		if (blank2 == i) score--;
	}
	return score;
}


void solve(Csp& csp, const int max_steps) {
	int var = INT8_MIN;
	int value = INT8_MIN;
	for (int i = 0; i < max_steps; i++) {
		if (csp.conflicts.size() == 0) return; // Found solution
		var = csp.pickRandomConflict(var);
		value = csp.findMinConflict(var);
		std::swap(csp.schedule[var], csp.schedule[value]);
		csp.calcConflicts();
	}
	return; // Failed
}

void preferences(Csp& csp, const int max_steps, int max_tests) {
	Csp bestCsp = csp;
	int maxScore = INT8_MAX;
	int score = 24;
	for (int i = 0; i < max_tests; i++) {
		csp.initialize();
		solve(csp, max_steps);
		score = calcScore(csp.schedule);
		csp.score = score;
		if (score < maxScore) {
			maxScore = score;
			bestCsp = csp;
		}
	}
	csp = bestCsp;
}


int main() {
	Csp csp;
	csp.initialize();
	csp.calcConflicts();
	const int max_steps = 1000;
	const int max_tests = 1000;
	std::cout << "Initial schedule: " << std::endl;
	csp.print();

	//solve(csp, max_steps); // TASK 3
	preferences(csp, max_steps, max_tests); // TASK 4

	std::cout << "Final schedule: " << std::endl;
	csp.print();
	return 1;
}
