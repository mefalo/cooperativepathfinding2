// AStarPool.cpp: implementation of the AStarPool class.
//
//////////////////////////////////////////////////////////////////////

#include "string.h"
#include "AStarNode.h"
#include "AStarPool.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

AStarPool::AStarPool()
{
	nextfree = 0;
}

AStarPool::~AStarPool()
{

}

AStarNode* AStarPool::Allocate() {
	int allocated = nextfree;
	int i;

	nextfree++;
	p[allocated].parent = NULL;
	for(i=0; i<8; i++) {
		p[allocated].kids[i] = NULL;
	}
	return &p[allocated];
}
