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
    std::deque<ProfileData> m_Data;
    std::string m_Name;
    std::map<std::string, ProfileTreeNode> m_Nodes;

    bool operator<(const ProfileTreeNode& node) const
    {
        return m_Name < node.m_Name;
    }
};

Profiler::Profiler(): m_CurrentNode(nullptr), m_Width(0), m_pFont(nullptr), m_pCanvas2D(nullptr), m_View(nullptr)
{
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
        m_View->get2DRenderer()->removeCanvas(m_pCanvas2D);
    }
    if (m_pFont != nullptr)
        m_pFont->release();
    delete m_pCanvas2D;
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

void Profiler::begin( const std::string& name )
{
    std::map<std::string, ProfileTreeNode>& currentBranch(m_CurrentNode != nullptr ? m_CurrentNode->m_Nodes : m_Data);
    
    if (currentBranch.find(name) == currentBranch.cend())
    {
        ProfileTreeNode node;
        node.m_Name = name;
        node.m_Parent = m_CurrentNode;
        currentBranch[name] = node;
    }
    m_CurrentNode = &currentBranch[name];

    if (m_CurrentNode->m_Data.size() == MAX_DATA)
        m_CurrentNode->m_Data.pop_front();
    m_CurrentNode->m_Data.push_back(ProfileData());
    m_CurrentNode->m_Data.back().startTime = boost::chrono::high_resolution_clock::now();
}

void Profiler::end()
{
    HE_ASSERT(m_CurrentNode != nullptr, "called PROFILER_END to many times?");
    m_CurrentNode->m_Data.back().endTime = boost::chrono::high_resolution_clock::now();
    m_CurrentNode = m_CurrentNode->m_Parent;
}
void Profiler::drawProfileNode(const ProfileTreeNode& node, gui::Text& text, int treeDepth)
{
    double avgTime(0.0);
    std::for_each(node.m_Data.cbegin(), node.m_Data.cend()-1, [&](const ProfileData& data)
    {
        avgTime += data.getDuration() * 1000;
    });
    char buff[16];
    std::sprintf(buff, ": %07.2f ms\0", avgTime / node.m_Data.size());

    std::stringstream stream;
    for (int i(0); i < treeDepth; ++i)
    {
        /*if (i == treeDepth - 1)
            stream << "|";*/
        stream << "|----";
    }

    stream << "+ " + node.m_Name << buff;
    text.addLine(stream.str());

    uint textWidth(static_cast<uint>(m_pFont->getStringWidth(stream.str())));
    if (textWidth > m_Width)
        m_Width = textWidth;

    std::for_each(node.m_Nodes.cbegin(), node.m_Nodes.cend(), [&](const std::pair<std::string, ProfileTreeNode>& treeNodePair)
    {       
        drawProfileNode(treeNodePair.second, text, treeDepth + 1);
    });
}
void Profiler::draw2D(gfx::Renderer2D* renderer)
{
    HIERARCHICAL_PROFILE(__HE_FUNCTION__);
    m_Width = 0;
    
    gui::Text text(m_pFont);
    text.addLine("----PROFILER------------------------------");
    std::for_each(m_Data.cbegin(), m_Data.cend(), [&](const std::pair<std::string, ProfileTreeNode>& treeNodePair)
    {     
        drawProfileNode(treeNodePair.second, text, 1);  
    });
    text.addLine("------------------------------------------");

    float y((float)(text.getText().size() * m_pFont->getLineSpacing()));

    m_pCanvas2D->setFillColor(Color(0.3f, 0.3f, 0.3f, 0.8f));
    m_pCanvas2D->fillRect(vec2(0, 0), vec2(m_Width + 5.0f, y + 5.0f));

    m_pCanvas2D->setFillColor(Color(1.0f, 1.0f, 1.0f));
    m_pCanvas2D->fillText(text, vec2(4, 4));

    m_pCanvas2D->draw2D(renderer);
}

void Profiler::setView( gfx::View* view )
{
    if (m_View != nullptr)
    {
        m_View->get2DRenderer()->detachFromRender(this);
        m_View->get2DRenderer()->removeCanvas(m_pCanvas2D);
    }
    m_View = view;
    m_View->get2DRenderer()->attachToRender(this);
    m_pCanvas2D = m_View->get2DRenderer()->createCanvasRelative(RectF(0, 0, 1, 1));
}

} } //end namespace