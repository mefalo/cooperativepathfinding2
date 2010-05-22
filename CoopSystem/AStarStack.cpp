// AStarStack.cpp: implementation of the AStarStack class.
//
//////////////////////////////////////////////////////////////////////
#include "string.h"
#include "AStarNode.h"
#include "AStarStack.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

AStarStack::AStarStack()
{
	top = 0;
}

AStarStack::~AStarStack()
{

}

AStarNode* AStarStack::Pop() {
	top = top - 1;
	if(top < 0) {
		top = 0;
		return NULL;
	} else {
		return data[top];
	}
}

void AStarStack::Push(AStarNode *node) {
	data[top] = node;
	top++;
}

int AStarStack::IsEmpty() {
	if(top == 0) return true;
	else return false;
}

