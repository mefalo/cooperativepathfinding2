#include "Agent.h"
#include "jCooperativeTable.h"

Agent::Agent()
{
	thing = new AWSprite("thing.bmp", 4);
	thing->set_transparent_colour(0, 0, 0);
	thing->set_auto_animate(300);

	breadcrumbs = new AWMagazine(200, "crumb.bmp", 1);
	breadcrumbs->set_transparent_colour(255, 255, 255);

	counter = 0;
	sol = 0;
	m_Steps = 0;

	TARGETSTATE = jCoop.getRandomSpawn();
	STARTSTATE = jCoop.getRandomDest();

	thing->set_world_position(STARTSTATE.x, STARTSTATE.y);
	
	jCoop.getGraphAtTime(0)->myMap[STARTSTATE.x][STARTSTATE.y] = 0;
	jCoop.getGraphAtTime(0)->myMap[TARGETSTATE.x][TARGETSTATE.y] = 0;


	OPEN = AStarList(STARTSTATE, TARGETSTATE);
}

Agent::~Agent()
{
}

void Agent::DoAStar()
{
	AStarNode *current, *kid, *pato, *patc;
	int k;
	int iterations = 0;

	while(OPEN.IsEmpty() == false) {
		current = OPEN.GetBestNode();
		current->setTarget(TARGETSTATE);
		iterations++;
		current->Print();
		if(current->SameTiles(TARGETSTATE)) {
			cout << "\nDONE! It took " << iterations << " iterations\n";
			goal = current;
			return;
		}

		current->Generate();

		for(k = 0; k < 8; k++) {
			kid = current->kids[k];
			if(kid == NULL) continue;

			pato = OPEN.PositionAlreadyThere(kid);
			patc = CLOSED.PositionAlreadyThere(kid);
			if(pato != NULL) {
			pato->setTarget(TARGETSTATE);
				if(pato->gsteps <= kid->gsteps) {
					current->kids[k] = NULL;
					continue;
				} else {
					OPEN.Delete(pato);
				}
			} else if(patc != NULL) {
				patc->setTarget(TARGETSTATE);
				if(patc->gsteps <= kid->gsteps) {
					current->kids[k] = NULL;
					continue;
				} else {
					patc->Update();
					CLOSED.Delete(patc);
				}
			}
			OPEN.Add(kid);
		}
		// Add current to CLOSED list
		CLOSED.Add(current);
	}
	goal = NULL;
	return;
}

void Agent::setStartPos()
{
}

void Agent::resetCrumbs()
{
			Uint32 counter = 0, sol = 0;

			sol = 0;
			counter = 0;
			// Clear the breadcrumbs
			unsigned int bc;
			for(bc = 0; bc < breadcrumbs->size(); bc++) {
				breadcrumbs->get(bc)->inUse = false;
			}
			thing->set_world_position(STARTSTATE.x, STARTSTATE.y);
}

void Agent::TracePath()
{
	AStarNode *node;
	AStarStack s;

	node = goal;
	s.Push(node);
	while(node->parent != NULL) {
		s.Push(node->parent);
		node = node->parent;
		m_Steps++;
	}
	cout << "\n\nHERE IS THE PATH\n";
	while(!s.IsEmpty()) {
		node = s.Pop();
		node->Print();
	}
}
void Agent::Walk()
{
		counter++;
		if(counter % 30 == 0) {
			sol++;
			AStarNode *node;
			if(sol < SOLUTIONSTEPS)
			if(sol <= m_Steps) {
				node = solution[sol];
				if(node != NULL) {
					thing->set_world_position(30*node->loc.x, 30*node->loc.y);
					// Show the crumb for this square
					AWBullet *crumb = breadcrumbs->allocate_a_bullet();
					if(crumb != NULL) crumb->set_world_position(30*node->loc.x, 30*node->loc.y);
				}
			}
		}

		breadcrumbs->update_everything();
		thing->update_everything();
}
void Agent::LinearisePath()
{
	AStarNode *node;
	AStarStack s;
	int totalCost = 0;
	
	node = goal;
	s.Push(node);
	while(node->parent != NULL) 
	{
		s.Push(node->parent);
		node = node->parent;
	}
	int i=0;
	while(!s.IsEmpty()) {
		node = s.Pop();
		totalCost += jCoop.getGraphAtTime(0)->myMap[node->loc.y][node->loc.x];
		solution[i] = node;
		i++;
		if(i >= SOLUTIONSTEPS-1) break;
	}
	solution[i] = NULL;
	cout << "Finished linearising, there are " << i << " entries." << endl;
	cout << "The total cost was: " << totalCost << endl << endl;

}