// wumpsim.cpp
//
// Main Wumpus Simulator procedure.

#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <iostream>
#include "Percept.h"
#include "Action.h"
#include "WumpusWorld.h"
#include "Agent.h"
#include "wumpsim.h"

using namespace std;

int main (int argc, char *argv[])
{
	int worldSize = 5;
	int numTrials = 1;
	int numTries = 1;
	unsigned seed;
	char* worldFile;
	bool seedSet = false;
	bool worldSet = false;//true;
	worldFile = "testworld.txt";

	// Set random number generator seed
	if (! seedSet)
	{
		seed = (unsigned) time (0);
	}
	srand (seed);

	// Print welcome
	cout << "Welcome to the Wumpus World Simulator v";
        cout << WUMPSIM_VERSION << ".  Happy hunting!" << endl << endl;

	// Run trials
	WumpusWorld* wumpusWorld;
	Agent* agent;
	Percept percept;
	Action action;
	int score;
	int trialScore;
	int totalScore = 0;
	float averageScore;
	int numMoves;

	for (int trial = 1; trial <= numTrials; trial++)
	{
		if (worldSet)
		{
			wumpusWorld = new WumpusWorld (worldFile);
		} else {
			wumpusWorld = new WumpusWorld (worldSize);
		}
		//wumpusWorld->Write (".world");
		agent = new Agent ();
		trialScore = 0;
		for (int tries = 1; tries <= numTries; tries++)
		{
			wumpusWorld->Initialize();
			//agent->Initialize(wumpusWorld->currentState.worldSize, wumpusWorld->initialMap);
			agent->Initialize();
			numMoves = 0;
			cout << "Trial " << trial << ", Try " << tries << " begin" << endl << endl;
			while ((! wumpusWorld->GameOver()) && (numMoves < MAX_MOVES_PER_GAME))
			{
				wumpusWorld->Print();
				percept = wumpusWorld->GetPercept();
				action = agent->Process (percept);
				cout << "Action = ";
				PrintAction (action);
				cout << endl << endl;
				wumpusWorld->ExecuteAction (action);
				numMoves++;
			}
			score = wumpusWorld->GetScore();
			agent->GameOver (score);
			trialScore = trialScore + score;
			cout << "Trial " << trial << ", Try " << tries << " complete: Score = " << score << endl << endl;
		}
		delete agent;
		delete wumpusWorld;
		averageScore = ((float) trialScore) / ((float) numTries);
		cout << "Trial " << trial << " complete: Average score for trial = " << averageScore << endl << endl;
		totalScore = totalScore + trialScore;
	}
	averageScore = ((float) totalScore) / ((float) (numTrials * numTries));
	cout << "All trials completed: Average score for all trials = " << averageScore << endl;
	cout << "Thanks for playing!" << endl << endl;

	return 0;
}
