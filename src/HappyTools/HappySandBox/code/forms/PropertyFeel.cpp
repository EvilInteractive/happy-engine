#include "HappySandBoxPCH.h"
#include "PropertyFeel.h"

namespace hs {

PropertyFeel::PropertyFeel()
    : m_Dirty(false)
{

}

void PropertyFeel::setDirty( const bool dirty )
{
    if (m_Dirty != dirty)
    {
        m_Dirty = dirty;
        onDirtyChanged(dirty);
    }
}

}