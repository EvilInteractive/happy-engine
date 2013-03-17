//HappyEngine Copyright (C) 2011 - 2012  Evil Interactive
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
//Author:  
//Created: //

#ifndef _HT_Node_H_
#define _HT_Node_H_
#pragma once

namespace ht {

class Node
{
public:

	enum NodeState
	{
		NODE_VISITED,
		NODE_UNVISITED,
		NODE_GOAL,
		NODE_START,
		NODE_PATH,
		NODE_BLOCKED,
		NODE_MAX
	};

	struct NodeCoord
	{
		int xPos;
		int yPos;

		NodeCoord()
			:xPos(0)
			,yPos(0)
		{

		}

		NodeCoord(int x, int y)
			:xPos(x)
			,yPos(y)
		{

		}
	};

    Node(int index, int xPos, int yPos);
	Node();

	NodeCoord GetCoords() const { return m_Coord; } 

	// STATE
	const NodeState GetState() const { return m_CurrentState; }
	void SetState(NodeState state) { m_CurrentState = state; }

	// PARENT
	const int GetParentIndex() const { return m_ParentIndex; }
	void SetParentIndex(const int index) { m_ParentIndex = index; }

	// SELF
	const int GetIndex() const { return m_Index; }
	void SetIndex(const int index) { m_Index = index; }

	// COST
	const int GetCost() const { return m_GValue; }
	void SetMovementCost(const int value) { m_GValue = value; }

	// HEURISTIC
	const int GetHeuristic() const { return m_HValue; }
	void SetHeuristic(const int value) { m_HValue = value; }
	void CalculateHeuristic(const Node& goal);

	// TOTAL SCORE
	const int GetTotalScore() const { return m_GValue + m_HValue; }

	virtual ~Node();
	Node& operator=(const Node&);
	bool operator==(const Node&) const;

private:

	NodeCoord m_Coord;		// location on our map
	int m_GValue;			// representation of how far we've already gone
	int m_HValue;			// estimation of how far is left
	int m_TotalScore;

	NodeState m_CurrentState;

	int m_Index;
	int m_ParentIndex;
	
    //Disable default copy constructor and default assignment operator
    Node(const Node&);
};

} //end namespace

#endif
