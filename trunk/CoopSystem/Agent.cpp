#include "Agent.h"
#include "jCooperativeTable.h"
#include <sstream>

Agent::Agent()
{
	m_CurrentTime = 0;
	int r = (rand() % SPRITES);
	string rString;

	
	std::stringstream out;
	out << r;
	rString = out.str();

	string Name = "thing" + rString + ".bmp";

	char cName[25];
	
	strcpy(cName, Name.c_str());



	thing = new AWSprite(cName, 4);
	thing->set_transparent_colour(0, 0, 0);
	thing->set_auto_animate(300);


	//crumb
	string Crumb = "crumb" + rString + ".bmp";

	char cCrumb[25];

	strcpy(cCrumb, Crumb.c_str());

	breadcrumbs = new AWMagazine(200, cCrumb, 1);
	breadcrumbs->set_transparent_colour(255, 255, 255);

	counter = 0;
	sol = 0;
	m_Steps = 0;

	TARGETSTATE = jCoop.getRandomDest();
	STARTSTATE = jCoop.getRandomSpawn();
	//TARGETSTATE = jCoop.getTestDest();
	//STARTSTATE = jCoop.getTestSpawn();

	thing->set_world_position(STARTSTATE.x, STARTSTATE.y);
	
	jCoop.getGraphAtTime(m_CurrentTime)->myMap[STARTSTATE.y][STARTSTATE.x] = 0;
	jCoop.getGraphAtTime(m_CurrentTime)->myMap[TARGETSTATE.y][TARGETSTATE.x] = 0;


	OPEN = AStarList(STARTSTATE, TARGETSTATE);
}

Agent::~Agent()
{
}

void Agent::DoAStar()
{
	AStarNode *current, *kid, *pato, *patc;
	int k;
	int i = 0;
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

		i++;

		current->Generate(m_CurrentTime+1);
		
		for(k = 0; k < 8; k++) {
			kid = current->kids[k];
			if(kid == NULL)
			{
				continue;
			}
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
		m_CurrentTime++;
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
		if(counter % 20 == 0) {
			sol++;
			AStarNode *node;
			if(sol < SOLUTIONSTEPS)
			if(sol <= m_Steps) {
				node = solution[sol];
				if(node != NULL) {
					thing->set_world_position(30*node->loc.x, 30*node->loc.y);
					// Show the crumb for this square
					AWBullet *crumb = breadcrumbs->allocate_a_bullet();
					jCoop.AddPath(sol, node->loc);
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
	AStarNode *CountNode;
	AStarStack s;
	int totalCost = 0;
	int TotalNodes = 0;

	CountNode = goal;
	node = goal;
	s.Push(node);

	while(CountNode->parent != NULL)
	{
		TotalNodes++;
		CountNode = CountNode->parent;
	}

	if(node !=NULL)
	{
	int i=0;
	while(node->parent != NULL) 
	{
		s.Push(node->parent);

		jCoop.ReserveNode(TotalNodes, node->loc);
		jCoop.ReserveNode(TotalNodes, node->parent->loc);

		jCoop.ReserveNode(TotalNodes, node->loc);
		jCoop.ReserveNode(TotalNodes, node->parent->loc);

		node = node->parent;
		TotalNodes--;
	}

	while(!s.IsEmpty()) 
	{
		node = s.Pop();
		totalCost += jCoop.getGraphAtTime(m_CurrentTime)->myMap[node->loc.y][node->loc.x];
		solution[i] = node;
		i++;
		if(i >= SOLUTIONSTEPS-1) break;
	}

	for(int i = 0; i < jCoop.getTimeDistance(); i++)
		jCoop.ReserveNode(i, node->loc);

		solution[i] = NULL;
		cout << "Finished linearising, there are " << i << " entries." << endl;
		cout << "The total cost was: " << totalCost << endl << endl;
	}
}