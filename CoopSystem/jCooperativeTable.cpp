#include "jCooperativeTable.h"

jCooperativeTable::jCooperativeTable()
{
	m_Init = false;
}

jCooperativeTable::~jCooperativeTable()
{
		 if (!m_SlicedSparseGraph.empty())
		 {
			 m_SlicedSparseGraph.clear();
	     }
//delete m_SlicedSparseGraph;
}

void jCooperativeTable::Init(float travelTime)
{
if(!(m_Init))
{
	agent = 0;
	m_Init = true;
	m_Map = new jMap();
	m_TimeDistance = 50;
		
		//determined by the time it takes an agent to go from node to node.
	m_UpdateInterval = travelTime;

	for(int i = 0; i < m_TimeDistance; i++)
	{
		jMap* m_MapCopy = new jMap(*m_Map);

		//m_SlicedSparseGraph.insert(std::pair<int,NavGraph*>(i,copy_car1));
		m_SlicedSparseGraph.push_back(m_MapCopy);

		m_MapCopy = NULL;
		delete m_MapCopy;
	}
}
}

void jCooperativeTable::ReserveNode(int time, locstruct pos)
{
	if(time >= 0 && time < m_TimeDistance)
	{
	 m_SlicedSparseGraph[time]->myMap[pos.y][pos.x] = 7;
	 m_ReservedPlaces.push_back(pos);
	}
}

locstruct jCooperativeTable::getTestSpawn()
{
	if(agent == 0)
	{
		agent = 1;
		locstruct Spawn ={0,0};
		return Spawn;
	}

	if(agent == 1)
	{
		locstruct Spawn ={26,0};
		return Spawn;
	}
}

locstruct jCooperativeTable::getTestDest()
{
		if(agent == 0)
	{
		locstruct Spawn ={ 25,0};
		return Spawn;
	}

	if(agent == 1)
	{
		locstruct Spawn ={1,0};
		return Spawn;
	}
}
locstruct jCooperativeTable::getRandomDest()
{

	locstruct STARTDEST = { 25,25};
	int randX;
	int randY;

	//for(it_TakenDest = m_TakenDest.begin(); it_TakenDest != m_TakenDest.end(); it_TakenDest++)
	//{
	//	if(STARTDEST.x == (*(it_TakenDest)).x)
	//		if(STARTDEST.y == (*(it_TakenDest)).y)
		
		randX = rand() % MAXX;
		randY = rand() % MAXY;

		STARTDEST.x = randX;
		STARTDEST.y = randY;
	//}

	m_TakenDest.push_back(STARTDEST);
	return STARTDEST;
}

locstruct jCooperativeTable::getRandomSpawn()
{
	int randX = rand() % MAXX;
	int randY = rand() % MAXY;

	locstruct STARTSTATE = { randX, randY };
	//if(!(m_TakenSpawn.find(STARTSTATE))
	//	m_TakenSpawn.push_back(STARTSTATE);


	return STARTSTATE;

}


void jCooperativeTable::MaintainTimeSlice()
{
//Get the current position in Time, delete all sparse graphs before this time
//Create copy of last sparse graph and add to container.

	jMap* navGraphCopy = new jMap();

	m_SlicedSparseGraph.pop_front();
	m_SlicedSparseGraph.push_back(navGraphCopy);
	
	navGraphCopy = NULL;
	delete navGraphCopy;

}


void jCooperativeTable::ReserveTile(int Time, locstruct tile)
{
	m_SlicedSparseGraph[Time]->myMap[tile.x][tile.y] = 6;
}
void jCooperativeTable::Update()
{
		MaintainTimeSlice();

}
void jCooperativeTable::AddPath(int time, locstruct path)
{
	std::vector<locstruct>::iterator it;

	if(m_CollisionCheck[time].size() > 0)
	{
		for(it = m_CollisionCheck[time].begin(); it != m_CollisionCheck[time].end(); it++)
			if((*it).x == path.x)
				if((*it).y == path.y)
				m_CollisionOccured.push_back((*it));
	}

	m_CollisionCheck[time].push_back(path);
}