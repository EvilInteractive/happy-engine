[Shader]
vsPath = "deferred/pre/deferredPreShaderInstanced.vert"
fsPath = "deferred/pre/deferredPreShader.frag"

[inPerVertex]
inPosition = POSITION
inTexCoord = TEXCOORD
inNormal = NORMAL
inTangent = TANGENT

[inPerInstance]
inWorld = MAT44

[uniform]
matV = VIEW
matVP = VIEWPROJECTION
diffuseMap = TEXTURE2D
normalMap = TEXTURE2D
specGlossIllMap = TEXTURE2D

[out]
outColor = GBUFFER_COLOR
outSGI = GBUFFER_SGI
outNormal = GBUFFER_NORMAL
