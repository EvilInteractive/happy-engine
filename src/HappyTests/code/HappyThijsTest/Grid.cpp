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
//Author:  Thijs Morlion
//Created: 16/03/2012
#include "HappyThijsTestPCH.h" 

#include "Grid.h"
#include "HappyEngine.h"
#include "ContentManager.h"
#include "Canvas2D.h"
#include "Font.h"

namespace ht {

Grid::Grid(int width, int height)
	:m_Width(width)
	,m_Height(height)
	,m_UnvisitedTexture(nullptr)
	,m_VisitedTexture(nullptr)
	,m_BlockedTexture(nullptr)
	,m_GoalTexture(nullptr)
	,m_StartTexture(nullptr)
	,m_PathTexture(nullptr)
	,m_DebugText(nullptr)
{
}


Grid::~Grid()
{
}

void Grid::init()
{
	// FILL UP GRID
	for (int columns = 0; columns < m_Width; ++columns)
	{
		for (int rows = 0; rows < m_Height; ++rows)
		{
			m_Nodes.add(ht::Node(getNodeIndex(columns, rows), columns, rows));
		}
	}

	// CREATE WALL
	int nodeIndex = 36;
	for (int i = 0; i < 4; ++i)
	{
		Node& testNode = m_Nodes[nodeIndex];
		testNode.SetState(ht::Node::NODE_BLOCKED);

		int topNodeIndex;
		getNeigbour(testNode, ht::Grid::NeighBour_Top, topNodeIndex);
		Node& topNode = getNodeByIndex(topNodeIndex);
		topNode.SetState(ht::Node::NODE_BLOCKED);
		nodeIndex = getNodeIndex(topNode.GetCoords().xPos, topNode.GetCoords().yPos);
	}


	// LOAD TEXTURES
	m_UnvisitedTexture = CONTENT->asyncLoadTexture2D("AI/node_unvisited.png");
	m_VisitedTexture = CONTENT->asyncLoadTexture2D("AI/node_visited.png");
	m_StartTexture = CONTENT->asyncLoadTexture2D("AI/node_start.png");
	m_GoalTexture = CONTENT->asyncLoadTexture2D("AI/node_goal.png");
	m_BlockedTexture = CONTENT->asyncLoadTexture2D("AI/node_blocked.png");
	m_PathTexture = CONTENT->asyncLoadTexture2D("AI/node_path.png");

	// INIT DEBUGTEXT
	he::ct::ContentManager* contentMan(CONTENT);
	he::gui::Font* const debugFont(contentMan->getDefaultFont(6));
	m_DebugText = NEW he::gui::Text();
	m_DebugText->setFont(debugFont);
	debugFont->release();
}

void Grid::draw2D( he::gui::Canvas2D* canvas )
{
	m_Nodes.forEach([&](ht::Node& gridNode)
	{
		m_DebugText->clear();

		float xPos = (float)gridNode.GetCoords().xPos * 64;
		float yPos = (float)gridNode.GetCoords().yPos * 64;
		he::vec2 drawPos(xPos, yPos);

		switch(gridNode.GetState())
		{
		case ht::Node::NODE_UNVISITED:
			canvas->drawImage(m_UnvisitedTexture, drawPos);
			break;
		case ht::Node::NODE_VISITED:
			canvas->drawImage(m_VisitedTexture, drawPos);
			break;
		case ht::Node::NODE_BLOCKED:
			canvas->drawImage(m_BlockedTexture, drawPos);
			break;
		case ht::Node::NODE_GOAL:
			canvas->drawImage(m_GoalTexture, drawPos);
			break;
		case ht::Node::NODE_START:
			canvas->drawImage(m_StartTexture, drawPos);
			break;
		case ht::Node::NODE_PATH:
			canvas->drawImage(m_PathTexture, drawPos);
			break;
		}

		m_DebugText->addTextExt("GValue: %i\nHValue: %i\nTotal: %i\n", gridNode.GetCost(), gridNode.GetHeuristic(), gridNode.GetTotalScore());

		canvas->fillText(*m_DebugText, drawPos);
	});
}

bool Grid::getNeigbour( const Node& node, const Grid::NeighBour side, int& neighbourIndex )
{
	int xPos = node.GetCoords().xPos;
	int yPos = node.GetCoords().yPos;

	switch (side)
	{
		case NeighBour_Left:
			{
				if (xPos - 1 < 0)
					return false;

				neighbourIndex = getNodeIndex(xPos - 1, yPos);
				break;
			}
		case NeighBour_Right:
			{
				if (xPos + 1 > m_Width - 1)
					return false;

				neighbourIndex = getNodeIndex(xPos + 1, yPos);
				break;
			}
		case NeighBour_Top:
			{
				if (yPos - 1 < 0)
					return false;

				neighbourIndex = getNodeIndex(xPos, yPos - 1);
				break;
			}
		case NeighBour_Bottom:
			{
				if (yPos + 1 > m_Height - 1)
					return false;

				neighbourIndex = getNodeIndex(xPos, yPos + 1);
				break;
			}
	}

	return true;
}

void Grid::ResetNodes( const int goalIndex )
{
	m_Nodes.forEach([&](ht::Node& gridNode)
	{
		gridNode.CalculateHeuristic(getNodeByIndex(goalIndex));
	});
}

} //end namespace