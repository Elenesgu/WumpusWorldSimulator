// Agent.cpp

#include <iostream>
#include <queue>
#include <vector>
#include "Agent.h"

Coord2::Coord2() : Coord2(0, 0) {
}
Coord2::Coord2(int ax, int ay) : x(ax), y(ay) {

}
Coord2::Coord2(const Coord2& obj) : Coord2(obj.x, obj.y) {

}

bool Coord2::CheckValid(const Coord2& obj) {
	return obj.x >= 0 && obj.y >= 0 && obj.x < Agent::defaultSize && obj.y < Agent::defaultSize;
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
		for (int x = -1; x <= 1; x += 2) {
			nCoord.x = obj.x + x;
			nCoord.y = obj.y;
			if (Coord2::CheckValid(nCoord)) {
				mapData[nCoord.x][nCoord.y].wumpus = false;
			}
		}
		for (int y = -1; y <= 1; y += 2){
			nCoord.x = obj.x;
			nCoord.y = obj.y + y;
			if (Coord2::CheckValid(nCoord)) {
				mapData[nCoord.x][nCoord.y].wumpus = false;
			}
		}
	}
	if (!base.percept.Breeze) {
		for (int x = -1; x <= 1; x += 2) {
			nCoord.x = obj.x + x;
			nCoord.y = obj.y;
			if (Coord2::CheckValid(nCoord)) {
				mapData[nCoord.x][nCoord.y].pitfall = false;
			}
		}
		for (int y = -1; y <= 1; y += 2){
			nCoord.x = obj.x;
			nCoord.y = obj.y + y;
			if (Coord2::CheckValid(nCoord)) {
				mapData[nCoord.x][nCoord.y].pitfall = false;
			}
		}
	}
}


void Knowledge::GetPercept(const Coord2& coord, Percept p) {
	if (!mapData[coord.x][coord.y].isVisited) {
		mapData[coord.x][coord.y].percept = p;
		mapData[coord.x][coord.y].isVisited = true;
		UpdateBase(coord);
	}
}

#pragma endregion

#pragma region Agent
Agent::Agent () {

}

Agent::~Agent () {

}

void Agent::Initialize() {
	curPosition = Coord2(0, 0);
	ended = false;
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
}

_coordlist Agent::FindPath(const Coord2& dest){
	Coord2 origin = curPosition;
	std::vector<std::pair<Coord2, Coord2>> Graph;
	std::queue<Coord2> nextQueue;
	nextQueue.push(origin);
	while (nextQueue.empty()) {

	}
	_coordlist path;
	return path;
}

void Agent::MakeActionQueue(_coordlist& path) {

}

Action Agent::Process (Percept& percept) {
	Action action;
	if (ended) {
		action = actionQueue.front();
		actionQueue.pop_front();
		return action;
	}
	else {
		if (percept.Glitter) {
			action = GRAB;
			ended = true;
			_coordlist path = FindPath(Coord2(0, 0));
			actionQueue.clear();
			MakeActionQueue(path);
		}
		else {
			KB.GetPercept(curPosition, percept);
		}

		if (actionQueue.empty()){
			Coord2 NextDest = FindNextDest();
			_coordlist path = FindPath(NextDest);
		}
		return action;
	}
}

void Agent::GameOver (int score) {

}
#pragma endregion