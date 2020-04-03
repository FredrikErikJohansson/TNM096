#include <iostream>
#include <utility>
#include <algorithm>
#include <vector>
#include <set>
#include <tuple>

const std::string goal = "123456780";

int calcHeuristic(std::string& state) {
    int h = 0;

    for (int i = 0; i < state.size(); i++) {
        if (state[i] != goal[i])
            h++;
    }

    return h;
}

std::tuple<int, std::string, int, std::string, int> move(int ind, std::string state, std::string dir, int numOfMoves) {
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

    return std::make_tuple(calcHeuristic(state) + numOfMoves, state, ind, dir, numOfMoves);
}


int main()
{

    std::set<std::string> closedSet;
    std::vector<std::tuple<int, std::string, int, std::string, int>> openVec;
    std::vector<std::tuple<int, std::string, int, std::string, int>> possibleVec;
    std::vector<std::string> solution;
    std::string state = "721850436";
    closedSet.insert(state);
    int ind = state.find("0");
    int numOfMoves = 0;
    int lastNumOfMoves = numOfMoves;
    
    int count = 0;

    while (true) {

        //Consider every possible move
        switch (ind)
        {
        case 0:
            openVec.push_back(move(ind, state, "right", numOfMoves));
            openVec.push_back(move(ind, state, "down", numOfMoves));
            break;
        case 1:
            openVec.push_back(move(ind, state, "left", numOfMoves));
            openVec.push_back(move(ind, state, "right", numOfMoves));
            openVec.push_back(move(ind, state, "down", numOfMoves));
            break;
        case 2:
            openVec.push_back(move(ind, state, "left", numOfMoves));
            openVec.push_back(move(ind, state, "down", numOfMoves));
            break;
        case 3:
            openVec.push_back(move(ind, state, "right", numOfMoves));
            openVec.push_back(move(ind, state, "up", numOfMoves));
            openVec.push_back(move(ind, state, "down", numOfMoves));
            break;
        case 4:
            openVec.push_back(move(ind, state, "left", numOfMoves));
            openVec.push_back(move(ind, state, "right", numOfMoves));
            openVec.push_back(move(ind, state, "up", numOfMoves));
            openVec.push_back(move(ind, state, "down", numOfMoves));
            break;
        case 5:
            openVec.push_back(move(ind, state, "left", numOfMoves));
            openVec.push_back(move(ind, state, "up", numOfMoves));
            openVec.push_back(move(ind, state, "down", numOfMoves));
            break;
        case 6:
            openVec.push_back(move(ind, state, "right", numOfMoves));
            openVec.push_back(move(ind, state, "up", numOfMoves));
            break;
        case 7:
            openVec.push_back(move(ind, state, "left", numOfMoves));
            openVec.push_back(move(ind, state, "right", numOfMoves));
            openVec.push_back(move(ind, state, "up", numOfMoves));
            break;
        case 8:
            openVec.push_back(move(ind, state, "left", numOfMoves));
            openVec.push_back(move(ind, state, "up", numOfMoves));
            break;
        default:
            break;
        }

        for (int i = 0; i < openVec.size(); i++) {
            if (closedSet.find(std::get<1>(openVec[i])) == closedSet.end()) {
                possibleVec.push_back(openVec[i]);
            }
        }

        if (possibleVec.empty()) {
            std::cout << "No solution found!" << std::endl;
            return 1;
        }

        std::sort(possibleVec.rbegin(), possibleVec.rend());

        state = std::get<1>(possibleVec.back());
        ind = std::get<2>(possibleVec.back());
        closedSet.insert(state);       
         
        if (numOfMoves < std::get<4>(possibleVec.back())) {
            solution.clear();
        }
        solution.push_back(std::get<3>(possibleVec.back()));

        numOfMoves = std::get<4>(possibleVec.back());
        numOfMoves++;
        possibleVec.pop_back();
        //possibleVec.erase(possibleVec.end());
        openVec.clear();

        if (state == goal) {
            std::cout << "Reached goal!" << std::endl;

            for (int i = 0; i > solution.size(); i++) {
                std::cout << solution[i] << " ";
            }

            std::cout <<  std::endl << numOfMoves << std::endl;
            return 1;
        }

            //std::cout << closedSet.size() << std::endl;
        if(closedSet.size()%1000 == 0) std::cout << closedSet.size() << std::endl;


        /*if (lastNumOfMoves == numOfMoves) {
            std::cout << "No solution found!" << std::endl;
            return 1;
        }
        else {
            lastNumOfMoves = numOfMoves;
        }*/
    }
}
