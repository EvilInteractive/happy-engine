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
#include "HappyPCH.h" 

#include "HappyFxEditorBinding.h"
#include "HappyNew.h"

#include "boost/bind.hpp"

#include "HappyEngine.h"
#include "NetworkManager.h"

#include "ContentManager.h"

#include <stdlib.h>
#include <direct.h>
#include <stdio.h>

#include "Message.h"

#include "fx/FxManager.h"
#include "fx/FxTimeLine.h"

#include "Game.h"

namespace he {
namespace tools {

HappyFxEditorBinding::HappyFxEditorBinding(): m_pServer(NEW net::TcpServer()), m_CurrentTimeLine(UINT_MAX), m_StartNew(false)
{
}



HappyFxEditorBinding::~HappyFxEditorBinding()
{
    delete m_pServer;
}

void HappyFxEditorBinding::init()
{
    m_pServer->setReceiveCallback(boost::bind(&HappyFxEditorBinding::receiveData, this, _1, _2));
    m_pServer->setConnectedCallback(boost::bind(&HappyFxEditorBinding::connected, this));
    GAME->addToTickList(this);
    start();
}

std::string getWorkDir()
{
    char* charWorkDir = nullptr;
    size_t workDirSize = 0;
    charWorkDir = _getcwd(charWorkDir, workDirSize);

    std::string tempDir(charWorkDir);
    std::stringstream workDir;
    std::for_each(tempDir.cbegin(), tempDir.cend(), [&workDir](char c)
    {
        if (c != '\\')
            workDir << c;
        else
            workDir << '/';
    });

    return workDir.str();
}
std::string getDataDir()
{
    std::string workDir(getWorkDir());
    std::string dataDir(CONTENT->getRootDir());

    if (workDir.back() == '/')
        workDir = workDir.substr(0, workDir.size() - 1);

    while(dataDir.find("../") != std::string::npos)
    {
        dataDir = dataDir.substr(3);
        workDir = workDir.substr(0, workDir.rfind("/"));
    }

    return workDir + "/" + dataDir;
}

void HappyFxEditorBinding::receiveData(const void* buffer, int /*bytesReceived*/ )
{
    FxInHeader::FxInHeaderEnum header = (FxInHeader::FxInHeaderEnum)(*((uint32*)buffer));
    
    net::details::Message::pointer msg;

    switch (header)
    {
        case FxInHeader::GetTexturePath:
            {
                std::string path(getDataDir() + CONTENT->getTextureFolder());
                uint header = FxOutHeader::SetTexturePath;

                char* buffer = (char*)he_malloc(sizeof(uint32) + path.size());
                uint32 pathlen(path.size());
                he_memcpy(buffer, &pathlen, sizeof(uint32));
                he_memcpy(&buffer[sizeof(uint32)], path.c_str(), path.size());
                msg = net::details::Message::createServerMsg(buffer, path.size() + sizeof(uint32), &header, sizeof(uint32));   
                he_free(buffer);

                break;
            }
        case FxInHeader::GetAudioPath:
            {
                std::string path(getDataDir() + CONTENT->getTextureFolder());
                uint header = FxOutHeader::SetTexturePath;
                
                char* buffer = (char*)he_malloc(sizeof(uint32) + path.size());
                uint32 pathlen(path.size());
                he_memcpy(buffer, &pathlen, sizeof(uint32));
                he_memcpy(&buffer[sizeof(uint32)], path.c_str(), path.size());
                msg = net::details::Message::createServerMsg(buffer, path.size() + sizeof(uint32), &header, sizeof(uint32));   
                he_free(buffer);

                break;
            }
        case FxInHeader::GetEffectPath:
            {
                std::string path(getDataDir() + CONTENT->getFxFolder());
                uint header = FxOutHeader::SetEffectPath;
                
                char* buffer = (char*)he_malloc(sizeof(uint32) + path.size());
                uint32 pathlen(path.size());
                he_memcpy(buffer, &pathlen, sizeof(uint32));
                he_memcpy(&buffer[sizeof(uint32)], path.c_str(), path.size());
                msg = net::details::Message::createServerMsg(buffer, path.size() + sizeof(uint32), &header, sizeof(uint32));   
                he_free(buffer);

                break;
            }
        case FxInHeader::Play:
            {
                m_StartNew = true;
            }
    }

    if (msg != nullptr)
        m_pServer->send(msg->getMsg(), msg->getSizeInBytes());
}

void HappyFxEditorBinding::start()
{
    m_pServer->start(3250);
    NETWORK->start();
}

void HappyFxEditorBinding::connected()
{
}

void HappyFxEditorBinding::tick( float /*dTime*/ )
{
    if (m_StartNew)
    {
        m_StartNew = false;
        m_CurrentTimeLine = CONTENT->loadFx("__fxeditor__.hfx");
        FX->getTimeline(m_CurrentTimeLine)->start();
    }
}

} } //end namespace