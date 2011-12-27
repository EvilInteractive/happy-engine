[Shader]
vsPath = "deferred/pre/deferredPreShaderInstanced.vert"
fsPath = "deferred/pre/deferredPreShader.frag"

[info]
translucent = false

[in]
inPosition = POSITION
inTexCoord = TEXCOORD
inNormal = NORMAL
inTangent = TANGENT
inWorld = WORLD

[uniform]
matVP = VIEWPROJECTION
diffuseMap = TEXTURE2D
normalMap = TEXTURE2D
specGlossIllMap = TEXTURE2D

[out]
outColor = GBUFFER_COLOR
outSGI = GBUFFER_SGI
outNormal = GBUFFER_NORMAL
