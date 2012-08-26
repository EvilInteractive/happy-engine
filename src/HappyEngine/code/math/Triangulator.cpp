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
//Created: 14/10/2011

#include "HappyPCH.h" 

#include "Triangulator.h"
#include "HappyNew.h"
#include "HappyTypes.h"
#include <algorithm>

namespace he {

bool Triangulator::triangulatePolygon(const std::vector<vec2>& vertices, std::vector<uint>& indices)
{
    int n = (int)vertices.size();
    if ( n < 3 ) return false;

    if (isConvex(vertices))
    {
        triangulateConvex(vertices, indices);

        return true;
    }
    else
    {
        return triangulateConcave(vertices, indices);
    }
}

void Triangulator::triangulateConvex(const std::vector<vec2>& vertices, std::vector<uint>& indices)
{
    for (uint i(0); i < vertices.size() - 2; ++i)
    {
        indices.push_back(0);
        indices.push_back(i + 1);
        indices.push_back(i + 2);
    }
}

bool Triangulator::triangulateConcave(const std::vector<vec2>& vertices, std::vector<uint>& indices)
{
    int n = (int)vertices.size();
    int* V = NEW int[n]; // TODO: seeb cache this buffer as a member, enlarge if needed

    if ( 0.0f < calculateArea(vertices) )
        for (int v=0; v<n; v++) V[v] = v;
    else
        for(int v=0; v<n; v++) V[v] = (n-1)-v;

    int nv = n;

    int count = 2*nv;

    for(int m=0, v=nv-1; nv>2; )
    {
        if (0 >= (count--))
        {
            delete[] V;
            return false;
        }

        int u = v  ; if (nv <= u) u = 0;
        v = u+1; if (nv <= v) v = 0;
        int w = v+1; if (nv <= w) w = 0;

        if ( snip(vertices,u,v,w,nv,V) )
        {
            int a,b,c,s,t;

            a = V[u]; b = V[v]; c = V[w];

            indices.push_back(a);
            indices.push_back(b);
            indices.push_back(c);

            m++;

            for(s=v,t=v+1;t<nv;s++,t++) V[s] = V[t]; nv--;

            count = 2*nv;
        }
    }

    delete[] V;

    return true;
}

float Triangulator::calculateArea(const std::vector<vec2>& vertices)
{
    int n((int)vertices.size());
    float A(0.0f);

    for (int p(n - 1), q(0); q < n; p = q++)
    {
        A += vertices[p].x * vertices[q].y - vertices[q].x * vertices[p].y;
    }

    return A * 0.5f;
}

bool Triangulator::hitTestTriangle(const vec2& p1, const vec2& p2, const vec2& p3, const vec2& hP)
{
    vec2 a, b, c, p1p, p2p, p3p;
    vec3 cross;

    a.x   = p3.x - p2.x; a.y   = p3.y - p2.y;
    b.x	  = p1.x - p3.x; b.y   = p1.y - p3.y;
    c.x	  = p2.x - p1.x; c.y   = p2.y - p1.y;
    p1p.x = hP.x - p1.x; p1p.y = hP.y - p1.y;
    p2p.x = hP.x - p2.x; p2p.y = hP.y - p2.y;
    p3p.x = hP.x - p3.x; p3p.y = hP.y - p3.y;

    cross.x = a.x * p2p.y - a.y * p2p.x;
    cross.y = c.x * p1p.y - c.y * p1p.x;
    cross.z = b.x * p3p.y - b.y * p3p.x;

    return (cross.z >= 0.0f && cross.y >= 0.0f && cross.x >= 0.0f);
}

bool Triangulator::isConvex(const std::vector<vec2>& vertices)
{
    if (vertices.size() < 3)
        return false;

    if (vertices.size() == 3)
        return true;

    int winding(sign(calculateArea(vertices)));

    int a((int)vertices.size() - 4);
    int b((int)vertices.size() - 2);

    vec2 t1,t2,t3;
    std::vector<vec2> tri;

    for (uint c(0); c < vertices.size(); ++c)
    {
        t1 = vertices[a];
        t2 = vertices[b];
        t3 = vertices[c];

        tri.clear();
        tri.push_back(t1);
        tri.push_back(t2);
        tri.push_back(t3);

        if (sign(calculateArea(tri)) != winding)
            return false;

        a = b;
        b = c;
    }

    return true;
}

bool Triangulator::snip(const std::vector<vec2>& vertices, int u, int v, int w, int n, int* V)
{
    vec2 a, b, c, p;

    a = vertices[V[u]];
    b = vertices[V[v]];
    c = vertices[V[w]];

    if ( FLT_EPSILON > (((b.x - a.x) * (c.y - a.y)) - ((b.y - a.y) * (c.x - a.x))) )
        return false;

    for (int i(0); i < n; ++i)
    {
        if ((i == u) || (i == v) || (i == w))
            continue;

        p = vertices[V[i]];

        if (hitTestTriangle(a,b,c,p))
            return false;
    }

  return true;
}

} //end namespace