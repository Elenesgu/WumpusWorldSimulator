// Agent.cpp

#include <iostream>
#include "Agent.h"

using namespace std;

Agent::Agent ()
{

}

Agent::~Agent ()
{

}

void Agent::Initialize ()
{

}


void Agent::Initialize(int worldSize, MapState* map)
{
	int x;
	int y;
	
	cout << "Map check\n";
	cout << "0 = NONE\n" << "1 = PIT\n" << "2 = WUMPUS\n" << "3 = GOLD\n" << "4 = PIT_GOLD\n" << "5 = PIT_WUMPUS\n" << "6 = WUMPUS_GOLD\n";
	cout << "7 = PIT_WUMPUS_GOLD\n";
	for (y = worldSize-1; y >= 0; y--)
	{
		cout << y << ":	";
		for (x = 0; x < worldSize; x++)
		{
			cout << map[y*worldSize + x] << '\t';
		}
		cout << '\n';
	}
	
}


Action Agent::Process (Percept& percept)
{
	char c;
	Action action;
	bool validAction = false;

	while (! validAction)
	{
		validAction = true;
		cout << "Action? ";
		cin >> c;
		if (c == 'f') {
			action = FORWARD;
		} else if (c == 'l') {
			action = TURNLEFT;
		} else if (c == 'r') {
			action = TURNRIGHT;
		} else if (c == 'g') {
			action = GRAB;
		} else if (c == 's') {
			action = SHOOT;
		} else if (c == 'c') {
			action = CLIMB;
		} else {
			cout << "Huh?" << endl;
			validAction = false;
		}
	}
	return action;
}

void Agent::GameOver (int score)
{

}

