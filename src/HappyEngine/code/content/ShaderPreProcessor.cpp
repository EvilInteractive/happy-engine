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

std::string trimEnd(const std::string& str)
{
    int i(str.size() - 1);
    for (; i >= 0; --i)
    {
        if (str[i] != ' ')
        {
            break;
        }
    }
    return str.substr(0, i + 1);
}
std::string ShaderPreProcessor::process(const std::string& code, const std::set<std::string>& defines)
{
    std::stringstream stream;

    std::stringstream linesStream;
    std::vector<std::string> lines;
    std::for_each(code.cbegin(), code.cend(), [&](const char& c)
    {
        if (c == '\n')
        {
            lines.push_back(linesStream.str());
            linesStream.clear();
            linesStream.str("");
        }
        else
        {
            linesStream << c;
        }
    });

    uint ifblocks = 0;
    uint discardBlock = 0;
    std::for_each(lines.cbegin(), lines.cend(), [&](const std::string& line)
    {        
        if (line.find("#endif") != std::string::npos)
        {
            --ifblocks;
            if (discardBlock > ifblocks)
                discardBlock = 0;
        }
        else if (line.find("#if ") != std::string::npos)
        {
            ++ifblocks;
            if (discardBlock == 0 && defines.find(trimEnd(line.substr(line.find("#if ") + 4))) == defines.cend())
            {
                discardBlock = ifblocks;
            }
        }
        else
        {
            if (discardBlock == 0)
            {
                if (line.find("#include ") != std::string::npos)
                {
                    std::string includeRelativePath(CONTENT->getRootDir() + CONTENT->getShaderFolder());
                    HE_ASSERT(includeRelativePath.back() == '/', "includeRelativePath does not end with trailing slash");
                    std::string fName(line.substr(10, line.length() - 11));

                    io::FileReader reader;

                    if (reader.open(includeRelativePath + fName, io::FileReader::OpenType_ASCII)) 
                    {
                        std::string str;
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
                    stream << line << "\n";
            }
        }
    });
    return stream.str();
}

} } } //end namespace