#include <iostream>
#include <utility>
#include <algorithm>
#include <vector>
#include <set>
#include <tuple>

const std::string goal = "012345678";

int calcHeuristic(std::string& state) {
    int h = 0;

    for (int i = 0; i < state.size(); i++) {
        if (state[i] != goal[i])
            h++;
    }

    return h;
}

std::tuple<int, std::string, int, std::string, int, std::vector<std::string>> move(int ind, std::string state, std::string dir, int numOfMoves, std::vector<std::string> moves) {
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

    moves.push_back(dir);

    return std::make_tuple(calcHeuristic(state) + numOfMoves, state, ind, dir, numOfMoves, moves);
}


int main()
{

    std::set<std::string> closedSet;
    std::vector<std::tuple<int, std::string, int, std::string, int, std::vector<std::string>>> openVec;
    std::set<std::tuple<int, std::string, int, std::string, int, std::vector<std::string>>> possibleSet;
    std::string state = "806547231";
    
    int ind = state.find("0");
    possibleSet.insert({ 0, state, ind, "start", 0, {} });
    int numOfMoves = 0;
    int lastNumOfMoves = numOfMoves;
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

            std::cout << "goal" << std::endl;

            return 1;
        }

        //Consider every possible move
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
        
        
        for (int i = 0; i < openVec.size(); i++) { // O(M) (M cant be bigger than 4)
            if (closedSet.find(std::get<1>(openVec[i])) == closedSet.end()) { // O(logN)
                possibleSet.insert(openVec[i]); // O(logN)
            }
        }

        openVec.clear();
    }
}
