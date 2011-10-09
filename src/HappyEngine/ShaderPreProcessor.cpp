//HappyEngine Copyright (C) 2011  Bastian Damman, Sebastiaan Sprengers
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

#include "ShaderPreProcessor.h"
#include "HappyNew.h"
#include "HappyTypes.h"

#include "FileReader.h"
#include <sstream>

namespace happyengine {
namespace content {
namespace details {

std::string ShaderPreProcessor::process(const std::string& filepath, const std::set<std::string>& defines)
{
    io::FileReader reader;
    reader.open(filepath, io::FileReader::OpenType_ASCII);

    std::stringstream stream;

    uint ifblocks = 0;
    uint discardBlock = 0;
    while(reader.eof() == false)
    {
        std::string line(reader.readLine());
        
        if (line.find("#endif") != std::string::npos)
        {
            --ifblocks;
            if (discardBlock > ifblocks)
                discardBlock = 0;
        }
        else if (line.find("#if ") != std::string::npos)
        {
            ++ifblocks;
            if (discardBlock == 0 && defines.find(line.substr(4)) == defines.cend())
            {
                discardBlock = ifblocks;
            }
        }
        else
        {
            if (discardBlock != 0)
                continue;
            if (line.find("#include ") != std::string::npos)
            {
                std::string fName(line.substr(10, line.length() - 11));
                std::string fPath(filepath.substr(0, filepath.rfind("/") + 1));
                stream << ShaderPreProcessor::process(fPath + fName, defines);
            }
            else
                stream << line << "\n";
        }
    }

    reader.close();

    return stream.str();
}

} } } //end namespace