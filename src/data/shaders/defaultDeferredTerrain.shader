[Shader]
vsPath = "deferred/pre/deferredPreShader.vert"
fsPath = "deferred/pre/deferredPreTerrainShader.frag"

[inPerVertex]
inPosition = POSITION
inTexCoord = TEXCOORD
inNormal = NORMAL
inTangent = TANGENT

[uniform]
matWVP = WORLDVIEWPROJECTION
matWorldView = WORLDVIEW
diffuseMap = TEXTURE2D
normalMap = TEXTURE2D
specGlossIllMap = TEXTURE2D
diffuseSpecTiling = VEC2

[out]
outColor = GBUFFER_COLOR
outSGI = GBUFFER_SGI
outNormal = GBUFFER_NORMAL
