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
//Author:  Bastian Damman
//Created: 23/11/2012

#ifndef _HE_MaterialGeneratorGraph_H_
#define _HE_MaterialGeneratorGraph_H_
#pragma once

namespace he {
namespace ct {
    class ShaderGenerator;
}
namespace tools {

class MaterialGeneratorGraph
{
public:
    MaterialGeneratorGraph();
    virtual ~MaterialGeneratorGraph();

    ct::ShaderGenerator* getGenerator() const { return m_Generator; }

private:
    ct::ShaderGenerator* m_Generator;

    //Disable default copy constructor and default assignment operator
    MaterialGeneratorGraph(const MaterialGeneratorGraph&);
    MaterialGeneratorGraph& operator=(const MaterialGeneratorGraph&);
};

} } //end namespace

#endif
