// Agent.h

#ifndef AGENT_H
#define AGENT_H

#include "Action.h"
#include "Percept.h"
#include <array>
#include <vector>
#include <functional>

struct Coord2 {
	int x, y;
	Coord2();
	Coord2(int ax, int ay);
	Coord2(const Coord2& obj);
};

typedef std::vector<Coord2> _coordlist;

class Knowledge {
private:
	typedef enum {
		safe = 0,
		unsate = 1,
		unknown = 2
	} _safety;

	struct Node {
		Coord2 coord;
		_safety safety;
		Node();
		Node(int ax, int ay);
		Node(const Coord2& ac, const _safety& as);
		Node(const Node& obj);
	};

	typedef std::vector<Node>  _mapcol;
	typedef std::vector<_mapcol> _mapdata;
	_mapdata mapData;
public:
	Knowledge();
	Knowledge(int size);
	void MakeSafe(const Coord2& coord);
	_coordlist FindAllSafeCoord();
};

class Agent {	
private:
	Knowledge KB;
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
