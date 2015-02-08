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
#include "HappyPCH.h" 

#include "GridLayout.h"

#define GRID_SPAN_POINTER reinterpret_cast<ILayoutable*>(0x00000001)

namespace he {
namespace gui {

GridLayout::GridLayout()
    : m_Grid(nullptr)
    , m_RowHeight(nullptr)
    , m_ColumnWidth(nullptr)
    , m_LayoutParent(nullptr)
    , m_Rows(0), m_Columns(0)
{
}

GridLayout::~GridLayout()
{
    he_free(m_Grid);
    he_free(m_RowHeight);
    he_free(m_ColumnWidth);
}

void GridLayout::initGrid( const uint8 rows, const uint8 columns )
{
    he_free(m_Grid);
    he_free(m_RowHeight);
    he_free(m_ColumnWidth);

    m_Grid = static_cast<ILayoutable**>(he_malloc("GridLayout::m_Grid", sizeof(ILayoutable*) * rows * columns));
    he_memset(m_Grid, 0, sizeof(ILayoutable**) * rows * columns);

    m_RowHeight = static_cast<float*>(he_malloc("GridLayout::m_RowHeight", sizeof(float) * rows));
    for (size_t i(0); i < rows; ++i)
        m_RowHeight[i] = -1.0f;

    m_ColumnWidth = static_cast<float*>(he_malloc("GridLayout::m_ColumnWidth", sizeof(float) * columns));
    for (size_t i(0); i < columns; ++i)
        m_ColumnWidth[i] = -1.0f;

    m_Rows = rows;
    m_Columns = columns;
    setLayoutDirty();
}

void GridLayout::setRowHeight( const uint8 row, const float height )
{
    HE_ASSERT(m_RowHeight, "GridLayout is not initialized!");
    HE_ASSERT(row < m_Rows, "Row is out of bound! Trying to set row size at row %u when only %u rows", row, m_Rows);
    if (m_RowHeight[row] != height)
    {
        m_RowHeight[row] = height;
        setLayoutDirty();
    }
}

void GridLayout::setColumnWidth( const uint8 column, const float width )
{
    HE_ASSERT(m_ColumnWidth, "GridLayout is not initialized!");
    HE_ASSERT(column < m_Columns, "Column is out of bound! Trying to set column size at column %u when only %u column", column, m_Columns);
    if (m_ColumnWidth[column] != width)
    {
        m_ColumnWidth[column] = width;
        setLayoutDirty();
    }
}

void GridLayout::setRowAutoSize( const uint8 row )
{
    HE_ASSERT(m_RowHeight, "GridLayout is not initialized!");
    if (m_RowHeight[row] != -1.0f)
    {
        m_RowHeight[row] = -1.0f;
        setLayoutDirty();
    }
}

void GridLayout::setColumnAutoSize( const uint8 column )
{
    HE_ASSERT(m_ColumnWidth, "GridLayout is not initialized!");
    if (m_ColumnWidth[column] != 1)
    {
        m_ColumnWidth[column] = -1.0f;
        setLayoutDirty();
    }
}

void GridLayout::setAt( const uint8 row, const uint8 column, uint8 span, ILayoutable* const obj )
{
    HE_IF_ASSERT(m_Grid, "GridLayout is not initialized!")
    HE_IF_ASSERT(row < m_Rows, "Row is out of bound! Trying to put at row %u when only %u rows", row, m_Rows)
    HE_IF_ASSERT(column < m_Columns, "Column is out of bound! Trying to put at column %u when only %u columns", column, m_Columns)
    HE_IF_ASSERT(column + span < m_Columns, "Column + span is out of bound! Trying to put obj at %u + %u but only %u columns", column, span, m_Columns)
    HE_IF_ASSERT(!m_Grid[row * m_Columns + column], "Already object @%u, %u!", row, column)
    {
        for (uint8 i(0); i < span; ++i)
        {
            he::gui::ILayoutable*& layoutable = m_Grid[row * m_Columns + column + i];
            if (!layoutable)
            {
                m_Grid[row * m_Columns + column + i] = GRID_SPAN_POINTER;
            }
            else
            {
                LOG(he::LogType_ProgrammerAssert, "Already object @%u, %u+%u", row, column, i);
                span = i - 1; // i == 0 is already checked by previous assert
                break;
            }
        }
        m_Grid[row * m_Columns + column + span] = obj;
        obj->initLayout(this);
        setLayoutDirty();
    }
}

void GridLayout::removeAt( const uint8 row, const uint8 column )
{
    HE_IF_ASSERT(m_Grid, "GridLayout is not initialized!")
    HE_IF_ASSERT(row < m_Rows, "Row is out of bound! Trying to remove at row %u when only %u rows", row, m_Rows)
    HE_IF_ASSERT(column < m_Columns, "Column is out of bound! Trying to remove at column %u when only %u columns", column, m_Columns)
    HE_IF_ASSERT(m_Grid[row * m_Columns + column], "No object @%u, %u!", row, column)
    HE_IF_ASSERT(column == 0 || m_Grid[row * m_Columns + column - 1] != GRID_SPAN_POINTER, "Removing invalid object @%u, %u! (Removing cell that is spanned by other object)", row, column)
    {
        m_Grid[row * m_Columns + column]->initLayout(nullptr);
        for (size_t c(column); c < m_Columns; ++c)
        {
            const ILayoutable* const obj(m_Grid[row * m_Columns + c]);
            m_Grid[row * m_Columns + c] = nullptr;
            if (obj != GRID_SPAN_POINTER)
            {
                setLayoutDirty();
                break;
            }
        }
    }

}

bool GridLayout::remove( ILayoutable* const obj )
{
    bool result(false);
    for (uint8 r(0); r < m_Rows; ++r)
    {
        uint8 spanStart(0);
        for (uint8 c(0); c < m_Columns; ++c)
        {
            if (m_Grid[r * m_Columns + c] == obj)
            {
                removeAt(spanStart, c);
                result = true;
                break;
            }
            else if (obj != GRID_SPAN_POINTER)
            {
                spanStart = r + 1;
            }
        }
    }
    return result;
}

void GridLayout::requestLayoutUpdate() 
{
    setLayoutDirty();
}

struct GridLayout::LayoutData
{
    LayoutData() : minSize(0.0f), maxSize(0.0f) {}
    float size;
    float minSize;
    float maxSize;
};

namespace
{
    void ResizeData(GridLayout::LayoutData* data, const int members, float space)
    {
        // try to evenly distribute
        size_t notFixed(0);
        for (int i(0); i < members; ++i)
        {
            GridLayout::LayoutData& data(data[i]);
            const float res(data.maxSize - data.minSize);
            if (res > FLT_EPSILON) // if not fixed
            {
                ++notFixed;
            }
        }
        for (int i(0); i < members; ++i)
        {
            GridLayout::LayoutData& data(data[i]);
            const float res(data.maxSize - data.minSize);
            if (res > FLT_EPSILON) // if not fixed
            {
                float resize(std::min(space / notFixed, res));
                data.size += resize;
                space -= resize;
                if (fabs(space) < FLT_EPSILON)
                    break;
            }
        }
        if (abs(space) > FLT_EPSILON)
        {
            // distribute excess
            for (int i(0); i < members; ++i)
            {
                GridLayout::LayoutData& data(data[i]);
                const float res(data.maxSize - data.minSize);
                if (res > FLT_EPSILON)
                {
                    float resize(std::min(space, res));
                    data.size += resize;
                    space -= resize;
                    if (fabs(space) < FLT_EPSILON)
                        break;
                }
            }
        }
    }
    void clipClipRect(RectF clipRect, RectF& inOutNewRect)
    {
        inOutNewRect.x = std::max(clipRect.x, inOutNewRect.x);
        inOutNewRect.y = std::max(clipRect.y, inOutNewRect.y);
        inOutNewRect.width = (inOutNewRect.x + inOutNewRect.width > clipRect.x + clipRect.width)? clipRect.x + clipRect.width - inOutNewRect.x : inOutNewRect.width;
        inOutNewRect.height = (inOutNewRect.y + inOutNewRect.height > clipRect.y + clipRect.height)? clipRect.y + clipRect.height - inOutNewRect.y : inOutNewRect.height;
    }
}

void GridLayout::calculateRowHeights( LayoutData* data )
{
    float autoSizeHeightSpace(m_LayoutBound.height - m_Margin.y - m_Margin.w);
    // Get all column width
    for (uint8 r(0); r < m_Rows; ++r)
    {
        LayoutData& data(data[r]);
        const float rh(m_RowHeight[r]);
        if (rh >= 0.0f)
        {
            autoSizeHeightSpace -= rh;
            data.size = rh;
            data.minSize = rh;
            data.maxSize = rh;
        }
        else
        {
            float minSize(0.0f);
            float maxSize(0.0f);
            for (uint8 c(0); c < m_Columns; ++c)
            {
                ILayoutable* obj(m_Grid[r * m_Columns + c]);
                if (obj && obj != GRID_SPAN_POINTER)
                {
                    const he::vec4& pad(obj->getLayoutPadding());
                    const float objMin(obj->getLayoutMinSize().y + pad.y + pad.w);
                    const float objMax(obj->getLayoutMaxSize().y + pad.y + pad.w);
                    minSize = std::max(minSize, objMin);
                    maxSize = std::max(minSize, std::max(maxSize, objMax));
                }
            }
            data.size = minSize;
            data.minSize = minSize;
            data.maxSize = maxSize;
            autoSizeHeightSpace -= minSize;
        }
    }
    if (autoSizeHeightSpace > 0)
    {
        ResizeData(data, m_Rows, autoSizeHeightSpace);
    }
    // else
    //{
    //    clipping.. sowwy :3
    //}
}

void GridLayout::calculateColumnWidths( LayoutData* data )
{
    float autoSizeWidthSpace(m_LayoutBound.width - m_Margin.x - m_Margin.z);
    // Get all column width
    for (uint8 c(0); c < m_Columns; ++c)
    {
        LayoutData& data(data[c]);
        const float cw(m_ColumnWidth[c]);
        if (cw >= 0.0f)
        {
            autoSizeWidthSpace -= cw;
            data.size = cw;
            data.minSize = cw;
            data.maxSize = cw;
        }
        else
        {
            float minSize(0.0f);
            float maxSize(FLT_MAX);
            for (uint8 r(0); r < m_Rows; ++r)
            {
                ILayoutable* obj(m_Grid[r * m_Columns + c]);
                if (obj && obj != GRID_SPAN_POINTER)
                {
                    const he::vec4& pad(obj->getLayoutPadding());
                    const float objMin(obj->getLayoutMinSize().x + pad.x + pad.z);
                    const float objMax(obj->getLayoutMaxSize().x + pad.x + pad.z);
                    minSize = std::max(minSize, objMin);
                    maxSize = std::max(minSize, std::max(maxSize, objMax));
                }
            }
            data.size = minSize;
            data.minSize = minSize;
            data.maxSize = maxSize;
            autoSizeWidthSpace -= minSize;
        }
    }
    if (autoSizeWidthSpace > 0)
    {
        ResizeData(data, m_Columns, autoSizeWidthSpace);
    }
    // else
    //{
    //    clipping.. sowwy :3
    //}
}

void GridLayout::performLayout()
{
    DefaultLayoutable::performLayout();

    LayoutData* rowData(HENewArray(LayoutData, m_Rows));
    LayoutData* columnData(HENewArray(LayoutData, m_Columns));

    calculateRowHeights(rowData);
    calculateColumnWidths(columnData);

    float y(m_LayoutBound.y + m_Margin.y);
    for (uint8 r(0); r < m_Rows; ++r)
    {
        const LayoutData& rData(rowData[r]);
        const float height(rData.size);
        float width(0.0f);
        float x(m_LayoutBound.x + m_Margin.x);
        for (uint8 c(0); c < m_Columns; ++c)
        {
            const LayoutData& cData(columnData[c]);
            width += cData.size;

            ILayoutable* obj(m_Grid[r * m_Columns + c]);
            if (obj != GRID_SPAN_POINTER)
            {
                if (obj != nullptr)
                {
                    const he::vec4& padding(obj->getLayoutPadding());
                    const he::vec2& minSize(obj->getLayoutMinSize());
                    const he::vec2& maxSize(obj->getLayoutMaxSize());

                    he::vec2 size(width, height);
                    size.x = std::max(std::min(width - padding.x - padding.z, maxSize.x), minSize.x);
                    size.y = std::max(std::min(height - padding.y - padding.w, maxSize.y), minSize.y);

                    he::vec2 pos(0, 0);
                    switch (obj->getLayoutHAlignment())
                    {
                    case eLayoutHAlignment_Left: pos.x = x + padding.x; break;
                    case eLayoutHAlignment_Center: pos.x = x + width / 2.0f - size.x / 2.0f; break;
                    case eLayoutHAlignment_Right: pos.x = x + width - size.x - padding.z; break;
                    }
                    switch (obj->getLayoutVAlignment())
                    {
                    case eLayoutVAlignment_Top: pos.y = y + padding.y; break;
                    case eLayoutVAlignment_Center: pos.y = y + height / 2.0f - size.y / 2.0f; break;
                    case eLayoutVAlignment_Bottom: pos.y = y + height - size.y - padding.w; break;
                    }
                    he::RectF rect(pos.x, pos.y, size.x, size.y);
                    obj->setLayoutBound(rect);
                    he::RectF clipRect(x, y, width, height);
                    clipClipRect(m_LayoutClipBound, clipRect);
                    obj->setLayoutClipBound(clipRect);
                    obj->performLayout();
                }
                x += width;
                width = 0;
            }
        }
        y += height;
    }

    HEDeleteArray(rowData);
    HEDeleteArray(columnData);
}

void GridLayout::setLayoutDirty()
{
    DefaultLayoutable::setLayoutDirty();
}

} } //end namespace
