#include "Agent.h"
#include "jCooperativeTable.h"

Agent::Agent()
{
	thing = new AWSprite("thing.bmp", 4);
	thing->set_transparent_colour(0, 0, 0);
	thing->set_auto_animate(300);

	breadcrumbs = new AWMagazine(200, "crumb.bmp", 1);
	breadcrumbs->set_transparent_colour(255, 255, 255);
}

Agent::~Agent()
{
}

void Agent::FindPath(locstruct to)
{

}

void Agent::setStartPos(locstruct STARTSTATE)
{
	thing->set_world_position(STARTSTATE.x, STARTSTATE.y);
}