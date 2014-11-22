//HappyEngine Copyright (C) 2012 - 2014 Evil Interactive                         
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
//This file was was generated with the HappyMaterialEditor                       
                                                                                 
#version 150 core                                                                
#pragma optionNV(fastmath on)                                                    
//#pragma optionNV(fastprecision on)                                             
#pragma optionNV(ifcvt none)                                                     
#pragma optionNV(inline all)                                                     
#pragma optionNV(strict on)                                                      
#pragma optionNV(unroll all)                                                     
                                                                                 
#include "packing/encode.frag"
#include "shared/perCameraUniformBuffer.frag"

in vec3 passNormal;
in float passDepth;

void main()
{
    outColor = vec4(vec3(0.5, 1, 0.25), 0);
    
    
    outSG = vec4(vec3(0, 0, 0), 1);
    
    
    outNormalDepth = vec3(encodeNormal(passNormal), ((passDepth - perCameraUniformBuffer.cameraNearFar.x) / (perCameraUniformBuffer.cameraNearFar.y - perCameraUniformBuffer.cameraNearFar.x)));
    
    
    
}


