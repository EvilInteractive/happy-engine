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

#ifndef _HE_AsyncLoadable_H_
#define _HE_AsyncLoadable_H_
#pragma once

namespace he {

class AsyncLoadable
{
    struct LoadContext;
public:
    typedef std::function<void(const ELoadResult result)> LoadCallback;

    AsyncLoadable();
    virtual ~AsyncLoadable();

    bool isLoaded() const { return m_IsLoaded == eLoadResult_Success; }
    ELoadResult getLoadResult() const { return m_IsLoaded; }
    void callbackOnceIfLoaded(const void* const id, const LoadCallback& callback) const;
    void cancelLoadCallback(const void* const id) const;
    void setLoaded(const ELoadResult result);

private:
    ELoadResult m_IsLoaded;
    LoadContext* m_LoadContext;

    // disabled assignment operator
    AsyncLoadable& operator=(const AsyncLoadable&);
    AsyncLoadable(const AsyncLoadable&);
};

} //end namespace

#endif
