// AStarPool.h: interface for the AStarPool class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ASTARPOOL_H__237CCA69_06E4_11D7_B15F_000476A2DA44__INCLUDED_)
#define AFX_ASTARPOOL_H__237CCA69_06E4_11D7_B15F_000476A2DA44__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class AStarPool  
{
public:
	AStarPool();
	virtual ~AStarPool();
	AStarNode p[100000];
	AStarNode *Allocate();
private:
	int nextfree;
};

#endif // !defined(AFX_ASTARPOOL_H__237CCA69_06E4_11D7_B15F_000476A2DA44__INCLUDED_)
