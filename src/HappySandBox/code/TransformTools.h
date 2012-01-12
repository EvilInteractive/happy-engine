//HappyEngine Copyright (C) 2011 - 2012  Bastian Damman, Sebastiaan Sprengers 
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
//Author:  Sebastiaan Sprengers
//Created: 11/12/2011

#ifndef _TRANSFORM_TOOLS_H_
#define _TRANSFORM_TOOLS_H_
#pragma once

#include "ModelMesh.h"
#include "ITickable.h"
#include "vec3.h"
#include "Entity.h"
#include "ModelComponent.h"

namespace happysandbox {

class TransformTools : public he::game::ITickable
{
public:

	enum MODE
	{
		MODE_OFF,
		MODE_MOVE,
		MODE_ROTATE,
		MODE_SCALE
	};

	/* CONSTRUCTOR - DESTRUCTOR */
    TransformTools();
    virtual ~TransformTools();

	/* GENERAL */
	void tick(float dTime);
	void draw();
	void setMode(MODE mode = MODE_OFF);

private:

	/* DATAMEMBERS */
	MODE m_Mode;

    he::game::Entity* m_pMoveTool;
    he::game::ModelComponent* m_pMoveAxis;
    he::game::ModelComponent* m_pMovePickAxisX;
    he::game::ModelComponent* m_pMovePickAxisY;
    he::game::ModelComponent* m_pMovePickAxisZ;
    he::game::ModelComponent* m_pMoveCenter;

	/*he::gfx::ModelMesh::pointer m_pMoveAxis;
    he::gfx::ModelMesh::pointer m_pMovePickAxis;
    he::gfx::ModelMesh::pointer m_pMoveCenter;*/
	he::gfx::ModelMesh::pointer m_pRotateAxis;
	he::gfx::ModelMesh::pointer m_pScaleAxis;

    he::gfx::BufferLayout m_BufferLayout;

    he::vec3 m_Pos;
    float m_Scale;

    bool m_bButtonDown;

    bool m_bLockX;
    bool m_bLockY;
    bool m_bLockZ;

    /* DEFAULT COPY & ASSIGNMENT */
    TransformTools(const TransformTools&);
    TransformTools& operator=(const TransformTools&);
};

} //end namespace

#endif
