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
//Created: 21/11/2011

#ifndef _HE_PROFILER_H_
#define _HE_PROFILER_H_
#pragma once

#include "IDrawable2D.h"
#include "Text.h"

namespace he {
namespace gui {
    class Text;
}
namespace gfx {
    class Renderer2D;
    class View;
    class Font;
}
namespace tools {

class HAPPY_ENTRY HierarchicalProfile
{
public:
    HierarchicalProfile(const char* name);
    ~HierarchicalProfile();
private:
    //Disable default copy constructor and default assignment operator
    HierarchicalProfile(const HierarchicalProfile&);
    HierarchicalProfile& operator=(const HierarchicalProfile&);
};

struct ProfileData;

class HAPPY_ENTRY Profiler : public gfx::IDrawable2D
{
    struct ProfileTreeNode;
public:
    static Profiler* getInstance();
    static void dispose();

    void load();

    void tick(); // resets frame counter

    void begin(const char* name);
    void end();

    void enable();
    void disable();

    inline bool isEnabled() { return m_State != Disabled && m_State != Enabling; }

    void attachToRenderer(gfx::Renderer2D* renderer);
    void detachFromRenderer();
    virtual void draw2D(gui::Canvas2D* canvas);

    void toggleProfiler();
    
    typedef he::ObjectList<ProfileTreeNode> DataMap;
private:

    Profiler();
    virtual ~Profiler();

    static Profiler* s_Profiler;

    // Double buffered data
    DataMap* m_NodesFront;
    DataMap* m_NodesBack;

    void resetNode(ProfileTreeNode& node);
    void drawProfileNode(const ProfileTreeNode& node, gui::Text& text, int treeDepth, size_t& lines);

    ProfileTreeNode* m_CurrentNode;
    ProfileTreeNode* m_PrevNode;

    gui::Font* m_Font;
    gfx::Renderer2D* m_Renderer;
    gui::Text m_Text;

    enum State
    {
        Enabled,
        Disabled,
        Enabling,
        Disabling
    };
    uint8 m_State;

    uint32 m_Width;

    //Disable default copy constructor and default assignment operator
    Profiler(const Profiler&);
    Profiler& operator=(const Profiler&);
};

#define ENABLE_PROFILING

#define PROFILER he::tools::Profiler::getInstance()

#ifdef ENABLE_PROFILING
#define PROFILER_BEGIN(name) PROFILER->begin(name)
#define PROFILER_END() PROFILER->end()
#define HIERARCHICAL_PROFILE(name) he::tools::HierarchicalProfile __hierarchical_profile(name);
#else
#define PROFILER_BEGIN(name) {}
#define PROFILER_END() {}
#define HIERARCHICAL_PROFILE(name) {}
#endif

} } //end namespace

#endif
