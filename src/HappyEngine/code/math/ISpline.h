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
//Author:  Bastian Damman
//Created: 08/09/2011

#ifndef _ISPLINE_H_
#define _ISPLINE_H_
#pragma once

namespace he {

namespace splines {

template<typename T>
class ISpline
{
public:
    virtual ~ISpline() {}

    virtual ISpline* clone() const = 0;

    virtual float arcLength(float /*t0*/, float /*t1*/) const = 0;

    virtual T getPoint(float /*t*/) const = 0;

    virtual T move(float /*distance*/, float& /*realTravelledDistance*/) = 0;

    virtual void reset() = 0;
};

} } //end namespace

#endif
