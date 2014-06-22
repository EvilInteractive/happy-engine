//HappyEngine Copyright (C) 2011 - 2014  Evil Interactive
//
//This file is part of HappyEngine.
//
//    HappyEngine is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Lesser General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    HappyEngine is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Lesser General Public License for more details.
//
//    You should have received a copy of the GNU Lesser General Public License
//    along with HappyEngine.  If not, see <http://www.gnu.org/licenses/>.
//
//Author: Thijs Morlion
//Created: 16/03/2013

#ifndef _HT_AStar_H_
#define _HT_AStar_H_
#pragma once
#include "List.h"
#include "Grid.h"

namespace ht {

class AStar
{
public:

	struct AStarStatistics
	{
		int nodesSearched;
		int nodesAdded;
		int nodesRemoved;
		int nodesVisited;
		int nodesLeft;
		int pathLength;
		int pathCost;

		AStarStatistics()
			:nodesSearched(0)
			,nodesAdded(0)
			,nodesRemoved(0)
			,nodesVisited(0)
			,nodesLeft(0)
			,pathLength(0)
			,pathCost(0)
		{

		}
	};

    AStar();
    virtual ~AStar();

	void init();
	void draw2D(he::gui::Canvas2D* /*canvas*/ );
	void Calculate();

	void ProcessNode(const int nodeIndex);

	// SET GOAL AND START
	void SetGoal(const int column, const int row);
	void SetStart(const int column, const int row);

	int nodeSorter(const int a, const int b);

private:

	Grid* m_Grid;

	AStarStatistics m_Stats;
	he::ObjectList<int> m_OpenNodes; // OPEN SET
	he::ObjectList<int> m_ClosedNodes; // CLOSED SET

	int m_StartIndex; 
	int m_GoalIndex;

    //Disable default copy constructor and default assignment operator
    AStar(const AStar&);
    AStar& operator=(const AStar&);
};

} //end namespace

#endif
