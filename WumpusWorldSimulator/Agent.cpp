// Agent.cpp


#include <array>
#include <functional>
#include <iostream>
#include <memory>
#include <queue>
#include <stack>
#include <utility>
#include <vector>
#include "Agent.h"

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
	if (Algorithm.answer) {
		Algorithm.Compute(mapdata);
	}
	Algorithm.MakeAction();
}


Action Agent::Process (Percept& percept) {
	Action action;
	//TO DO
	int n;
	cin >> n;
	return action;
}

void Agent::GameOver (int score) {
}

#pragma region A*Algorithm

void AstarAlgo::Compute(const MapData& mapdata) {
	isAccessed = std::vector<std::vector<bool>>(mapdata.size(),
		std::vector<bool>(mapdata.size(), false));
	Node headNode;
	headNode.x = 0;
	headNode.y = 0;
	headNode.parentIndex = -1;
	headNode.index = 0;
	headNode.cost = 0;
	Graph.push_back(headNode);
	Compute(mapdata, Graph[0]);
}

//Recursive Call for A* algorithm
void AstarAlgo::Compute(const MapData& mapdata, const AstarAlgo::Node& curNode) {
	isAccessed[curNode.x][curNode.y] = true;
	Candidates.clear();
	if (mapdata[curNode.x][curNode.y] == GOLD) {
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
	Compute(mapdata, *minNode);
}

void AstarAlgo::MakeAction() {
	if (!answer) {
		ActionQue.push(CLIMB);
		return;
	}
	std::stack<Node> Path;
	Node cur = Graph.back();
	while (cur.parentIndex != -1) {
		Path.push(cur);
		cur = Graph[cur.parentIndex];
	}
	Graph.clear();

}

Action AstarAlgo::operator() (const MapData& mapdata
	, std::function<int(const MapData&)> HeuristicFunc) {


	return TURNLEFT;
}

int Heuristic::Zero(const MapData& mapdata, int x, int y) {
	return 0;
}

#pragma endregion