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

float getExposure(in sampler2D lumMap)
{
    return clamp(textureLod(lumMap, vec2(0.5f, 0.5f), 0).r, 0.01f, 2.0f);
}
vec3 tonemap(in vec3 hdr, float exposure)
{
    hdr = hdr / exposure;

    vec3 ldr = clamp(vec3(1) - hdr, 0.0f, 1.0f);
    ldr *= ldr;
    ldr = max((hdr * 0.25f) + 0.75f, vec3(1.0f)) - ldr;
    return pow(ldr * 0.5f, vec3(2.2f));
}
