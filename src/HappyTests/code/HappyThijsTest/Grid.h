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

#ifndef _HT_Grid_H_
#define _HT_Grid_H_
#pragma once
#include "Node.h"
#include "Texture2D.h"
#include <IDrawable2D.h>

namespace he {
	namespace gui {
		class Text;
	}
}

namespace ht {

class Grid
{
public:

	enum NeighBour
	{
		NeighBour_Left,
		NeighBour_Right,
		NeighBour_Top,
		NeighBour_Bottom,
		NeighBour_Max
	};

    Grid(int width, int height);
    virtual ~Grid();

	void init();
	void draw2D(he::gui::Canvas2D* /*canvas*/ );

	inline Node& getNode(const int column, const int row) { return m_Nodes[((m_Width) * column) + row]; }
	inline Node& getNodeByIndex(const int index) { return m_Nodes[index]; }
	inline const int getNodeIndex(const int column, const int row) { return  ((m_Width) * column) + row; }

	bool getNeigbour(const Node& node, const Grid::NeighBour side, int& neighbourIndex);

	void ResetNodes(const int goalIndex);

private:

	int m_Width; 
	int m_Height;

	he::ObjectList<Node> m_Nodes;

	// TEXTURES
	const he::gfx::Texture2D* m_UnvisitedTexture;
	const he::gfx::Texture2D* m_VisitedTexture;
	const he::gfx::Texture2D* m_StartTexture;
	const he::gfx::Texture2D* m_GoalTexture;
	const he::gfx::Texture2D* m_BlockedTexture;
	const he::gfx::Texture2D* m_PathTexture;

	// DEBUG TEXT
	he::gui::Text* m_DebugText;

    //Disable default copy constructor and default assignment operator
    Grid(const Grid&);
    Grid& operator=(const Grid&);
};

} //end namespace

#endif
