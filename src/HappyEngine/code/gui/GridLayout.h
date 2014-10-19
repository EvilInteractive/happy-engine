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
//Created: 2014/10/10

#ifndef _HE_GridLayout_H_
#define _HE_GridLayout_H_
#pragma once

#include "DefaultLayoutContainer.h"
#include "DefaultLayoutable.h"

namespace he {
namespace gui {

class HAPPY_ENTRY GridLayout : public DefaultLayoutContainer, public DefaultLayoutable
{
public:
    struct LayoutData;

    GridLayout();
    virtual ~GridLayout();

    // GridLayout
    void initGrid(const uint8 rows, const uint8 columns);
    void setAt(const uint8 row, const uint8 column, const uint8 span, ILayoutable* const obj);
    void removeAt(const uint8 row, const uint8 column);
    bool remove(ILayoutable* const obj);

    void setRowHeight(const uint8 row, const float height);
    void setColumnWidth(const uint8 column, const float width);

    void setRowAutoSize(const uint8 row);
    void setColumnAutoSize(const uint8 column);

    // ILayoutContainer
    virtual void requestLayoutUpdate() override;

    // ILayoutable
    virtual void performLayout() override;

protected:
    virtual void setLayoutDirty();

private:
    void calculateColumnWidths(LayoutData* data);
    void calculateRowHeights(LayoutData* data);

    ILayoutable** m_Grid;
    float* m_RowHeight;
    float* m_ColumnWidth;
    ILayoutContainer* m_LayoutParent;

    uint8 m_Rows, m_Columns;

    //Disable default copy constructor and default assignment operator
    GridLayout(const GridLayout&);
    GridLayout& operator=(const GridLayout&);
};

} } //end namespace

#endif
