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
#include "HappySandBoxPCH.h"
#include "NodeGraphNodeConnector.h"

namespace hs {

NodeGraphNodeConnector::NodeGraphNodeConnector()
    : m_Flags(eFlags_NeedsLayoutUpdate)
    , m_State(eConnectorState_Normal)
    , m_ConnectorBound(0, 0, 8, 8)
{

}

NodeGraphNodeConnector::~NodeGraphNodeConnector()
{

}

bool NodeGraphNodeConnector::isInsideConnector( const he::vec2& worldPos ) const 
{
    return m_ConnectorBound.isInside(worldPos);
}

void NodeGraphNodeConnector::draw( const NodeGraphDrawContext& /*context*/ )
{

}

} //end namespace

