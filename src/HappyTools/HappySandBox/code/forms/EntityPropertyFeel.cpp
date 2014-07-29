#include "HappySandBoxPCH.h"
#include "EntityPropertyFeel.h"

namespace hs {

EntityPropertyFeel::EntityPropertyFeel()
    : m_Dirty(false)
{

}

void EntityPropertyFeel::setDirty( const bool dirty )
{
    if (m_Dirty != dirty)
    {
        m_Dirty = dirty;
        onDirtyChanged(dirty);
    }
}

}