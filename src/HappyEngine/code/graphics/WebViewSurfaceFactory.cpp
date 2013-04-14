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
//Author:  Sebastiaan Sprengers
//Created: 15/03/2013

#include "HappyPCH.h"
#include "WebViewSurfaceFactory.h"

#include "WebViewSurface.h"

namespace he {
namespace gfx {

/* CONSTRUCTOR - DESTRUCTOR */
WebViewSurfaceFactory::WebViewSurfaceFactory()
{
}

WebViewSurfaceFactory::~WebViewSurfaceFactory()
{
}

/* AWESOMIUM SURFACEFACTORY */
Awesomium::Surface* WebViewSurfaceFactory::CreateSurface(Awesomium::WebView* /*view*/,
                                                         int width, int height)
{
    return NEW WebViewSurface((uint16)width, (uint16)height);
}

void WebViewSurfaceFactory::DestroySurface(Awesomium::Surface* surface)
{
    delete static_cast<WebViewSurface*>(surface);
}

}} //end namespace
