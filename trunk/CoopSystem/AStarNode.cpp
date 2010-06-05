// AStarNode.cpp: implementation of the AStarNode class.
//
//////////////////////////////////////////////////////////////////////
#include <iostream>
#include "math.h"
#include "AStarNode.h"
#include "AStarStack.h"
#include "string.h"
#include "data.h"
#include "jCooperativeTable.h"


using namespace std;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

AStarNode::AStarNode()
{
}

void AStarNode::setTarget(locstruct start)
{
TARGETSTATE = start;
}
AStarNode::~AStarNode()
{

}

double AStarNode::f() {
	return gsteps + hestimate;
}

void AStarNode::Generate(int currentTime) {
	int i;
	locstruct newloc;
	this->setTarget(TARGETSTATE);

	// Now decide what moves are possible, given the location of the gap
	for(i=0; i < 8; i++) {
		kids[i] = NULL;
		newloc.x = this->loc.x + generator[i].x;
		newloc.y = this->loc.y + generator[i].y;
			if(newloc.x < 0 || newloc.x >= MAXX || newloc.y < 0 || newloc.y >= MAXY) continue;
				if(jCoop.getGraphAtTime(0)->myMap[newloc.y][newloc.x] == 6)
				setCost(jCoop.getGraphAtTime(0)->myMap[newloc.y][newloc.x]);
				else
				setCost(jCoop.getGraphAtTime(currentTime)->myMap[newloc.y][newloc.x]);
					if(isWall()) continue;
						if(isBlocked()) continue;
						if(this->SameTiles(newloc)) continue;
									kids[i] = pool.Allocate();
									kids[i]->setTarget(TARGETSTATE);
									kids[i]->loc = newloc;
									kids[i]->parent = this;
									kids[i]->gsteps = gsteps + jCoop.getGraphAtTime(currentTime)->myMap[newloc.y][newloc.x];
									kids[i]->hestimate = kids[i]->Manhattan();
	}
	cout << endl << flush;
}


// VERY IMPORTANT!!
// This implementation is NOT admissible in the sense that A*
// requires. This means that it does not provide an underestimate
// of the distance to the solution, so it doesn't provide the optimal
// path. We could solve this problem by returning h/2.0
double AStarNode::Manhattan() {
	int m;
	double h;

	m = abs(this->loc.x - TARGETSTATE.x);
	m = m + abs(this->loc.y - TARGETSTATE.y);
	h = (float)m;
	return h;
}



double AStarNode::AirDistance() {
	int xdiff, ydiff;
	double diff;

	xdiff = (this->loc.x - TARGETSTATE.x) * (this->loc.x - TARGETSTATE.x);
	ydiff = (this->loc.y - TARGETSTATE.y) * (this->loc.y - TARGETSTATE.y);

	diff = sqrt((float)xdiff+(float)ydiff);

	return diff;
}




// asn is a node whose gsteps value turns out to be too high
//  we therefore want to change it, but all its kids and their
//  kids potentially have been affected by its incorrect value
//  so we go down making a stack of these to be corrected.
// We correct the g value and check to see if a child's g value
//  is higher than the parent's gvalue + 1. If so, we re-do it.
// REDUNDANT AT THE MOMENT
void AStarNode::Update() {
	AStarStack s;
	AStarNode *node, *kid;
	int k;

	s.Push(this);

	while(!s.IsEmpty()) {
		node = s.Pop();
		for(k=0; k < 8; k++) {
			if(node->kids[k] == NULL) continue;
			kid = node->kids[k];
			if(kid->gsteps > node->gsteps+1.0) kid->gsteps = node->gsteps+1.0;
			kid->parent = node;
			s.Push(kid);
		}
	}
}



int AStarNode::SameTiles(locstruct other) {
	if((this->loc.x != other.x) || (this->loc.y != other.y)) 
		return false;
	else return true;
}

void AStarNode::setCost(int cost)
{
	m_Cost = cost;
}
void AStarNode::Print() {
	cout << "\n";
	cout << "(" << this->loc.x << "," << this->loc.y << ") ";
	cout << "score is " << hestimate << endl;
	cout << flush;
}

bool AStarNode::isWall()
{
	if(m_Cost == 6)
		return true;

	return false;
}

bool AStarNode::isBlocked()
{
	if(m_Cost == 7)
		return true;

	return false;

}

void AStarNode::TracePath() {
	AStarNode *node;
	AStarStack s;

	node = this;
	s.Push(node);
	while(node->parent != NULL) {
		s.Push(node->parent);
		node = node->parent;
	}
	cout << "\n\nHERE IS THE PATH\n";
	while(!s.IsEmpty()) {
		node = s.Pop();
		node->Print();
	}
}

