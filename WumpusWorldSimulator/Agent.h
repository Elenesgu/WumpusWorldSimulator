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
public:
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
	static mOrient table[16];
	bool answer;
	std::pair<int, int> Dest;
	//For a*algorithm
	typedef std::vector<Node> NodeData;
	std::function<int(const MapData&, const NodeData&, const Node&, const pair<int, int>&)> HeuristicFunc;
	NodeData Graph;
	NodeData Candidates;
	std::vector<std::vector<bool>> isAccessed;
	//----------------
	std::queue<Action> ActionQue;
	void Compute(const MapData& mapdata, std::pair<int, int> start, std::pair<int,int> target, Orientation initorient);
	void Compute(const MapData& mapdata, const Node& curNode, std::pair<int, int> target);
	void MakeAction(const MapData& mapdata);

	Action operator() (const MapData& mapdata,
		Percept& percept);
	static Orientation calcOrient(const Node& source, const Node& target);
};

namespace Heuristic {
	//int(const Mapdata&, pair<int, int>)
	static int Zero(const MapData& mapdata, const AstarAlgo::NodeData& graph, AstarAlgo::Node& coord, const pair<int, int>& dest);
	static int MDistance(const MapData& mapdata, const AstarAlgo::NodeData& graph, const AstarAlgo::Node& coord, const pair<int, int>& dest);
	static int CostBase(const MapData& mapdata, const AstarAlgo::NodeData& graph, const AstarAlgo::Node& coord, const pair<int, int>& dest);
};



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
