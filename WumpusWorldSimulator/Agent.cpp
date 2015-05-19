// Agent.cpp

#include <iostream>
#include <climits>
#include <array>
#include <list>
#include <queue>
#include <random>
#include <vector>
#include <set>
#include "Agent.h"

AstarAlgo::mOrient AstarAlgo::table[16] = { N, U, L, R,
U, N, R, L,
R, L, N, U,
L, R, U, N
};

Coord2::Coord2() : Coord2(0, 0) {
}
Coord2::Coord2(int ax, int ay) : x(ax), y(ay) {

}
Coord2::Coord2(const Coord2& obj) : Coord2(obj.x, obj.y) {

}

bool Coord2::CheckValid(const Coord2& obj) {
	return obj.x >= 0 && obj.y >= 0 && obj.x < Agent::defaultSize && obj.y < Agent::defaultSize;
}

bool operator==(const Coord2& f, const Coord2& s) {
	return f.x == s.x && f.y == f.y;
}

bool operator<(const Coord2& f, const Coord2& s) {
	return f.x + f.y < s.x + s.y;
}

#pragma region Knowledge
Knowledge::Node::Node() : Node(Coord2(-1, -1)){
	isVisited = false;
	pitfall = true;
	wumpus = true;
}
Knowledge::Node::Node(int ax, int ay) : Node(Coord2(ax, ay)) {

}

//Default Constructor
Knowledge::Node::Node(const Coord2& ac) : coord(ac) {

}

Knowledge::Node::Node(const Node& obj) : Node(obj.coord) {
	percept = obj.percept;
	isVisited = obj.isVisited;
	pitfall = obj.pitfall;
	wumpus = obj.wumpus;
}

Knowledge::Knowledge() : Knowledge(Agent::defaultSize) {
}
Knowledge::Knowledge(int size) {
	discoverd = 0;
	mapData = _mapdata(size, _mapcol(size));
	for (int x = 0; x < size; x++) {
		for (int y = 0; y < size; y++) {
			mapData[x][y].coord.x = x;
			mapData[x][y].coord.y = y;
		}
	}
}

void Knowledge::UpdateBase(const Coord2& obj) {
	Node& base = mapData[obj.x][obj.y];
	Coord2 nCoord;
	if (!base.percept.Stench) {
		for (int i = -1; i <= 1; i += 2) {
			nCoord.x = obj.x + i;
			nCoord.y = obj.y;
			if (Coord2::CheckValid(nCoord)) {
				mapData[nCoord.x][nCoord.y].wumpus = false;
			}
			nCoord.x = obj.x;
			nCoord.y = obj.y + i;
			if (Coord2::CheckValid(nCoord)) {
				mapData[nCoord.x][nCoord.y].wumpus = false;
			}
		}
	}
	if (!base.percept.Breeze) {
		for (int i = -1; i <= 1; i += 2) {
			nCoord.x = obj.x + i;
			nCoord.y = obj.y;
			if (Coord2::CheckValid(nCoord)) {
				mapData[nCoord.x][nCoord.y].pitfall = false;
			}
			nCoord.x = obj.x;
			nCoord.y = obj.y + i;
			if (Coord2::CheckValid(nCoord)) {
				mapData[nCoord.x][nCoord.y].pitfall = false;
			}
		}
	}
}


void Knowledge::GetPercept(const Coord2& coord, Percept p) {
	if (!mapData[coord.x][coord.y].isVisited) {
		discoverd++;
		mapData[coord.x][coord.y].percept = p;
		mapData[coord.x][coord.y].isVisited = true;
		UpdateBase(coord);
	}
}

#pragma endregion

#pragma region Agent
Agent::Agent () {
	Initialize();
}

Agent::~Agent () {

}

void Agent::Initialize() {
	curPosition = Coord2(0, 0);
	curOrient = RIGHT;
	ended = false;
	arrow = true;
	KB.mapData[0][0].wumpus = false;
	KB.mapData[0][0].pitfall = false;
}

void Agent::Initialize(int worldSize, MapState* map) {
	Initialize();
}

Coord2 Agent::FindNextDest() {
	for (const Knowledge::_mapcol& col : KB.mapData) {
		for (Knowledge::Node var : col) {
			if (!var.isVisited && !var.wumpus && !var.pitfall) {
				return var.coord;
			}
		}
	}
	return Coord2(-1, -1);
}

Coord2 Agent::FindNextWumpus() {
	std::vector<std::vector<Coord2>> records;
	std::set<Coord2> candidate, ncandidate;
	Coord2 nCoord;
	for (const auto& col : KB.mapData) {
		for (auto var : col) {
			if (var.percept.Stench) {
				std::vector<Coord2> localrecord;
				for (int i = -1; i <= 1; i += 2) {
					nCoord.x = var.coord.x + i;
					nCoord.y = var.coord.y;
					localrecord.push_back(nCoord);

					nCoord.x = var.coord.x;
					nCoord.y = var.coord.y + i;
					localrecord.push_back(nCoord);
				}
				records.push_back(localrecord);
			}
		}
	}
	if (records.size() <= 1) {
		return Coord2(-1, -1);
	}
	for (auto var : records[0]) {
		ncandidate.insert(var);
	}
	for (auto& localrecord : records) {
		for (auto var : ncandidate) {
			candidate.insert(var);
		}
		ncandidate.clear();
		for (auto var : localrecord) {
			for (auto origin : candidate) {
				if (var == origin) {
					ncandidate.insert(var);
				}
			}
		}
	}
	if (candidate.size() == 1) {
		return *candidate.begin();
	}
	else {
		return Coord2(-1, -1);
	}
}

Coord2 Agent::FindRandomDest(float T) {
	float temperature;
	temperature = std::exp(-(1.0/T));
	std::mt19937 gen(std::random_device{}());
	std::uniform_real_distribution<float> range(0.0, 1.0);
	if (range(gen) < temperature) {
		const Knowledge::_mapdata& record = KB.mapData;
		std::vector<std::pair<Coord2, int>> adjacentCoord;
		Coord2 nCoord;
		//Find all adjacent nodes;
		for (const auto& col : record) {
			for (const auto& var : col) {
				for (int i = -1; i <= 1; i += 2) {
					nCoord.x = var.coord.x;
					nCoord.y = var.coord.y + i;
					if (Coord2::CheckValid(nCoord) && !record[nCoord.x][nCoord.y].isVisited) {
						adjacentCoord.push_back(make_pair(nCoord, 0));
					}

					nCoord.x = var.coord.x + i;
					nCoord.y = var.coord.y;
					if (Coord2::CheckValid(nCoord) && !record[nCoord.x][nCoord.y].isVisited) {
						adjacentCoord.push_back(make_pair(nCoord, 0));
					}
				}
			}
		}
		//Calculate danger point of each adjacent node
		{


		}
		//Pick the lowest point's node
		int minValue = std::numeric_limits<int>::max();
		for (auto& var : adjacentCoord) {
			if (var.second < minValue) {
				minValue = var.second;
				nCoord = var.first;
			}
		}
		//return
		return nCoord;
	}
	else {
		return Coord2(-1, -1);
	}
}

Action Agent::Process (Percept& percept) {
	Action action;
	if (ended) {
		if (actionQueue.empty()){
			action = CLIMB;
			return action;
		}
		action = actionQueue.front();
		actionQueue.pop_front();
		return action;
	}
	else {
		if (percept.Glitter) {
			action = GRAB;
			ended = true;
			actionQueue = algoMethod(KB.mapData, curPosition, Coord2(0, 0), curOrient);
			return action;
		}
		else {
			KB.GetPercept(curPosition, percept);
		}

		if (percept.Scream) {
			actionQueue.push_front(FORWARD);
		}

		if (actionQueue.empty()){
			Coord2 NextDest = FindNextDest();
			if (!Coord2::CheckValid(NextDest)) {
				//Wumpus를 찾는다.
				if (arrow) {
					NextDest = FindNextWumpus();
				}

				//wumpus찾음
				if(Coord2::CheckValid(NextDest)) {
					wumpusPosition = NextDest;
					actionQueue = algoMethod(KB.mapData, curPosition, NextDest, curOrient);
					actionQueue.pop_back();
				} //Wumpus 못찾음 or 화살 이미 씀
				else {
					//랜덤 탐색
					NextDest = FindRandomDest(static_cast<float>(KB.discoverd));

					//위험한 곳 도전 실패
					if (!Coord2::CheckValid(NextDest)) {
						NextDest = Coord2(0, 0);
						ended = true;
						actionQueue = algoMethod(KB.mapData, curPosition, NextDest, curOrient);
						if (actionQueue.empty()){
							actionQueue.push_front(CLIMB);
						}
					} //도전성공
					else {
						actionQueue = algoMethod(KB.mapData, curPosition, NextDest, curOrient);
					}
				}
			}
			else {
				actionQueue = algoMethod(KB.mapData, curPosition, NextDest, curOrient);
			}
		}

		action = actionQueue.front();
		actionQueue.pop_front();
		UpdateState(action);
		return action;
	}
}

void Agent::UpdateState(Action action) {	
	switch (action) {
	case FORWARD:
		switch (curOrient) {
		case UP:
			curPosition.x += 1;
			break;
		case DOWN:
			curPosition.x -= 1;
			break;
		case LEFT:
			curPosition.y -= 1;
			break;
		case RIGHT:
			curPosition.y += 1;
			break;
		}
		break;
	case TURNLEFT:
		switch (curOrient) {
		case UP:
			curOrient = LEFT;
			break;
		case DOWN:
			curOrient = RIGHT;
			break;
		case LEFT:
			curOrient = DOWN;
			break;
		case RIGHT:
			curOrient = UP;
			break;
		}
		break;
	case TURNRIGHT:
		switch (curOrient) {
		case UP:
			curOrient = RIGHT;
			break;
		case DOWN:
			curOrient = LEFT;
			break;
		case LEFT:
			curOrient = UP;
			break;
		case RIGHT:
			curOrient = DOWN;
			break;
		}
		break;
	case SHOOT:
		arrow = false;
		break;
	default:
		break;
		//Not to do
	}
}

void Agent::GameOver (int score) {

}
#pragma endregion


#pragma region A*Algorithm

std::deque<Action> AstarAlgo::operator()(const AstarAlgo::MapData& mapdata,
	const Coord2& origin, const Coord2& dest, Orientation initori) {
	try {
		Compute(mapdata, origin, dest, initori);
	}
	catch (bool answer) {
		if (answer == false) {
			//길이 없다
		}
		else {
			throw std::exception("코딩 다시해");
		}
	}
	return MakeAction(mapdata);
}

void AstarAlgo::Compute(const MapData& mapdata, Coord2 start,
	Coord2 target, Orientation initorient) {

	isAccessed = std::vector<std::vector<bool>>(mapdata.size(),
		std::vector<bool>(mapdata.size(), false));
	ActionQue.clear();
	Graph.clear();
	Node headNode;
	headNode.x = start.x;
	headNode.y = start.y;
	headNode.parentIndex = -1;
	headNode.index = 0;
	headNode.cost = 0;
	headNode.or = initorient;
	Graph.push_back(headNode);
	Compute(mapdata, Graph[0], target);
}

//Recursive Call for A* algorithm
//Construct Tree using A* algorithm
//Tree is stored in AstarAlgo::Graph
void AstarAlgo::Compute(const MapData& mapdata, const AstarAlgo::Node& curNode, Coord2 target) {
	isAccessed[curNode.x][curNode.y] = true;
	Candidates.clear();
	//Chekc if it is final state.
	if (curNode.x == target.x && curNode.y == target.y) {
		return;
	}
	for_each(Graph.begin(), Graph.end(), [this, &mapdata](const Node& n) {
		Node newNode;
		for (int i = -1; i < 2; i += 2) {
			//Check adjacent node
			newNode.x = n.x + i;
			newNode.y = n.y;
			newNode.parentIndex = n.index;
			if (Coord2::CheckValid(Coord2(newNode.x, newNode.y))) {
				if (isAccessed[newNode.x][newNode.y] == false) {
					this->Candidates.push_back(newNode);
				}
			}
			newNode.x = n.x;
			newNode.y = n.y + i;
			newNode.parentIndex = n.index;
			if (Coord2::CheckValid(Coord2(newNode.x, newNode.y))) {
				if (isAccessed[newNode.x][newNode.y] == false) {
					this->Candidates.push_back(newNode);
				}
			}
		}
	});
	Node* minNode = nullptr;
	long long minCost = upperBound;
	for_each(Candidates.begin(), Candidates.end(), [this, &minNode, &mapdata, &minCost, &target](Node& n) {
		n.or = calcOrient(Graph[n.parentIndex], n);
		n.cost = Graph[n.parentIndex].cost + Heuristic::CostBase(mapdata, Graph, n, target);
		if (mapdata[n.x][n.y].pitfall || mapdata[n.x][n.y].wumpus) {
			n.cost += upperBound;
		}
		if (n.cost < minCost) {
			minCost = n.cost;
			minNode = &n;
		}
	});
	if (minNode == nullptr) {
		throw false;
	}
	minNode->index = Graph.size();
	Graph.push_back(*minNode);
	Compute(mapdata, *minNode, target);
}

std::deque<Action> AstarAlgo::MakeAction(const MapData& mapdata) {

	/*mOrient table[16] = {N, U, L, R,
	U, N, R, L,
	R, L, N, U,
	L, R, U, N
	};*/
	//Lambda for caculating action.
	auto calcAction = [this, &mapdata](const Node& source, const Node& target, Orientation& curOrient) {
		auto targetOrinet = calcOrient(source, target);
		switch (table[curOrient * 4 + targetOrinet]) {
		case N:
			break;
		case U:
			ActionQue.push_back(TURNLEFT); ActionQue.push_back(TURNLEFT); break;
		case L:
			ActionQue.push_back(TURNLEFT); break;
		case R:
			ActionQue.push_back(TURNRIGHT); break;
		}
		ActionQue.push_back(FORWARD);
		curOrient = targetOrinet;
	};
	NodeData Path;
	Node cur = Graph.back();
	while (cur.parentIndex != -1) {
		Path.push_back(cur);
		cur = Graph[cur.parentIndex];
	}
	Path.push_back(cur);
	for (int index = Path.size() - 2; index >= 0; index--) {
		calcAction(Path[index + 1], Path[index], cur.or);
	}
	return ActionQue;
}

Orientation AstarAlgo::calcOrient(const Node& source, const Node& target) {
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
}

AstarAlgo::AstarAlgo() {

}

int Heuristic::MDistance(const AstarAlgo::MapData& mapdata, const AstarAlgo::NodeData& graph,
	const AstarAlgo::Node& coord, const Coord2& dest) {
	return std::abs(dest.x - coord.x) + std::abs(dest.y - coord.y);
}

int Heuristic::CostBase(const AstarAlgo::MapData& mapdata, const AstarAlgo::NodeData& graph,
	const AstarAlgo::Node& coord, const Coord2& dest) {
	int result = 0;
	AstarAlgo::Node pNode = graph[coord.parentIndex];
	result += Heuristic::MDistance(mapdata, graph, coord, dest);
	Orientation newOr = AstarAlgo::calcOrient(pNode, coord);
	switch (AstarAlgo::table[pNode.or * 4 + newOr]) {
	case AstarAlgo::mOrient::N:
		break;
	case AstarAlgo::mOrient::U:
		result += 2;
		break;
	case AstarAlgo::mOrient::L:
	case AstarAlgo::mOrient::R:
		result += 1;
		break;
	}
	return result;
}

#pragma endregion