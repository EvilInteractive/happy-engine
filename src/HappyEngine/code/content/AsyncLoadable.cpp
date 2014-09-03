//HappyEngine Copyright (C) 2011 - 2014  Bastian Damman, Sebastiaan Sprengers
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
//Created: 2014/02/09
#include "HappyPCH.h"
#include "AsyncLoadable.h"

namespace he {

struct AsyncLoadable::LoadContext
{
public:
    typedef he::ObjectList<std::pair<const void*, LoadCallback>> LoadCallbackList;

    LoadContext() {}
    ~LoadContext()
    {
        HE_ASSERT(m_LoadCallbacks.empty(), "Loadcallbacks are not empty when the LoadContext is destroyed!");
    }

    LoadCallbackList m_LoadCallbacks;

private:
    LoadContext(const LoadContext&);
    LoadContext& operator=(const LoadContext&);
};

AsyncLoadable::AsyncLoadable()
    : m_IsLoaded(eLoadResult_Unloaded)
    , m_LoadContext(NEW LoadContext())
{

}

AsyncLoadable::~AsyncLoadable()
{
    delete m_LoadContext;
}

void AsyncLoadable::setLoaded( const ELoadResult result )
{
    GUARD(he::eThreadTicket_Main); // To avoid problems, callbacks should always be executed on the main thread
    if (m_IsLoaded == eLoadResult_Unloaded)
    {
        m_IsLoaded = result;
        HE_ASSERT(m_LoadContext != nullptr, "LoadContext == nulltpr!");
        LoadContext::LoadCallbackList& list(m_LoadContext->m_LoadCallbacks);
        list.forEach([result](const std::pair<const void*, LoadCallback>& p)
        {
            p.second(result);
        });
        list.clear();
        delete m_LoadContext;
        m_LoadContext = nullptr;
    }
}

void AsyncLoadable::callbackOnceIfLoaded( const void* const id, const LoadCallback& callback ) const
{
    GUARD(he::eThreadTicket_Main);
    if (m_IsLoaded != eLoadResult_Unloaded)
    {
        callback(m_IsLoaded);
    }
    else
    {
        HE_ASSERT(m_LoadContext != nullptr, "LoadContext == nulltpr!");
        m_LoadContext->m_LoadCallbacks.add(std::make_pair(id, callback));
    }
}

void AsyncLoadable::cancelLoadCallback( const void* const id ) const
{
    GUARD(he::eThreadTicket_Main);
    if (m_IsLoaded == eLoadResult_Unloaded)
    {
        HE_ASSERT(m_LoadContext != nullptr, "LoadContext == nulltpr!");
        LoadContext::LoadCallbackList& list(m_LoadContext->m_LoadCallbacks);
        if (list.empty() == false)
        {
            for (size_t i(0); i < list.size();)
            {
                const std::pair<const void*, LoadCallback>& p(list[i]);
                if (p.first == id)
                {
                    list.removeAt(i); // No break, we need to remove all instances of id
                }
                else
                {
                    ++i;
                }
            }
        }
    }
}

} //end namespace

