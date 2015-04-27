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
	Action Process (Percept& percept);
	void GameOver (int score);
};

#endif // AGENT_H
