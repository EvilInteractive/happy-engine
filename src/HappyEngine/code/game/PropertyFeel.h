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
//Created: 2013/03/20

#ifndef _HE_PropertyFeel_H_
#define _HE_PropertyFeel_H_
#pragma once

namespace he {
namespace ge {

enum EPropertyFeel
{
    ePropertyFeel_Default,
    ePropertyFeel_Path,
    ePropertyFeel_CheckBox,
    ePropertyFeel_Color,
    ePropertyFeel_Slider,
    ePropertyFeel_DropDown
};

class HAPPY_ENTRY PropertyFeel
{
public:
    virtual ~PropertyFeel() {}

    virtual EPropertyFeel getType() const = 0;
};

class HAPPY_ENTRY PropertyFeelDefault : public PropertyFeel
{
public:
    PropertyFeelDefault() {}
    virtual ~PropertyFeelDefault() {}

    virtual EPropertyFeel getType() const override { return ePropertyFeel_Default; }

private:

    //Disable default copy constructor and default assignment operator
    PropertyFeelDefault(const PropertyFeelDefault&);
    PropertyFeelDefault& operator=(const PropertyFeelDefault&);
};

class HAPPY_ENTRY PropertyFeelPath : public PropertyFeel
{
public:
    PropertyFeelPath(bool isDirectory, he::String filter) : m_IsDirectory(isDirectory), m_Filter(std::move(filter)) {}
    virtual ~PropertyFeelPath() {}

    virtual EPropertyFeel getType() const override { return ePropertyFeel_Path; }

    bool isDirectory() const { return m_IsDirectory; }
    const he::String& getFilter() const { return m_Filter; }

private:
    bool m_IsDirectory;
    he::String m_Filter;

    //Disable default copy constructor and default assignment operator
    PropertyFeelPath(const PropertyFeelPath&);
    PropertyFeelPath& operator=(const PropertyFeelPath&);
};

class HAPPY_ENTRY PropertyFeelCheckBox : public PropertyFeel
{
public:
    PropertyFeelCheckBox() {}
    virtual ~PropertyFeelCheckBox() {}

    virtual EPropertyFeel getType() const override  { return ePropertyFeel_CheckBox; }

private:

    //Disable default copy constructor and default assignment operator
    PropertyFeelCheckBox(const PropertyFeelCheckBox&);
    PropertyFeelCheckBox& operator=(const PropertyFeelCheckBox&);
};

class HAPPY_ENTRY PropertyFeelColor : public PropertyFeel
{
public:
    PropertyFeelColor() {}
    virtual ~PropertyFeelColor() {}

    virtual EPropertyFeel getType() const override  { return ePropertyFeel_Color; }

private:

    //Disable default copy constructor and default assignment operator
    PropertyFeelColor(const PropertyFeelColor&);
    PropertyFeelColor& operator=(const PropertyFeelColor&);
};

class HAPPY_ENTRY PropertyFeelSlider : public PropertyFeel
{
public:
    PropertyFeelSlider(const float min, const float max) : m_Min(min), m_Max(max) {}
    virtual ~PropertyFeelSlider() {}

    virtual EPropertyFeel getType() const override  { return ePropertyFeel_Slider; }

    float getMin() const { return m_Min; }
    float getMax() const { return m_Max; }

private:
    float m_Min;
    float m_Max;

    //Disable default copy constructor and default assignment operator
    PropertyFeelSlider(const PropertyFeelSlider&);
    PropertyFeelSlider& operator=(const PropertyFeelSlider&);
};

class HAPPY_ENTRY PropertyFeelDropDown : public PropertyFeel
{
public:
    PropertyFeelDropDown() {}
    virtual ~PropertyFeelDropDown() {}

    virtual EPropertyFeel getType() const override  { return ePropertyFeel_DropDown; }

    void addItem(const char* item) { m_List.add(he::String(item)); }
    const he::ObjectList<he::String>& getList() const { return m_List; }

private:
    he::ObjectList<he::String> m_List;

    //Disable default copy constructor and default assignment operator
    PropertyFeelDropDown(const PropertyFeelDropDown&);
    PropertyFeelDropDown& operator=(const PropertyFeelDropDown&);
};

} } //end namespace

#endif
