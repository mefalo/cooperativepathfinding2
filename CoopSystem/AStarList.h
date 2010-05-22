// AStarList.h: interface for the AStarList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ASTARLIST_H__237CCA63_06E4_11D7_B15F_000476A2DA44__INCLUDED_)
#define AFX_ASTARLIST_H__237CCA63_06E4_11D7_B15F_000476A2DA44__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "defs.h"


class AStarList  
{
public:
	AStarList();
	AStarList(locstruct startingstate);
	virtual ~AStarList();
	bool IsEmpty();
	AStarNode *GetBestNode();
	void Add(AStarNode *asn);
	void Delete(AStarNode *asn);
	int Find(AStarNode *asn);
	AStarNode *PositionAlreadyThere(AStarNode *asn);
private:
	int howmany;

	AStarNode* n[100000];

};

#endif // !defined(AFX_ASTARLIST_H__237CCA63_06E4_11D7_B15F_000476A2DA44__INCLUDED_)
