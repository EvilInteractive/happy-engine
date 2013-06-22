//HappyEngine Copyright (C) 2011 - 2012  Evil Interactive
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
//Created: 13/03/2013

_DEF_FS(str, "");

DEF_FS(Array);

DEF_FS(X);
DEF_FS(Y);
DEF_FS(Z);
DEF_FS(W);

DEF_FS(Error);

//////////////////////////////////////////////////////////////////////////
// Components
//////////////////////////////////////////////////////////////////////////
DEF_FS(Entity);
DEF_FS(CharacterPhysicsComponent);
DEF_FS(DynamicPhysicsComponent);
DEF_FS(PointLightComponent);
DEF_FS(SpotLightComponent);
DEF_FS(ModelComponent);
DEF_FS(PickingComponent);
DEF_FS(StaticPhysicsComponent);
DEF_FS(TriggerComponent);

//////////////////////////////////////////////////////////////////////////
// Component Properties
//////////////////////////////////////////////////////////////////////////
DEF_FS(Translate);
DEF_FS(Rotate);
DEF_FS(Scale);

DEF_FS(Multiplier);
DEF_FS(Attenuation);
DEF_FS(Color);
DEF_FS(Fov);
DEF_FS(Direction);
DEF_FS(ShadowResolution);

DEF_FS(Model);
DEF_FS(Material);

DEF_FS(Version);
DEF_FS(Nodes);
DEF_FS(Type);
DEF_FS(Connections);
DEF_FS(Position);
DEF_FS(ID);
DEF_FS(Index);
DEF_FS(From);
DEF_FS(FromOutput);
DEF_FS(To);
DEF_FS(ToInput);

//////////////////////////////////////////////////////////////////////////
// Shader
//////////////////////////////////////////////////////////////////////////
DEF_FS(dTime);
DEF_FS(prevLumMap);
DEF_FS(hdrMap);
DEF_FS(cubeMap);

DEF_FS(Name);
DEF_FS(AttributeType);
DEF_FS(Attributes);
DEF_FS(UniformType);
DEF_FS(VariableType);
DEF_FS(DefaultValue);
DEF_FS(Uniforms);

//////////////////////////////////////////////////////////////////////////
// Material
//////////////////////////////////////////////////////////////////////////
DEF_FS(Add);
DEF_FS(Subtract);
DEF_FS(ReverseSubtract);
DEF_FS(Min);
DEF_FS(Max);
DEF_FS(Zero);
DEF_FS(One);
DEF_FS(SrcColor);
DEF_FS(OneMinSrcColor);
DEF_FS(DestColor);
DEF_FS(OneMinDestColor);
DEF_FS(SrcAlpha);
DEF_FS(OneMinSrcAlpha);
DEF_FS(DestAlpha);
DEF_FS(OneMinDestAlpha);
DEF_FS(SrcAlphaSaturate);
DEF_FS(Shader);
DEF_FS(IsBlended);
DEF_FS(NoPost);
DEF_FS(CullFrontFace);
DEF_FS(DepthRead);
DEF_FS(DepthWrite);
DEF_FS(BlendEquation);
DEF_FS(SourceBlend);
DEF_FS(DestBlend);

//////////////////////////////////////////////////////////////////////////
// Config
//////////////////////////////////////////////////////////////////////////
DEF_FS(HDRSettings);
DEF_FS(ExposureSpeed);
DEF_FS(AOSettings);
DEF_FS(Radius);
DEF_FS(MaxDistance);
DEF_FS(ShaderSettings);
DEF_FS(EnableHDR);
DEF_FS(EnableAO);
DEF_FS(EnableBloom);
DEF_FS(EnableNormalEdgeDetect);
DEF_FS(EnableDepthEdgeDetect);
DEF_FS(EnableVignette);
DEF_FS(EnableFog);
DEF_FS(PostSettings);
DEF_FS(ShadowSettings);
DEF_FS(ShadowQuality);
DEF_FS(EnableLighting);
DEF_FS(EnableSpecular);
DEF_FS(EnableNormalMaps);
DEF_FS(EnableShadows);
DEF_FS(LightingSettings);
DEF_FS(RenderSettings);
DEF_FS(EnableDeferredRendering);
DEF_FS(EnablePostEffects);
DEF_FS(EnableOculusRift);
