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
//
//Author:  Bastian Damman
//Created: 13/09/2011

#ifndef _HE_LINE_H_
#define _HE_LINE_H_
#pragma once

#include "Vector3.h"
#include "HappyTypes.h"
#include "boost/shared_ptr.hpp"

#include <vector>

namespace happyengine {
namespace graphics {

class Line
{
public:
	Line();
    virtual ~Line();

    void setVertices(const std::vector<math::Vector3>& vert);
    void setIndices(const std::vector<ushort>& ind);

    const std::vector<math::Vector3>& getVertices() const;
    const std::vector<ushort>& getIndices() const;

    typedef boost::shared_ptr<Line> pointer;

private:

    std::vector<math::Vector3> m_Vertices;
    std::vector<ushort> m_Indices;

    //Disable default copy constructor and default assignment operator
    Line(const Line&);
    Line& operator=(const Line&);
};

} } //end namespace

#endif
