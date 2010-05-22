// AStarStack.h: interface for the AStarStack class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ASTARSTACK_H__46290521_0777_11D7_B160_000476A2DA44__INCLUDED_)
#define AFX_ASTARSTACK_H__46290521_0777_11D7_B160_000476A2DA44__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class AStarStack  
{
public:
	AStarStack();
	virtual ~AStarStack();
	AStarNode* Pop();
	void Push(AStarNode *node);
	int IsEmpty();
private:
	AStarNode* data[10000];
	int top;

};

#endif // !defined(AFX_ASTARSTACK_H__46290521_0777_11D7_B160_000476A2DA44__INCLUDED_)
