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
    he::String m_Name;

    ProfileData m_CurrentProfile;
    Profiler::DataMap m_Nodes;

    ProfileTreeNode(): m_Parent(nullptr), m_TotalFrameTime(0.0), m_MaxTime(0.0), m_HitsPerFrame(0), m_Recurses(0), m_Name("") {}
    ProfileTreeNode(ProfileTreeNode& other)
        : m_Parent(other.m_Parent), m_TotalFrameTime(other.m_TotalFrameTime), 
        m_MaxTime(other.m_MaxTime), m_HitsPerFrame(other.m_HitsPerFrame), m_Recurses(other.m_Recurses), 
        m_Name(other.m_Name), m_CurrentProfile(other.m_CurrentProfile)
    {
        HE_ASSERT(other.m_Nodes.empty(), "A copied node cannot have nodes");
    }
    ProfileTreeNode& operator=(const ProfileTreeNode& other)
    {
        m_Parent = other.m_Parent;
        m_TotalFrameTime = other.m_TotalFrameTime;
        m_MaxTime = other.m_MaxTime;
        m_HitsPerFrame = other.m_HitsPerFrame;
        m_Recurses = other.m_Recurses;
        m_Name = other.m_Name;
        m_CurrentProfile = other.m_CurrentProfile;
        m_Nodes.clear();
        HE_ASSERT(other.m_Nodes.empty(), "A copied node cannot have nodes");
        return *this;
    }
    ~ProfileTreeNode() {}

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
}
void Profiler::load()
{
    m_Font = CONTENT->loadFont("UbuntuMono-R.ttf", 11);
    if (nullptr != m_Font)
    {
        m_Text.setFont(m_Font);
    }

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
    node.m_Nodes.forEach([this](ProfileTreeNode& node)   
    {
        resetNode(node); 
    });
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

void Profiler::begin( const char* name )
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
        size_t index(0);
        ProfileTreeNode* parent(m_CurrentNode);
        if (currentBranch.find_if([&name](const ProfileTreeNode& node) { return node.m_Name == name; }, index) == false)
        {
            ProfileTreeNode node;
            node.m_Name = name;
            node.m_Parent = parent;
            node.m_HitsPerFrame = 0;
            node.m_TotalFrameTime = 0.0f;
            node.m_MaxTime = 0.0f;
            node.m_Recurses = 1;
            currentBranch.add(node);
            m_CurrentNode = &currentBranch.back();
        }
        else
        {
            m_CurrentNode = &currentBranch[index];
            m_CurrentNode->m_Recurses = 1;
            m_CurrentNode->m_Parent = parent; // prev pointer could have been invalidated
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
void Profiler::drawProfileNode(const ProfileTreeNode& node, gui::Text& text, int treeDepth, size_t& lines)
{
    HE_ASSERT(node.m_HitsPerFrame > 0, "Cannot have 0 hit!");
    double totalFrameTime(node.m_TotalFrameTime);
    double avgFrameTime(node.m_TotalFrameTime / node.m_HitsPerFrame);
    double maxFrameTime(node.m_MaxTime);
    
    for (int i(0); i < treeDepth; ++i)
    {
        text.addText("|----");
    }
    text.addTextExt("+ %s: tot: %02.4f avg: %02.4f max: %02.4f calls: %d\n", node.m_Name.c_str(), totalFrameTime, 
        avgFrameTime, maxFrameTime, node.m_HitsPerFrame);
    
    ++lines;

    node.m_Nodes.forEach([&](const ProfileTreeNode& treeNode)
    {       
        drawProfileNode(treeNode, text, treeDepth + 1, lines);
    });
}
void Profiler::draw2D(gui::Canvas2D* canvas)
{
    if (isEnabled() == false)
        return;
    HIERARCHICAL_PROFILE(__HE_FUNCTION__);
    m_Text.clear();
    m_Width = 0;
    
    size_t lines(2);
    m_Text.addText("----PROFILER------------------------------\n");
    m_NodesBack->forEach([&](const ProfileTreeNode& treeNode)
    {
        drawProfileNode(treeNode, m_Text, 1, lines);  
    });
    m_Text.addText("------------------------------------------\n");

    //float y((float)(m_Text.getText().size() * m_Font->getLineSpacing()));

    //canvas->setFillColor(Color(0.3f, 0.3f, 0.3f, 0.8f));
    //canvas->fillRect(vec2(0, 0), vec2(m_Width + 5.0f, y + 5.0f));

    canvas->setColor(Color(1.0f, 1.0f, 1.0f));
    canvas->fillText(m_Text, vec2(4, 4));
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
    if (m_Renderer != nullptr)
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
