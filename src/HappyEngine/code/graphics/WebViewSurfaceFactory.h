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
//Author:  Sebastiaan Sprengers
//Created: 15/03/2013

#ifndef _HE_WEBVIEWSURFACEFACTORY_H_
#define _HE_WEBVIEWSURFACEFACTORY_H_
#pragma once

#ifdef USE_WEB
#include <Awesomium/Surface.h>

namespace he {
namespace gfx {

class WebViewSurfaceFactory : public Awesomium::SurfaceFactory
{
public:

    /* CONSTRUCTOR - DESTRUCTOR */
    WebViewSurfaceFactory();
    virtual ~WebViewSurfaceFactory();

    /* AWESOMIUM SURFACEFACTORY */
    virtual Awesomium::Surface* CreateSurface(Awesomium::WebView* view,
                                              int width, int height);
    virtual void DestroySurface(Awesomium::Surface* surface);

private:

    /* DEFAULT COPY & ASSIGNMENT */
    WebViewSurfaceFactory(const WebViewSurfaceFactory&);
    WebViewSurfaceFactory& operator=(const WebViewSurfaceFactory&);
};

}} //end namespace

#endif
#endif
