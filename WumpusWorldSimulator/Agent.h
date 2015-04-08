// Agent.h

#ifndef AGENT_H
#define AGENT_H

#include "Action.h"
#include "Percept.h"
#include <array>
#include <functional>
#include <utility>
#include <queue>
#include <vector>

typedef std::vector<std::vector<MapState>> MapData;

const long long upperBound = 2147483648i64;

enum mOrient { N, R, L, U };

class AstarAlgo{
private:
	struct Node{
		int x, y;
		int parentIndex, index;
		long long cost;
		Node() {
			x = -1;
			y = -1;
			parentIndex = -1;
			index = -1;
			cost = -1;
		}
		Node(const Node& obj) {
			x = obj.x;
			y = obj.y;
			parentIndex = obj.parentIndex;
			index = obj.index;
			cost = obj.cost;
		}
	};
public:
	bool answer;
	std::pair<int, int> Dest;
	std::function<int(const MapData&, int, int)> HeuristicFunc;
	std::vector<Node> Graph;
	std::vector<Node> Candidates;
	std::vector<std::vector<bool>> isAccessed;
	std::queue<Action> ActionQue;
	void Compute(const MapData& mapdata, std::pair<int, int> start, std::pair<int,int> target);
	void Compute(const MapData& mapdata, const Node& curNode, std::pair<int, int> target);
	void MakeAction(const MapData& mapdata);
	Action operator() (const MapData& mapdata,
		Percept& percept);
};

namespace Heuristic {
	int Zero(const MapData& mapdata, int x, int y);
	int Distance(const MapData& mapdata, int x, int y);
}

class Agent {	
private:
	AstarAlgo Algorithm;
	MapData mapdata;
public:
	Agent ();
	~Agent ();
	void Initialize ();
	void Initialize(int worldSize, MapState* map);
	Action Process (Percept& percept);
	void GameOver (int score);
};
#endif // AGENT_H
