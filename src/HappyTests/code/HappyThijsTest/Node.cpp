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
//Author:  Thijs Morlion
//Created: 16/03/2012
#include "HappyThijsTestPCH.h" 

#include "Node.h"

namespace ht {

Node::Node(int index, int xPos, int yPos)
	:m_Coord(xPos, yPos)
	,m_GValue(0)
	,m_HValue(0)
	,m_CurrentState(NODE_UNVISITED)
	,m_ParentIndex(0)
	,m_Index(index)
{
}

Node::Node()
	:m_Coord(0, 0)
	,m_GValue(0)
	,m_HValue(0)
	,m_CurrentState(NODE_UNVISITED)
	,m_ParentIndex(0)
	,m_Index(0)
{

}


Node::~Node()
{
}

Node& Node::operator=( const Node& other)
{
	m_Coord = other.m_Coord;
	m_CurrentState = other.m_CurrentState;
	m_Index = other.m_Index;
	m_ParentIndex = other.m_ParentIndex;
	m_HValue = other.m_HValue;
	m_GValue = other.m_GValue;

	return *this;
}

bool Node::operator==( const Node& other) const
{
	if (m_Coord.xPos != other.m_Coord.xPos)
		return false;

	if (m_Coord.yPos != other.m_Coord.yPos)
		return false;

	if (m_CurrentState != other.m_CurrentState)
		return false;

	if (m_GValue != other.m_GValue)
		return false;

	if (m_HValue != other.m_HValue)
		return false;

	if (m_Index != other.m_Index)
		return false;

	if (m_ParentIndex != other.m_ParentIndex)
		return false;

	return true;
}

void Node::CalculateHeuristic( const Node& goal )
{
	//MANHATTAN DISTANCE
	//H = 10*(abs(currentX-targetX) + abs(currentY-targetY))
	int goalX = goal.GetCoords().xPos;
	int goalY = goal.GetCoords().yPos;

	int xScore = abs(GetCoords().xPos-goalX);
	int yScore = abs(GetCoords().yPos-goalY);
	SetHeuristic(1* (xScore + yScore));
}

} //end namespace