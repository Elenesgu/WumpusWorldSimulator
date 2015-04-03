// Agent.cpp

#include <iostream>
#include <vector>
#include "Agent.h"

using namespace std;

Agent::Agent () {

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
			map++;
		}
	}
}


Action Agent::Process (Percept& percept) {
	Action action;
	AstarAlgo Algorithm;
	//TO DO
	action = Algorithm(percept,mapdata, Heuristic::Zero);
	return action;
}

void Agent::GameOver (int score) {

}


Action AstarAlgo::operator() (Percept& percepnt, const MapData& mapdata, std::function<int(const MapData&)> HeuristicFunc) {
	return TURNLEFT;
}

int Heuristic::Zero(const MapData& mapdata) {
	return 0;
}