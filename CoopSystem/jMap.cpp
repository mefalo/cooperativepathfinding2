#include "jMap.h"

jMap::jMap()
{
	int row, col;
	for(row=0; row < MAXY ; row++) {
			for(col=0; col < MAXX; col++) {
				int r = (rand() % 20) + 1;
				myMap[row][col] = 1;
			if(r > 17)
				myMap[row][col] = 6;
			}
	}
}

jMap::~jMap()
{

}