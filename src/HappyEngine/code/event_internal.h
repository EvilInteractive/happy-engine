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
//Created: 28/10/2012

#define CONCAT(a, b) a##b
#define CONCAT_EXT(a, b) CONCAT(a, b)
#define EVALUATE(x) x
#define EVENTCALLBACK CONCAT_EXT(eventCallback, ARGS)
#define EVENT CONCAT_EXT(event, ARGS)
#define EVENTBASE CONCAT_EXT(eventBase, ARGS)

#ifdef TEMPLATE_EXTRA_ARGS
#define COMMA ,
#else
#define TEMPLATE_EXTRA_ARGS
#define COMMA
#endif

#ifndef DECL_PARAMS
#define DECL_PARAMS
#endif

#ifndef PARAMS
#define PARAMS
#endif

//////////////////////////////////////////////////////////////////////////
// eventCallback
//////////////////////////////////////////////////////////////////////////
template<typename ReturnType COMMA TEMPLATE_EXTRA_ARGS> class EVENT;
namespace details {
template<typename ReturnType COMMA TEMPLATE_EXTRA_ARGS> class EVENTBASE;
}
template<typename ReturnType COMMA TEMPLATE_EXTRA_ARGS>
struct EVENTCALLBACK
{
    template<typename ReturnType COMMA TEMPLATE_EXTRA_ARGS> friend class EVENT;
    template<typename ReturnType COMMA TEMPLATE_EXTRA_ARGS> friend class details::EVENTBASE;
    typedef typename CONCAT_EXT(boost::function,ARGS)<ReturnType COMMA TEMPLATE_EXTRA_ARGS> Function;

public:
    EVENTCALLBACK(): m_Connection(UINT16_MAX) {}
    explicit EVENTCALLBACK(const Function& Function):
    m_Function(Function), m_Connection(UINT16_MAX) {}

private:
    Function m_Function;
    uint16 m_Connection;
};

//////////////////////////////////////////////////////////////////////////
// event
//////////////////////////////////////////////////////////////////////////
namespace details
{
    template<typename ReturnType COMMA TEMPLATE_EXTRA_ARGS>
    class EVENTBASE
    {
    public:
        typedef EVENTCALLBACK<ReturnType COMMA TEMPLATE_EXTRA_ARGS> Callback;
        typedef typename Callback::Function Function;

        EVENTBASE() {}
        ~EVENTBASE() {}

        void operator+=(Callback& func)
        {
            add(func);
        }
        void operator-=(const Callback& func)
        {
            remove(func);
        }
        void add(Callback& func)
        {
            HE_IF_ASSERT(func.m_Connection == UINT16_MAX, "Callback already connected to an EVENTBASE! Ignoring connection")
            {
                if (m_FreeConnections.empty())
                    enlargePool();
                uint16 con(m_FreeConnections.front());
                m_FreeConnections.pop();
                func.m_Connection = con;
                m_Connections.push_back(con);
                m_CallbackPool[con] = func.m_Function;
            }
        }
        void enlargePool()
        {
            const uint16 step = 1;                          
            HE_ASSERT(m_CallbackPool.size() + step < UINT16_MAX, "Fatal error - to many connected EVENTBASEs!");        
            for (uint16 i(static_cast<uint16>(m_CallbackPool.size())); i < m_CallbackPool.size() + step; ++i)            
                m_FreeConnections.push(i);
            m_CallbackPool.resize(m_CallbackPool.size() + step);
        }
        void remove(const Callback& func)
        {
            if (func.m_Connection == UINT16_MAX)
                return;
            std::vector<uint16>::iterator it(std::find(m_Connections.begin(), m_Connections.end(), func.m_Connection));
            HE_IF_ASSERT(it != m_Connections.end(), "Callback not connected to this EVENTBASE!")
            {
                m_FreeConnections.push(*it);
                *it = m_Connections.back();
                m_Connections.pop_back();
            }
        }
        bool empty() const { return m_Connections.empty(); }

        void clear()
        {
            std::for_each(m_Connections.cbegin(), m_Connections.cend(), [this](uint16 c)
            {
                m_FreeConnections.push(c);
            });
            m_Connections.clear();
        }

    protected:
        std::vector<Function> m_CallbackPool;
        std::vector<uint16> m_Connections;
        std::queue<uint16> m_FreeConnections;

        EVENTBASE(const EVENTBASE&);
        EVENTBASE& operator=(const EVENTBASE&);
    };
}
template<typename ReturnType COMMA TEMPLATE_EXTRA_ARGS>
class EVENT : public details::EVENTBASE<ReturnType COMMA TEMPLATE_EXTRA_ARGS>
{
public:
    typedef boost::function2<bool, ReturnType&, const ReturnType&> EventCombiner;

    EVENT(): 
        m_Combiner(&details::defaultEventCombiner<ReturnType>),
        m_DefaultValue() {}
    EVENT(const EventCombiner& combiner, const ReturnType& defaultValue):
        m_Combiner(combiner),
        m_DefaultValue(defaultValue) {}
    ~EVENT() {}

    ReturnType operator()(DECL_PARAMS) const
    {
        return execute(PARAMS);
    }
    ReturnType execute(DECL_PARAMS) const
    {
        std::vector<uint16>::const_iterator it(m_Connections.cbegin());
        std::vector<uint16>::const_iterator last(m_Connections.cend());

        if (it == last)
            return m_DefaultValue;

        ReturnType outValue(m_CallbackPool[*it](PARAMS));
        while (++it != last && !m_Combiner(outValue, m_CallbackPool[*it](PARAMS)));

        return outValue;
    }

private:
    EventCombiner m_Combiner;
    ReturnType m_DefaultValue;

    EVENT(const EVENT&);
    EVENT& operator=(const EVENT&);
};
template<TEMPLATE_EXTRA_ARGS>
class EVENT<void COMMA TEMPLATE_EXTRA_ARGS> : public details::EVENTBASE<void COMMA TEMPLATE_EXTRA_ARGS>
{
public:
    EVENT() {}
    ~EVENT() {}

    void operator()(DECL_PARAMS) const
    {
        return execute(PARAMS);
    }

    void execute(DECL_PARAMS) const
    {
        std::vector<uint16>::const_iterator it(m_Connections.cbegin());
        std::vector<uint16>::const_iterator last(m_Connections.cend());

        if (it == last)
            return;

        m_CallbackPool[*it](PARAMS);
        while (++it != last)
            m_CallbackPool[*it](PARAMS);
    }

private:
    EVENT(const EVENT&);
    EVENT& operator=(const EVENT&);
};

#undef ARGS
#undef COMMA
#undef TEMPLATE_EXTRA_ARGS
#undef DECL_PARAMS
#undef PARAMS
