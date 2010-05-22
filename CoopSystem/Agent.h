/* class to allow for multiple Agents to pathfind */
#ifndef AGENT_H
#define AGENT_H
#include "defs.h"
#include "AWTileSet.h"
#include "AWTileMap.h"
#include "AWSprite.h"
#include "AWFont.cpp"
#include "AWMagazine.h"

class Agent
{
	Agent();
	~Agent();

	void setStartPos(locstruct STARTSTATE);
	void FindPath(locstruct to);

protected:
	AWSprite *thing;
	AWMagazine *breadcrumbs;
};

#endif