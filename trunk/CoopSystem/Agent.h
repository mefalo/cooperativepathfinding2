/* class to allow for multiple Agents to pathfind */
#ifndef AGENT_H
#define AGENT_H
#include <iostream>
#include "string.h"
#include "AStarNode.h"
#include "AStarPool.h"
#include "AStarList.h"
#include "AStarStack.h"
#include "defs.h"
#include "data.h"

#include "SDL.h"
#include "AWTileSet.h"
#include "AWTileMap.h"
#include "AWSprite.h"
#include "AWMagazine.h"
#include "Agent.h"

#include "jCooperativeTable.h"

using namespace std;

#define SOLUTIONSTEPS   (500)

class Agent
{
public:
	Agent();
	~Agent();

	void setStartPos();
	void DoAStar();
	void TracePath();
	void resetCrumbs();
	void Walk();
	void LinearisePath();
	AStarNode* getPathGoal()
	{
		return goal;
	}

	locstruct STARTSTATE;
	locstruct TARGETSTATE;

protected:
	int m_Sprite;
	int m_CurrentTime;
	int m_Steps;
	AStarList OPEN;
	AStarList CLOSED;
	AStarNode *solution[SOLUTIONSTEPS];
	Uint32 counter, sol;
	AStarNode *goal;
	AWSprite *thing;
	AWMagazine *breadcrumbs;
};

#endif