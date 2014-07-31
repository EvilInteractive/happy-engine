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
//Created: 08/10/2011
#include "HappyPCH.h" 

#include "ShaderPreProcessor.h"
#include "HappyNew.h"
#include "HappyTypes.h"

#include "FileReader.h"
#include <sstream>

#include "ContentManager.h"

namespace he {
namespace ct {
namespace details {

he::String trimEnd(const he::String& str)
{
    int32 i(static_cast<int32>(str.size()) - 1);
    for (; i >= 0; --i)
    {
        if (str[i] != ' ')
        {
            break;
        }
    }
    return str.substr(0, i + 1);
}
he::String trimBegin(const he::String& str)
{
    const size_t size(str.size());
    size_t i(0);
    for (; i < size; ++i)
    {
        if (str[i] != ' ')
        {
            break;
        }
    }
    return str.substr(i);
}

he::String ShaderPreProcessor::process(const he::String& code, he::ObjectList<he::String>& defines)
{
    std::stringstream stream;

    std::stringstream linesStream;
    he::ObjectList<he::String> lines;
    std::for_each(code.cbegin(), code.cend(), [&](const char& c)
    {
        if (c == '\n')
        {
            lines.add(linesStream.str());
            linesStream.clear();
            linesStream.str("");
        }
        else
        {
            linesStream << c;
        }
    });

    uint32 ifblocks = 0;
    uint32 discardBlock = 0;
    lines.forEach([&](const he::String& line)
    {        
        if (line.find("#endif") != he::String::npos)
        {
            --ifblocks;
            if (discardBlock > ifblocks)
                discardBlock = 0;
        }
        else if (line.find("#if ") != he::String::npos)
        {
            ++ifblocks;
            size_t index;
            if (discardBlock == 0 && !defines.find(trimEnd(line.substr(line.find("#if ") + 4)), index))
            {
                discardBlock = ifblocks;
            }
        }
        else
        {
            if (discardBlock == 0)
            {
                if (line.find("#define") != he::String::npos)
                {
                    he::String defineName(line.substr(7));
                    defineName = trimBegin(defineName);
                    defineName = trimEnd(defineName);
                    if (!defines.contains(defineName))
                        defines.add(defineName);
                }
                else if (line.find("#include ") != he::String::npos)
                {
                    const he::String& includeRelativePath(CONTENT->getShaderFolderPath().str());
                    HE_ASSERT(includeRelativePath.back() == '/', "includeRelativePath does not end with trailing slash");
                    he::String fName(line.substr(10, line.length() - 11));

                    io::FileReader reader;

                    if (reader.open(includeRelativePath + fName, io::FileReader::OpenType_ASCII)) 
                    {
                        he::String str;
                        str = reader.readToEnd();
                        reader.close();

                        stream << process(str, defines);
                        reader.close();
                    }
                    else
                    {
                        HE_ERROR("Error when preprocessing shader: including %s%s failed", includeRelativePath.c_str(), fName.c_str());
                    }

                }
                else
                {
                    stream << line << "\n";
                }
            }
        }
    });
    return stream.str();
}

} } } //end namespace