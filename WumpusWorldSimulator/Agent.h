// Agent.h

#ifndef AGENT_H
#define AGENT_H

#include "Action.h"
#include "Percept.h"
#include "Orientation.h"
#include <array>
#include <functional>
#include <utility>
#include <queue>
#include <vector>

typedef std::vector<std::vector<MapState>> MapData;

//upperBound is INT_MAX+1 (64bit integer data type must be needed)
const long long upperBound = 2147483648i64;

enum mOrient { N, R, L, U };

class AstarAlgo{
private:
	struct Node{
		int x;
		int y;
		int parentIndex, index;
		Orientation or;
		long long cost;
		Node() {
			x = -1;
			y = -1;
			parentIndex = -1;
			index = -1;
			cost = -1;
			or = RIGHT;
		}
		Node(const Node& obj) {
			x = obj.x;
			y = obj.y;
			parentIndex = obj.parentIndex;
			index = obj.index;
			cost = obj.cost;
			or = obj.or;
		}
	};
public:
	bool answer;
	std::pair<int, int> Dest;
	//For a*algorithm
	std::function<int(const MapData&, pair<int, int>, const pair<int, int>&)> HeuristicFunc;
	std::vector<Node> Graph;
	std::vector<Node> Candidates;
	std::vector<std::vector<bool>> isAccessed;
	//----------------
	std::queue<Action> ActionQue;
	Orientation calcOrient(const Node& source, const Node& target);
	void Compute(const MapData& mapdata, std::pair<int, int> start, std::pair<int,int> target, Orientation initorient);
	void Compute(const MapData& mapdata, const Node& curNode, std::pair<int, int> target);
	void MakeAction(const MapData& mapdata);
	Action operator() (const MapData& mapdata,
		Percept& percept);
};

namespace Heuristic {
	//int(const Mapdata&, pair<int, int>)
	int Zero(const MapData& mapdata, pair<int, int> coord, const pair<int, int>& dest);
	int MDistance(const MapData& mapdata, pair<int, int> coord, const pair<int, int>& dest);
	int CostBase(const MapData& mapdata, pair<int, int> coord, const pair<int, int>& dest);
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
