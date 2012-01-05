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

//source: http://aras-p.info/texts/CompactNormalStorage.html#method04spheremap
//vec2 encodeNormal(vec3 normal)
//{
    //vec2 encNormal = normalize(normal.xy) * sqrt(-normal.z * 0.5f + 0.5f);
    //return encNormal * 0.5f + 0.5f;
//}
vec2 encodeNormal(vec3 normal)
{
    float f = sqrt(-normal.z * 8.0f + 8.0f);
    return normal.xy / f + 0.5f;
}