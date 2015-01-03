//HappyEngine Copyright (C) 2011 - 2015  Evil Interactive
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
//Created: 2015/01/03

#ifndef Options_h__
#define Options_h__

#include <FixedStringMap.h>
#include <Singleton.h>

namespace he {
namespace io {
    class StructuredVisitor;
}
namespace ge {
    struct PropertyDesc;
} }

namespace hs {

class Options : public he::Singleton<Options>
{
public:
    typedef he::FixedStringMap<he::ge::PropertyDesc*> TOptionList;

    Options();
    ~Options();

    void save();
    void load();

    const TOptionList& getOptions() const { return m_Options; }
    const he::Path& getPathOption(const he::FixedString& name) const;

private:
    void visit(he::io::StructuredVisitor* visitor);

    void addPathProperty(const he::FixedString& name, const char* displayName, const char* tooltip, he::Path defaultValue, const bool isDirectory, const he::String& filter = he::String());

    TOptionList m_Options;

    Options(const Options&);
    Options& operator=(const Options&);
};

}

#endif // Options_h__
