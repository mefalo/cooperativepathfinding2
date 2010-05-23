#include "jMap.h"

jMap::jMap()
{
	int row, col;
	for(row=0; row < MAXY ; row++) {
			for(col=0; col < MAXX; col++) {
				int r = (rand() % 6) + 1;
				myMap[row][col] = r;
			}
	}
}

jMap::~jMap()
{

}