/* Map class so that we can pass a pointer around when dealing with the map */

#ifndef JMAP_H
#define JMAP_H

#include "defs.h"
#include "SDL.h"

class jMap
{
public:
	jMap();
	~jMap();

	int myMap[MAXY][MAXX];

};

#endif