// AStarList.cpp: implementation of the AStarList class.
//
//////////////////////////////////////////////////////////////////////
#include <iostream>
#include "string.h"
#include "AStarNode.h"
#include "AStarList.h"
#include "AStarPool.h"
#include "data.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

using namespace std;


AStarList::AStarList()
{
	howmany = 0;
}

AStarList::AStarList(locstruct startingstate)
{
	n[0] = pool.Allocate();
	n[0]->loc = startingstate;
	n[0]->gsteps = 0;
	n[0]->hestimate = n[0]->Manhattan();

	howmany = 1;

}

AStarList::~AStarList()
{

}

bool AStarList::IsEmpty() {
	if(howmany == 0) return true;
	else return false;
}

// This finds the best node in the list and deletes it
//  The deletion is effected by reducing howmany by 1
AStarNode* AStarList::GetBestNode() {
	AStarNode *swap;
	int ENDOFLIST = howmany-1;
	int i;

	if(IsEmpty()) return NULL;
//	cout << endl << endl << "AVAILABLE: ";
	for(i=ENDOFLIST; i >= 0; i--) {
//		cout << n[i]->f() << " ";
		if(n[i]->f() < n[ENDOFLIST]->f()) {
			swap = n[ENDOFLIST];
			n[ENDOFLIST] = n[i];
			n[i] = swap;
		}
	}
//	cout << endl << "CHOSEN: "<< n[ENDOFLIST]->f() << endl << endl << flush;
	howmany--;

	
	return n[ENDOFLIST];
}

void AStarList::Add(AStarNode *asn) {
	n[howmany] = asn;
	howmany++;
}

int AStarList::Find(AStarNode *asn) {
	int i;
	for(i=0; i < howmany; i++) {
		if(asn == n[i]) return i;
	}
	return howmany; // ie not found
}

AStarNode* AStarList::PositionAlreadyThere(AStarNode *asn) {
	int i;
	for(i=0; i< howmany; i++) {
		if(asn->SameTiles(n[i]->loc)) return n[i];
	}
	return NULL;
}


void AStarList::Delete(AStarNode *asn) {
	int deletee;
	int i;
cout << endl << "BEFORE DELETING " << howmany;
	deletee = Find(asn);
	if(deletee >= howmany) return;
	for(i=deletee+1; i < howmany; i++) {
		n[i-1] = n[i];
	}
	howmany--;
cout << " AFTER DELETING " << howmany << endl;
}
