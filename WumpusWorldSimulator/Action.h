// Action.h

#ifndef ACTION_H
#define ACTION_H

#include <vector>

using namespace std;

enum Action {FORWARD, TURNLEFT, TURNRIGHT, GRAB, SHOOT, CLIMB};

enum MapState { NONE, PIT, WUMPUS, GOLD, PIT_GOLD, PIT_WUMPUS, WUMPUS_GOLD, PIT_WUMPUS_GOLD };


typedef vector<Action> ActionList;

void PrintAction (Action action);

#endif // ACTION_H
