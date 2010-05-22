#include "jCooperativeTable.h"

jCooperativeTable::jCooperativeTable()
{

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

	m_Map = new jMap();
	m_TimeDistance = 5;
		
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

locstruct jCooperativeTable::getRandomDest()
{
	int randX = rand() % 7;
	int randY = rand() % 7;

	locstruct STARTSTATE = { randX, randY };
	//if(!(m_TakenSpawn.find(STARTSTATE))
	//	m_TakenSpawn.push_back(STARTSTATE);


	return STARTSTATE;

}

locstruct jCooperativeTable::getRandomSpawn()
{
	int randX = rand() % 7;
	int randY = rand() % 7;

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


void jCooperativeTable::Update()
{
		MaintainTimeSlice();

}