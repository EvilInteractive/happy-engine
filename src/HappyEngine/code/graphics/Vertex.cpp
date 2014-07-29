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
#include "HappyPCH.h" 

#include "Vertex.h"

namespace he {
namespace gfx {
    
/*----------------------------------------------------------------*/
/*                          VertexPosCol                          */
/*----------------------------------------------------------------*/
VertexPosCol::VertexPosCol() : position(), color()
{
}
VertexPosCol::VertexPosCol(const vec3& pos, const vec4& col): position(pos), color(col)
{
}
VertexPosCol::~VertexPosCol()
{
}

/*----------------------------------------------------------------*/
/*                          VertexPos                             */
/*----------------------------------------------------------------*/
VertexPos::VertexPos() : position()
{
}
VertexPos::VertexPos(const vec3& pos): position(pos)
{
}
VertexPos::~VertexPos()
{
}

/*----------------------------------------------------------------*/
/*                          VertexPos2D                           */
/*----------------------------------------------------------------*/
VertexPos2D::VertexPos2D() : position()
{
}
VertexPos2D::VertexPos2D(const vec2& pos): position(pos)
{
}
VertexPos2D::~VertexPos2D()
{
}

/*----------------------------------------------------------------*/
/*                          VertexPosCol2D                        */
/*----------------------------------------------------------------*/
VertexPosCol2D::VertexPosCol2D() : position(), color()
{
}
VertexPosCol2D::VertexPosCol2D(const vec2& pos, const vec4& col): position(pos), color(col)
{
}
VertexPosCol2D::~VertexPosCol2D()
{
}


/*----------------------------------------------------------------*/
/*                          VertexPosTex                          */
/*----------------------------------------------------------------*/
VertexPosTex::VertexPosTex(): position(), textureCoord()
{
}
VertexPosTex::VertexPosTex(const vec3& pos, const vec2& tex): position(pos), textureCoord(tex)
{
}
VertexPosTex::~VertexPosTex()
{
}

/*----------------------------------------------------------------*/
/*                          VertexPosTex2D                        */
/*----------------------------------------------------------------*/
VertexPosTex2D::VertexPosTex2D(): position(), textureCoord()
{
}
VertexPosTex2D::VertexPosTex2D(const vec2& pos, const vec2& tex): position(pos), textureCoord(tex)
{
}
VertexPosTex2D::~VertexPosTex2D()
{
}

/*----------------------------------------------------------------*/
/*                          VertexPTNT                            */
/*----------------------------------------------------------------*/
VertexPTNT::VertexPTNT(): position(), textureCoord(), normal(), tangent()
{
}
VertexPTNT::VertexPTNT(const vec3& pos, const vec2& tex, const vec3& norm, const vec3& tan): 
        position(pos), textureCoord(tex), normal(norm), tangent(tan)
{
}

} } //end namespace