// Agent.h

#ifndef AGENT_H
#define AGENT_H

#include "Action.h"
#include "Percept.h"
#include "Orientation.h"
#include <array>
#include <deque>
#include <queue>
#include <vector>
#include <functional>

class AstarAlgo;

struct Coord2 {
	int x, y;
	Coord2();
	Coord2(int ax, int ay);
	Coord2(const Coord2& obj);
	static bool CheckValid(const Coord2& obj);
	
};

bool operator==(const Coord2& f, const Coord2& s);
bool operator<(const Coord2& f, const Coord2& s);
typedef std::vector<Coord2> _coordlist;

class Knowledge {
public:
	struct Node {
		Coord2 coord;
		bool pitfall;
		bool wumpus;
		Percept percept;
		bool isVisited;
		Node();
		Node(int ax, int ay);
		Node(const Coord2& ac);
		Node(const Node& obj);
	};

	typedef std::vector<Node>  _mapcol;
	typedef std::vector<_mapcol> _mapdata;
	size_t discoverd;
	_mapdata mapData;
private:
	void UpdateBase(const Coord2& obj);
public:
	Knowledge();
	Knowledge(int size);
	void GetPercept(const Coord2& coord, Percept p);
};

class AstarAlgo{
public:
	typedef Knowledge::_mapdata MapData;
	enum mOrient { N, R, L, U };
	const long long upperBound = 2147483647;
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
	Coord2 Origin;
	Coord2 Dest;


	//Variable for a*algorithm
	typedef std::vector<Node> NodeData;

	NodeData Graph;
	NodeData Candidates;
	std::vector<std::vector<bool>> isAccessed;
	//functions-------
	std::deque<Action> ActionQue;
	void Compute(const MapData& mapdata, Coord2 start, Coord2 target, Orientation initorient);
	void Compute(const MapData& mapdata, const Node& curNode, Coord2 target);
	std::deque<Action> MakeAction(const MapData& mapdata);
	std::deque<Action> operator() (const AstarAlgo::MapData& mapdata, const Coord2& origin, const Coord2& dest, Orientation initori);
	static Orientation calcOrient(const Node& source, const Node& target);
	AstarAlgo();
};

class Agent {
private:
	bool arrow;
	bool ended;
	bool kill;
	Knowledge KB;
	Coord2 curPosition;
	Coord2 wumpusPosition;
	Orientation curOrient;

	std::deque<Action> actionQueue;
	std::vector<Coord2> safeNodes;

	std::vector<Coord2> FindSafeNode();
	Coord2 FindNextDest(const std::vector<Coord2>& safeList);
	Coord2 FindNextWumpus();
	Coord2 FindRandomDest(float T);
	AstarAlgo algoMethod;
	void UpdateState(Action action);
	static void PrintCoordVector(const std::vector<Coord2>& list);
public:
	const static int defaultSize = 5;

	Agent();
	~Agent();
	void Initialize();
	void Initialize(int worldSize, MapState* map);
	Action Process(Percept& percept);
	void GameOver(int score);
};

namespace Heuristic {
	//int(const Mapdata&, pair<int, int>)
	int MDistance(const AstarAlgo::MapData& mapdata, const AstarAlgo::NodeData& graph, const AstarAlgo::Node& coord, const Coord2& dest);
	int CostBase(const AstarAlgo::MapData& mapdata, const AstarAlgo::NodeData& graph, const AstarAlgo::Node& coord, const Coord2& dest);
};

#endif // AGENT_H
