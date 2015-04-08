// Agent.cpp


#include <array>
#include <exception>
#include <functional>
#include <iostream>
#include <memory>
#include <queue>
#include <stack>
#include <utility>
#include <vector>
#include "Agent.h"
#include "Orientation.h"

using namespace std;

Agent::Agent () {
	Algorithm.HeuristicFunc = Heuristic::Zero;
	Algorithm.answer = true;
}

Agent::~Agent () {

}

void Agent::Initialize () {

}


void Agent::Initialize(int worldSize, MapState* map) {
	mapdata = MapData(worldSize, std::vector<MapState>(worldSize));
	for (int row = 0; row < worldSize; row++) {
		for (int col = 0; col < worldSize; col++) {
			mapdata[row][col] = *map;
			if (*map == GOLD) {
				Algorithm.Dest.first = row;
				Algorithm.Dest.second = col;
			}
			if (*map == PIT_GOLD || *map == PIT_WUMPUS_GOLD) {
				Algorithm.answer = false;
			}
			map++;
		}
	}
	Algorithm.MakeAction(mapdata);
}


Action Agent::Process (Percept& percept) {
	Action action;
#ifdef _DEBUG
	int n;
	cin >> n;
#endif
	return Algorithm(mapdata, percept);
}

void Agent::GameOver (int score) {
}

#pragma region A*Algorithm

void AstarAlgo::Compute(const MapData& mapdata, std::pair<int, int> start, std::pair<int, int> target) {
	isAccessed = std::vector<std::vector<bool>>(mapdata.size(),
		std::vector<bool>(mapdata.size(), false));
	Graph.clear();
	Node headNode;
	headNode.x = start.first;
	headNode.y = start.second;
	headNode.parentIndex = -1;
	headNode.index = 0;
	headNode.cost = 0;
	Graph.push_back(headNode);
	Compute(mapdata, Graph[0], target);
}

//Recursive Call for A* algorithm
void AstarAlgo::Compute(const MapData& mapdata, const AstarAlgo::Node& curNode, std::pair<int, int> target) {
	isAccessed[curNode.x][curNode.y] = true;
	Candidates.clear();
	if (curNode.x == target.first && curNode.y == target.second) {
		return;
	}
	auto checkvalid = [this](Node& obj) {
		if (obj.x >= 0 && obj.y >= 0 &&
			obj.x < isAccessed.size() && obj.y < isAccessed.size()) {
			return true;
		}
		return false;
	};
	for_each(Graph.begin(), Graph.end(), [this, &checkvalid, &mapdata](const Node& n) {
		Node newNode;
		for (int i = -1; i < 2; i += 2) {
			//Check adjacent node
			newNode.x = n.x + i;
			newNode.y = n.y;
			newNode.parentIndex = n.index;
			if (checkvalid(newNode)) {
				if (isAccessed[newNode.x][newNode.y] == false) {
					this->Candidates.push_back(newNode);
				}
			}
			newNode.x = n.x;
			newNode.y = n.y + i;
			newNode.parentIndex = n.index;
			if (checkvalid(newNode)) {
				if (isAccessed[newNode.x][newNode.y] == false) {
					this->Candidates.push_back(newNode);
				}
			}
		}
	});
	Node* minNode = nullptr;
	long long minCost = upperBound;
	for_each(Candidates.begin(), Candidates.end(), [this, &minNode, &mapdata, &minCost](Node& n) {
		n.cost = Graph[n.parentIndex].cost + HeuristicFunc(mapdata, n.x, n.y);
		if (mapdata[n.x][n.y] == PIT || mapdata[n.x][n.y] == PIT_WUMPUS) {
			n.cost += upperBound;
		}
		if (n.cost < minCost) {
			minCost = n.cost;
			minNode = &n;
		}
	});
	if (minNode == nullptr) {
		answer = false;
		return;
	}
	minNode->index = Graph.size();
	Graph.push_back(*minNode);
	Compute(mapdata, *minNode, target);
}

void AstarAlgo::MakeAction(const MapData& mapdata) {
	mOrient table[16] = {N, U, L, R,
		U, N, R, L,
		R, L, N, U,
		L, R, U, N
	};
	auto calcAction = [this, &table, &mapdata](const Node& source, const Node& target, Orientation& curOrient) {
		auto calcOrient = [](const Node& source, const Node& target) {
			if (source.x == target.x) {
				if (target.y == source.y + 1) {
					return RIGHT;
				}
				else if (target.y == source.y - 1) {
					return LEFT;
				}
				else {
					throw std::invalid_argument("Not valid movement");
				}
			}
			else if (source.y == target.y){
				if (target.x == source.x + 1) {
					return UP;
				}
				else if (target.x == source.x - 1) {
					return DOWN;
				}
				else {
					throw std::invalid_argument("Not valid movemnet");
				}
			}
			else {
				throw std::invalid_argument("Not valid movement");
			}
		};
		auto targetOrinet = calcOrient(source, target);
		switch (table[curOrient * 4 + targetOrinet]) {
		case N:
			break;
		case U:
			ActionQue.push(TURNLEFT); ActionQue.push(TURNLEFT); break;
		case L:
			ActionQue.push(TURNLEFT); break;
		case R:
			ActionQue.push(TURNRIGHT); break;
		}
		curOrient = targetOrinet;
		if (mapdata[target.x][target.y] == WUMPUS || mapdata[target.x][target.y] == WUMPUS_GOLD) {
			ActionQue.push(SHOOT);
		}
		ActionQue.push(FORWARD);
	};

	if (!answer) {
		ActionQue.push(CLIMB);
		return;
	}
	Compute(mapdata, std::pair<int, int>(0,0),Dest);
	std::vector<Node> Path;
	Node cur = Graph.back();
	while (cur.parentIndex != -1) {
		Path.push_back(cur);
		cur = Graph[cur.parentIndex];
	}
	Path.push_back(cur);
	Orientation curOrient = RIGHT;
	for (int index = Path.size() - 2; index >= 0; index--) {
		calcAction(Path[index + 1], Path[index], curOrient);
	}
	ActionQue.push(GRAB);

	//Back route
	Compute(mapdata, Dest, std::pair<int, int>(0, 0));
	Path.clear();
	cur = Graph.back();
	while (cur.parentIndex != -1) {
		Path.push_back(cur);
		cur = Graph[cur.parentIndex];
	}
	Path.push_back(cur);
	for (int index = Path.size() - 2; index >= 0; index--) {
		calcAction(Path[index + 1], Path[index], curOrient);
	}
	ActionQue.push(CLIMB);
}

Action AstarAlgo::operator() (const MapData& mapdata,
	Percept& percept) {
	Action action = ActionQue.front();
	ActionQue.pop();
	return action;
}

int Heuristic::Zero(const MapData& mapdata, int x, int y) {
	return 0;
}

#pragma endregion