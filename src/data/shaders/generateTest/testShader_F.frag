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
    if (0 < 0.5)
    {
        discard;
    }
    
    
}

