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

#ifndef _HE_PROFILER_H_
#define _HE_PROFILER_H_
#pragma once

#include "Font.h"

namespace he {
    namespace gui {
        class Text;
    }
    namespace gfx {
        class Canvas2D;
    }
namespace tools {

class HierarchicalProfile
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

class Profiler
{
public:
    static Profiler* getInstance();
    static void dispose();

    void load();

    void begin(const std::string& name);
    void end();

    void draw();
    
private:
    struct ProfileTreeNode;

    Profiler();
    virtual ~Profiler();

    static Profiler* s_Profiler;

    static const int MAX_DATA = 50;
    std::map<std::string, ProfileTreeNode> m_Data;

    void drawProfileNode(const ProfileTreeNode& node, gui::Text& text, int treeDepth);

    ProfileTreeNode* m_CurrentNode;

    gfx::Font* m_pFont;
    gfx::Canvas2D* m_pCanvas2D;

    uint m_Width;

    //Disable default copy constructor and default assignment operator
    Profiler(const Profiler&);
    Profiler& operator=(const Profiler&);
};
#define PROFILER he::tools::Profiler::getInstance()

#define PROFILER_BEGIN(name) PROFILER->begin(name)
#define PROFILER_END PROFILER->end
#define HIERARCHICAL_PROFILE(name) he::tools::HierarchicalProfile __hierarchical_profile(name);

} } //end namespace

#endif
