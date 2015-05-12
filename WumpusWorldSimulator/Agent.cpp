// Agent.cpp

#include <iostream>
#include <vector>
#include "Agent.h"

Coord2::Coord2() : Coord2(0, 0) {
}
Coord2::Coord2(int ax, int ay) : x(ax), y(ay) {

}
Coord2::Coord2(const Coord2& obj) : Coord2(obj.x, obj.y) {

}

#pragma region Knowledge
Knowledge::Node::Node() : Node(Coord2(0, 0), unknown){

}
Knowledge::Node::Node(int ax, int ay) : Node(Coord2(ax, ay), unknown) {

}
Knowledge::Node::Node(const Coord2& ac, const _safety& as) : coord(ac) {
	safety = as;
}
Knowledge::Node::Node(const Node& obj) : Node(obj.coord, obj.safety) {

}

Knowledge::Knowledge() : Knowledge(Agent::defaultSize) {
}
Knowledge::Knowledge(int size) {
	mapData = _mapdata(size, _mapcol(size));
	for (int x = 0; x < size; x++) {
		for (int y = 0; y < size; y++) {
			mapData[x][y].coord.x = x;
			mapData[x][y].coord.x = y;
		}
	}
}

void Knowledge::MakeSafe(const Coord2& coord) {
	mapData[coord.x][coord.y].safety = safe;
}

_coordlist Knowledge::FindAllSafeCoord() {
	_coordlist result;
	for (_mapcol col : mapData) {
		for (Node var : col) {
			if (var.safety == safe) {
				result.push_back(var.coord);
			}
		}
	}
}
#pragma endregion

#pragma region Agent
Agent::Agent () {

}

Agent::~Agent () {

}

void Agent::Initialize () {

}

void Agent::Initialize(int worldSize, MapState* map) {
	Initialize();
}


Action Agent::Process (Percept& percept) {
	Action action;
 	return action;
}

void Agent::GameOver (int score) {

}
#pragma endregion