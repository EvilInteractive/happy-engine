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
//Author:  Bastian Damman
//Created: 23/11/2011

#ifndef _HE_HAPPY_FX_EDITOR_BINDING_H_
#define _HE_HAPPY_FX_EDITOR_BINDING_H_
#pragma once

#include "TcpServer.h"

namespace he {
namespace tools {


class HappyFxEditorBinding
{
public:
    HappyFxEditorBinding();
    virtual ~HappyFxEditorBinding();

    void init();

private:
    struct FxInHeader
    {
        enum
        {
            GetEffectPath = 0,
            GetTexturePath,
            GetModelPath,
            GetAudioPath,
        };
    };
    struct FxOutHeader
    {
        enum
        {
            SetEffectPath = 0,
            SetTexturePath,
            SetModelPath,
            SetAudioPath,
        };
    };

    net::TcpServer* m_pServer;

    void connected(); 
    void receiveData(int bytesReceived); 
    void start();

    //Disable default copy constructor and default assignment operator
    HappyFxEditorBinding(const HappyFxEditorBinding&);
    HappyFxEditorBinding& operator=(const HappyFxEditorBinding&);
};

} } //end namespace

#endif
