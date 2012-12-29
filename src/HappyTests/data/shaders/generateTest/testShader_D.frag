//HappyEngine Copyright (C) 2012 Evil Interactive                                
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
                                                                                 
in vec3 passNormal;
in vec3 passTangent;
in float passDepth;

uniform vec2 camNearFar;

out vec4 outColor;
out vec3 outNormalDepth;
out vec4 outSG;

vec3 calcNormal(in vec3 normal, in vec3 tangent, in vec3 rgb)            
{                                                                        
    //NormalMap                                                          
    tangent = normalize(tangent);                                        
    normal = normalize(normal);                                          
                                                                         
    vec3 binormal = cross(tangent, normal);                              
                                                                         
    mat3 assenstelsel = mat3(binormal, tangent, normal);                 
                                                                         
    vec3 xyz = vec3(rgb.x * 2 - 1, (1 - rgb.y) * 2 - 1, rgb.z * 2 - 1);  
                                                                         
    return normalize(assenstelsel * xyz);                                
}                                                                        
                                                                         
vec2 encodeNormal(in vec3 normal)                                        
{                                                                        
    float f = sqrt(-normal.z * 8.0f + 8.0f);                             
    return normal.xy / f + 0.5f;                                         
}                                                                        
                                                                         

void main()
{
    float localvar5 = (1 - 0);
    float localvar7 = pow(localvar5, 0);
    if (localvar7 < 0.5)
    {
        discard;
    }
    
    const vec3 localvar0 = vec3(0, 0, 0);
    float localvar16 = ((passDepth - camNearFar.x) / (camNearFar.y - camNearFar.x));
    vec3 localvar21 = encodeNormal(calcNormal(passNormal, passTangent, (localvar7 * vec3(0, 0, 0))));
    outColor = vec4(localvar0, 1);
    outNormalDepth = vec3(localvar21, localvar16);
    outSG = vec4(localvar0, 0);
    
}

