// Agent.h

#ifndef AGENT_H
#define AGENT_H

#include "Action.h"
#include "Percept.h"
#include "Orientation.h"
#include <array>
#include <deque>
#include <vector>
#include <functional>

struct Coord2 {
	int x, y;
	Coord2();
	Coord2(int ax, int ay);
	Coord2(const Coord2& obj);
	static bool CheckValid(const Coord2& obj);
};

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
	_mapdata mapData;
private:
	void UpdateBase(const Coord2& obj);
public:
	Knowledge();
	Knowledge(int size);
	void GetPercept(const Coord2& coord, Percept p);
};



class Agent {
private:
	bool ended;
	Knowledge KB;
	Coord2 curPosition;
	Orientation curOrient;

	std::deque<Action> actionQueue;

	Coord2 FindNextDest();
	_coordlist FindPath(const Coord2& dest);
	void MakeActionQueue(_coordlist& path);
public:
	const static int defaultSize = 5;

	Agent ();
	~Agent ();
	void Initialize ();
	void Initialize(int worldSize, MapState* map);
	Action Process (Percept& percept);
	void GameOver (int score);
};

#endif // AGENT_H
