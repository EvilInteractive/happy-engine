//HappyEngine Copyright (C) 2011  Bastian Damman, Sebastiaan Sprengers
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

#include "Vertex.h"

namespace happyengine {
namespace graphics {
    
/*----------------------------------------------------------------*/
/*                          VertexPosCol                          */
/*----------------------------------------------------------------*/
VertexPosCol::VertexPosCol() : position(), color()
{
}
VertexPosCol::VertexPosCol(const math::Vector3& pos, const math::Vector4& col): position(pos), color(col)
{
}
VertexPosCol::~VertexPosCol()
{
}

/*----------------------------------------------------------------*/
/*                          VertexPosCol2D                        */
/*----------------------------------------------------------------*/
VertexPosCol2D::VertexPosCol2D() : position(), color()
{
}
VertexPosCol2D::VertexPosCol2D(const math::Vector2& pos, const math::Vector4& col): position(pos), color(col)
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
VertexPosTex::VertexPosTex(const math::Vector3& pos, const math::Vector2& tex): position(pos), textureCoord(tex)
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
VertexPosTex2D::VertexPosTex2D(const math::Vector2& pos, const math::Vector2& tex): position(pos), textureCoord(tex)
{
}
VertexPosTex2D::~VertexPosTex2D()
{
}

} } //end namespace