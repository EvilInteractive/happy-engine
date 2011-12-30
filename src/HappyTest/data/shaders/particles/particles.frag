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
//Author: Bastian Damman

#version 150 core

out vec4 outColor;

in vec2 passTexCoord;
in vec4 passBlendColor;
in vec2 passUvTile;

uniform sampler2D diffuseMap;
uniform vec2 uvTiles;

void main()
{
    vec4 diff = texture(diffuseMap, (passTexCoord / uvTiles) * (1 + passUvTile) );
    if (diff.a < 0.01f)
        discard;
    outColor = diff * passBlendColor;
}


