#include <iostream>
#include <utility>
#include <algorithm>
#include <vector>
#include <set>
#include <tuple>
#include <stdlib.h>
#include <time.h>

std::string generateStart();

// worst-case: start = 806547231 if goal = 012345678
const std::string goal = "012345678";
const std::string start = generateStart();

std::string generateStart() {

    std::string randomStart;
    std::set<int> tiles;
    int tile;
    srand(time(NULL));

    while (randomStart.size() < 9) {
        tile = rand() % 9;
        if (tiles.insert(tile).second) {
            char a = tile + '0';
            randomStart.push_back(a);
        }
    }

    std::cout << "Start: " << randomStart << std::endl;
    return randomStart;
}

int calcHeuristicMisplaced(std::string& state) {
    int h = 0;

    for (int i = 0; i < state.size(); i++) {
        if (state[i] != goal[i]) h++;
    }

    return h;
}

int calcHeuristicManhattan(std::string& state) {
    int h = 0;
    const int n = state.size() / 3;
    int grid[3][3];
    int count = 0;
    int gi, gj;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            grid[i][j] = state[count] - '0';
            gi = (grid[i][j] - 1) / 3;
            gj = abs((grid[i][j] - 1) % 3); 

            if (grid[i][j] == 0) {
                gi = 2;
                gj = 2;
            }
            
            h += abs(i - gi) + abs(j - gj);
            count++;
        }
    }

    return h;
}

std::tuple<int, std::string, int, std::string, int, std::vector<std::string>> move(int ind, std::string state, std::string&& dir, int numOfMoves, std::vector<std::string> solution) {
    if (dir == "left") {
        std::swap(state[ind], state[ind - 1]);
        ind--;
    }

    if (dir == "right") {
        std::swap(state[ind], state[ind + 1]);
        ind++;
    }

    if (dir == "up") {
        std::swap(state[ind], state[ind - 3]);
        ind -= 3;
    }

    if (dir == "down") {
        std::swap(state[ind], state[ind + 3]);
        ind += 3;
    }
    solution.push_back(dir);

    return std::make_tuple(calcHeuristicMisplaced(state) + numOfMoves, state, ind, dir, numOfMoves, solution);
}

void considerMove(int ind, std::string state, int numOfMoves, std::vector<std::string> solution, std::vector<std::tuple<int, std::string, int, std::string, int, std::vector<std::string>>>& openVec) {
    switch (ind)
    {
    case 0:
        openVec.push_back(move(ind, state, "right", numOfMoves, solution));
        openVec.push_back(move(ind, state, "down", numOfMoves, solution));
        break;
    case 1:
        openVec.push_back(move(ind, state, "left", numOfMoves, solution));
        openVec.push_back(move(ind, state, "right", numOfMoves, solution));
        openVec.push_back(move(ind, state, "down", numOfMoves, solution));
        break;
    case 2:
        openVec.push_back(move(ind, state, "left", numOfMoves, solution));
        openVec.push_back(move(ind, state, "down", numOfMoves, solution));
        break;
    case 3:
        openVec.push_back(move(ind, state, "right", numOfMoves, solution));
        openVec.push_back(move(ind, state, "up", numOfMoves, solution));
        openVec.push_back(move(ind, state, "down", numOfMoves, solution));
        break;
    case 4:
        openVec.push_back(move(ind, state, "left", numOfMoves, solution));
        openVec.push_back(move(ind, state, "right", numOfMoves, solution));
        openVec.push_back(move(ind, state, "up", numOfMoves, solution));
        openVec.push_back(move(ind, state, "down", numOfMoves, solution));
        break;
    case 5:
        openVec.push_back(move(ind, state, "left", numOfMoves, solution));
        openVec.push_back(move(ind, state, "up", numOfMoves, solution));
        openVec.push_back(move(ind, state, "down", numOfMoves, solution));
        break;
    case 6:
        openVec.push_back(move(ind, state, "right", numOfMoves, solution));
        openVec.push_back(move(ind, state, "up", numOfMoves, solution));
        break;
    case 7:
        openVec.push_back(move(ind, state, "left", numOfMoves, solution));
        openVec.push_back(move(ind, state, "right", numOfMoves, solution));
        openVec.push_back(move(ind, state, "up", numOfMoves, solution));
        break;
    case 8:
        openVec.push_back(move(ind, state, "left", numOfMoves, solution));
        openVec.push_back(move(ind, state, "up", numOfMoves, solution));
        break;
    default:
        break;
    }
}


int main()
{
    std::set<std::string> closedSet;
    std::set<std::tuple<int, std::string, int, std::string, int, std::vector<std::string>>> possibleSet;
    std::vector<std::tuple<int, std::string, int, std::string, int, std::vector<std::string>>> openVec;
    std::string state = start;
    
    int ind = state.find("0");
    possibleSet.insert({ 0, state, ind, "start", 0, {} });
    int numOfMoves = 0;
    std::vector<std::string> solution;

    while (true) {
        if (possibleSet.empty()) {
            std::cout << "No solution" << std::endl;
            return 1;
        }

        auto node = *possibleSet.begin();
        possibleSet.erase(possibleSet.begin()); // O(1)
        closedSet.insert(std::get<1>(node)); // O(logN)

        state = std::get<1>(node);
        ind = std::get<2>(node);

        solution = std::get<5>(node); // O(N)
        numOfMoves = std::get<4>(node) + 1;

        if (std::get<1>(node) == goal) {
            std::cout << "Goal reached in " << std::get<4>(node) << " moves." << std::endl;
            for (int i = 0; i < std::get<5>(node).size(); i++) {
                std::cout << std::get<5>(node)[i] << " -> ";
            }
            std::cout << "GOAL" << std::endl;

            return 1;
        }

        considerMove(ind, state, numOfMoves, solution, openVec); //O(1)
        
        for (int i = 0; i < openVec.size(); i++) { // O(M) (M cant be bigger than 4)
            if (closedSet.find(std::get<1>(openVec[i])) == closedSet.end()) { // O(logN)
                possibleSet.insert(openVec[i]); // O(logN)
            }
        }
        openVec.clear(); // O(M) (M cant be bigger than 4)
    }
}
