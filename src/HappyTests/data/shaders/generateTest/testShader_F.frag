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




void main() 
{
passNormal = (matWorldView * vec4(inNormal, 0)).xyz;
gl_Position = (matProj * ((matView * inWorld) * vec4((inPosition + vec3(0, 0, 0)), 1)));
}

