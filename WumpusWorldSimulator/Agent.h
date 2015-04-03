// Agent.h

#ifndef AGENT_H
#define AGENT_H

#include "Action.h"
#include "Percept.h"
#include <array>
#include <functional>

typedef std::vector<std::vector<MapState>> MapData;

class Agent {	
private:
	MapData mapdata;
public:
	Agent ();
	~Agent ();
	void Initialize ();
	void Initialize(int worldSize, MapState* map);
	Action Process (Percept& percept);
	void GameOver (int score);
};

struct AstarAlgo{
	Action operator() (Percept& percept, const MapData& mapdata, std::function<int(const MapData&)> HeuristicFunc);
};

namespace Heuristic {
	int Zero(const MapData& mapdata);
}

#endif // AGENT_H
