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
#include "Canvas2Dnew.h"

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
    uint32 m_Recurses;
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
        if (m_Recurses == 0)
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

Profiler::Profiler()
    : m_CurrentNode(nullptr), 
      m_Width(0), 
      m_Font(nullptr), 
      m_Renderer(nullptr), 
      m_State(Disabled),
      m_NodesFront(NEW DataMap()),
      m_NodesBack(NEW DataMap())
{
    s_Stream.precision(2);
}
void Profiler::load()
{
    m_Font = CONTENT->loadFont("UbuntuMono-R.ttf", 11);
    m_Text.setFont(m_Font);

    CONSOLE->registerCmd(boost::bind(&he::tools::Profiler::toggleProfiler, this), "toggle_profiler");
}



Profiler::~Profiler()
{
    disable();
    if (m_Font != nullptr)
        m_Font->release();
    delete m_NodesBack;
    delete m_NodesFront;
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
    {
        m_State = Enabled;
    }
    if (m_State == Disabling)
    {
        m_State = Disabled;
        return;
    }

    std::swap(m_NodesFront, m_NodesBack);
    m_NodesFront->clear();
}

void Profiler::begin( const std::string& name )
{
    if (isEnabled() == false)
        return;

    if (m_CurrentNode != nullptr && m_CurrentNode->m_Name == name)
    { // recursive function
        ++m_CurrentNode->m_Recurses;
    }
    else
    {
        DataMap& currentBranch(m_CurrentNode != nullptr ? m_CurrentNode->m_Nodes : *m_NodesFront);
        DataMap::iterator it(currentBranch.find(name));
        if (it == currentBranch.end())
        {
            ProfileTreeNode node;
            node.m_Name = name;
            node.m_Parent = m_CurrentNode;
            node.m_HitsPerFrame = 0;
            node.m_TotalFrameTime = 0.0f;
            node.m_MaxTime = 0.0f;
            node.m_Recurses = 1;
            m_CurrentNode = &(currentBranch[name] = node);
        }
        else
        {
            m_CurrentNode = &it->second;
            m_CurrentNode->m_Recurses = 1;
        }

        m_CurrentNode->start();
    }
}

void Profiler::end()
{
    if (isEnabled() == false)
        return;

    HE_ASSERT(m_CurrentNode != nullptr, "called PROFILER_END to many times?");
    HE_ASSERT(m_CurrentNode->m_Recurses > 0, "Node not active?");
    --m_CurrentNode->m_Recurses;
    m_CurrentNode->stop();
    if (m_CurrentNode->m_Recurses == 0)
    {
        m_CurrentNode = m_CurrentNode->m_Parent;
    }
}
void Profiler::drawProfileNode(const ProfileTreeNode& node, gui::Text& text, int treeDepth)
{
    HE_ASSERT(node.m_HitsPerFrame > 0, "Cannot have 0 hit!");
    double totalFrameTime(node.m_TotalFrameTime);
    double avgFrameTime(node.m_TotalFrameTime / node.m_HitsPerFrame);
    double maxFrameTime(node.m_MaxTime);

    char totalBuf[10];
    char avgBuf[10];
    char maxBuf[10];
    sprintf(totalBuf, "%02.4f\0", totalFrameTime);
    sprintf(avgBuf, "%02.4f\0", avgFrameTime);
    sprintf(maxBuf, "%02.4f\0", maxFrameTime);

    s_Stream.clear();
    s_Stream.str("");
    for (int i(0); i < treeDepth; ++i)
    {
        s_Stream << "|----";
    }
    s_Stream << "+ " + node.m_Name << ": tot: " << totalBuf << " avg: " << avgBuf << " max: " << maxBuf << " calls: " << node.m_HitsPerFrame;
    text.addLine(s_Stream.str());

    uint32 textWidth(static_cast<uint32>(m_Font->getStringWidth(s_Stream.str())));
    if (textWidth > m_Width)
        m_Width = textWidth;

    std::for_each(node.m_Nodes.cbegin(), node.m_Nodes.cend(), [&](const std::pair<std::string, ProfileTreeNode>& treeNodePair)
    {       
        drawProfileNode(treeNodePair.second, text, treeDepth + 1);
    });
}
void Profiler::draw2D(gfx::Canvas2D* canvas)
{
    if (isEnabled() == false)
        return;
    HIERARCHICAL_PROFILE(__HE_FUNCTION__);
    m_Text.clear();
    m_Width = 0;
    
    m_Text.addLine("----PROFILER------------------------------");
    std::for_each(m_NodesBack->cbegin(), m_NodesBack->cend(), [&](const std::pair<std::string, ProfileTreeNode>& treeNodePair)
    {
        drawProfileNode(treeNodePair.second, m_Text, 1);  
    });
    m_Text.addLine("------------------------------------------");

    float y((float)(m_Text.getText().size() * m_Font->getLineSpacing()));

    he::gui::Canvas2Dnew* cvs(canvas->getRenderer2D()->getNewCanvas());
    canvas->setFillColor(Color(0.3f, 0.3f, 0.3f, 0.8f));
    canvas->fillRect(vec2(0, 0), vec2(m_Width + 5.0f, y + 5.0f));

    cvs->setColor(Color(1.0f, 1.0f, 1.0f));
    cvs->fillText(m_Text, vec2(4, 4));
}

void Profiler::attachToRenderer( gfx::Renderer2D* renderer )
{
    HE_IF_ASSERT(m_Renderer == nullptr, "Profiler already attached to a renderer")
    {
        m_Renderer = renderer;
        if (m_State == Enabled || m_State == Enabling)
            m_Renderer->attachToRender(this);
    }
}

void Profiler::detachFromRenderer()
{
    HE_IF_ASSERT(m_Renderer != nullptr, "Profiler not attached to a renderer")
    {
        if (m_State == Enabled || m_State == Enabling)
            m_Renderer->detachFromRender(this);
        m_Renderer = nullptr;
    }
}

void Profiler::enable()
{
    if (m_State != Enabled || m_State != Enabling)
    {
        m_State = Enabling;
        if (m_Renderer != nullptr)
            m_Renderer->attachToRender(this);
    }
}

void Profiler::disable()
{
    if (m_State != Disabled || m_State != Disabling)
    {
        m_State = Disabling;
        if (m_Renderer != nullptr)
            m_Renderer->detachFromRender(this);
    }
}

void Profiler::toggleProfiler()
{
    HE_ASSERT(m_Renderer != nullptr, "Profiler renderer not set!");
    if (m_State == Enabled || m_State == Enabling)
    {
        disable();
    }
    else if (m_State == Disabled || m_State == Disabling)
    {
        enable();
    }
}

} } //end namespace