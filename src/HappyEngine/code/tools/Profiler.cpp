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
//Created: 21/11/2011
#include "HappyPCH.h" 

#include "Profiler.h"
#include "Renderer2D.h"

#include "ContentManager.h"

#include "Font.h"
#include "Text.h"
#include "Canvas2D.h"
#include "View.h"

namespace he {
namespace tools {

Profiler* Profiler::s_Profiler = nullptr;
std::stringstream Profiler::s_Stream;

HierarchicalProfile::HierarchicalProfile(const char* name)
{
    PROFILER_BEGIN(name);
}
HierarchicalProfile::~HierarchicalProfile()
{
    PROFILER_END();
}

struct ProfileData
{
    boost::chrono::high_resolution_clock::time_point startTime;
    boost::chrono::high_resolution_clock::time_point endTime;

    double getDuration() const
    {
        boost::chrono::high_resolution_clock::duration elapsedTime(endTime - startTime);
        return elapsedTime.count() / static_cast<double>(boost::nano::den);
    }
};

struct Profiler::ProfileTreeNode
{
    ProfileTreeNode* m_Parent;
    double m_TotalFrameTime;
    double m_MaxTime;
    uint32 m_HitsPerFrame;
    std::string m_Name;

    ProfileData m_CurrentProfile;

    Profiler::DataMap m_Nodes;

    void start() 
    { 
        m_CurrentProfile.startTime = boost::chrono::high_resolution_clock::now(); 
    }
    void stop() 
    { 
        m_CurrentProfile.endTime = boost::chrono::high_resolution_clock::now(); 
        double time = m_CurrentProfile.getDuration();
        m_MaxTime = std::max(m_MaxTime, time);
        m_TotalFrameTime += time;
        ++m_HitsPerFrame; 
    }
    void reset()    
    {
        m_HitsPerFrame = 0;
        m_MaxTime = 0.0;
        m_TotalFrameTime = 0.0;
    }
};

Profiler::Profiler(): m_CurrentNode(nullptr), m_Width(0), m_pFont(nullptr), m_Canvas2D(nullptr), m_View(nullptr), m_State(Disabled)
{
    s_Stream.precision(2);
}
void Profiler::load()
{
    m_pFont = CONTENT->loadFont("UbuntuMono-R.ttf", 11);
}



Profiler::~Profiler()
{
    if (m_View != nullptr)
    {
        m_View->get2DRenderer()->detachFromRender(this);
        m_View->get2DRenderer()->removeCanvas(m_Canvas2D);
    }
    if (m_pFont != nullptr)
        m_pFont->release();
    delete m_Canvas2D;
}

Profiler* Profiler::getInstance()
{
    if (s_Profiler == nullptr)
        s_Profiler = NEW Profiler();
    return s_Profiler;
}

void Profiler::dispose()
{
    delete s_Profiler;
    s_Profiler = nullptr;
}


void Profiler::resetNode( ProfileTreeNode& node )
{
    node.reset();
    DataMap::iterator it(node.m_Nodes.begin());
    for(; it != node.m_Nodes.end(); ++it)   
        resetNode(it->second); 
}
void Profiler::tick()
{
    HE_ASSERT(m_CurrentNode == nullptr, "No node should be active!");

    if (m_State == Disabled)
        return;

    if (m_State == Enabling)
        m_State = Enabled;
    if (m_State == Disabling)
    {
        m_State = Disabled;
        return;
    }

    DataMap::iterator it(m_Nodes.begin());
    for(; it != m_Nodes.end(); ++it)   
        resetNode(it->second);  
}

void Profiler::begin( const std::string& name )
{
    if (m_State == Disabled)
        return;

    DataMap& currentBranch(m_CurrentNode != nullptr ? m_CurrentNode->m_Nodes : m_Nodes);
    DataMap::iterator it(currentBranch.find(name));
    if (it == currentBranch.end())
    {
        ProfileTreeNode node;
        node.m_Name = name;
        node.m_Parent = m_CurrentNode;
        node.m_HitsPerFrame = 0;
        node.m_TotalFrameTime = 0.0f;
        m_CurrentNode = &(currentBranch[name] = node);
    }
    else
    {
        m_CurrentNode = &it->second;
    }

    m_CurrentNode->start();
}

void Profiler::end()
{
    if (m_State == Disabled)
        return;

    HE_ASSERT(m_CurrentNode != nullptr, "called PROFILER_END to many times?");
    m_CurrentNode->stop();
    m_CurrentNode = m_CurrentNode->m_Parent;
}
void Profiler::drawProfileNode(const ProfileTreeNode& node, gui::Text& text, int treeDepth)
{
    double avgFrameTime(node.m_TotalFrameTime / node.m_HitsPerFrame);
    double maxFrameTime(node.m_MaxTime);
    
    s_Stream.clear();
    s_Stream.str("");
    for (int i(0); i < treeDepth; ++i)
    {
        /*if (i == treeDepth - 1)
            stream << "|";*/
        s_Stream << "|----";
    }
    s_Stream << "+ " + node.m_Name << ": avg: " << avgFrameTime << " max: " << maxFrameTime << " calls: " << node.m_HitsPerFrame;
    text.addLine(s_Stream.str());

    uint textWidth(static_cast<uint>(m_pFont->getStringWidth(s_Stream.str())));
    if (textWidth > m_Width)
        m_Width = textWidth;

    std::for_each(node.m_Nodes.cbegin(), node.m_Nodes.cend(), [&](const std::pair<std::string, ProfileTreeNode>& treeNodePair)
    {       
        drawProfileNode(treeNodePair.second, text, treeDepth + 1);
    });
}
void Profiler::draw2D(gfx::Renderer2D* renderer)
{
    if (m_State == Disabled)
        return;

    HIERARCHICAL_PROFILE(__HE_FUNCTION__);
    m_Width = 0;
    
    gui::Text text(m_pFont);
    text.addLine("----PROFILER------------------------------");
    std::for_each(m_Nodes.cbegin(), m_Nodes.cend(), [&](const std::pair<std::string, ProfileTreeNode>& treeNodePair)
    {
        drawProfileNode(treeNodePair.second, text, 1);  
    });
    text.addLine("------------------------------------------");

    float y((float)(text.getText().size() * m_pFont->getLineSpacing()));

    m_Canvas2D->setFillColor(Color(0.3f, 0.3f, 0.3f, 0.8f));
    m_Canvas2D->fillRect(vec2(0, 0), vec2(m_Width + 5.0f, y + 5.0f));

    m_Canvas2D->setFillColor(Color(1.0f, 1.0f, 1.0f));
    m_Canvas2D->fillText(text, vec2(4, 4));

    m_Canvas2D->draw2D(renderer);
}

void Profiler::setView( gfx::View* view )
{
    if (m_View != nullptr)
    {
        m_View->get2DRenderer()->detachFromRender(this);
        m_View->get2DRenderer()->removeCanvas(m_Canvas2D);
    }
    m_View = view;
    m_View->get2DRenderer()->attachToRender(this);
    m_Canvas2D = m_View->get2DRenderer()->createCanvasRelative(RectF(0, 0, 1, 1));
}

void Profiler::enable()
{
    if (m_State != Enabled)
        m_State = Enabling;
}

void Profiler::disable()
{
    if (m_State != Disabled)
        m_State = Disabling;
}

} } //end namespace