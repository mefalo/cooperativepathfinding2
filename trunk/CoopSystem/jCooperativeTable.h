#ifndef J_COOPERATIVE_TABLE_H
#define J_COOPERATIVE_TABLE_H
//------------------------------------------------------------------------
//
//  Name:   JCooperativeTable.h
//
//  Desc:   Converts the 2D Nav Graph into a cooperative Table
//		for use by path finding algorithms.
//          
// ---------------------------------------------------------------------
#include <string>
#include <deque>
#include <vector>
#include "defs.h"
#include "jMap.h"


#define jCoop jCooperativeTable::Instance()

class jCooperativeTable
{
	public:

	 static jCooperativeTable& Instance()
		{
//    if (!m_pInstance)
			static jCooperativeTable m_pInstance;

		    return m_pInstance;
		}

	 		// Other non-static member functions
		//private:
		  jCooperativeTable();
		  jCooperativeTable(const jCooperativeTable&){};                 // Prevent copy-construction
		  jCooperativeTable& operator=(const jCooperativeTable&){};      // Prevent assignment
		  ~jCooperativeTable();

	 //Access functions
	jMap* getGraphAtTime(int time)
	 {
		 if(time >= 0 && time < m_TimeDistance)
		  return m_SlicedSparseGraph[time];
	 }

	 int getTimeDistance()
	 {
		 return m_TimeDistance;
	 }


	 void Init(float travelTime);
	 void Clear()
	 {
		 if (!m_SlicedSparseGraph.empty())
		 {
			 m_SlicedSparseGraph.clear();
	     }

	 }

	 
	locstruct getRandomSpawn();	
	locstruct getRandomDest();
	 
	 void MaintainTimeSlice();

	 void Update();


	protected:

		std::vector<locstruct> m_TakenSpawn;
		std::vector<locstruct> m_TakenDest;

		jMap* m_Map;

		int m_TimeDistance;
		
		//determined by the time it takes an agent to go from node to node.
		float m_UpdateInterval;


		bool m_Init;
		//Map version - NOT WORKING WELL AT ALL - Thought about using a Vector but you can't easily delete the first element
		//which is something i NEED TO DO!
		//std::map<int,NavGraph*>  m_SlicedSparseGraph;
		//std::map<int,NavGraph*>::iterator it;

//		std::queue<NavGraph*> m_SlicedSparseGraph;



		std::deque <jMap*> m_SlicedSparseGraph;

		
};

#endif