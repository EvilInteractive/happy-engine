[Shader]
vsPath = "deferred/pre/deferredPreShader.vert"
fsPath = "deferred/pre/deferredPreShader.frag"

[in]
inPosition = POSITION
inTexCoord = TEXCOORD
inNormal = NORMAL
inTangent = TANGENT

[uniform]
matWVP = WORLDVIEWPROJECTION
matWorld = WORLD
diffuseMap = TEXTURE2D
normalMap = TEXTURE2D
specGlossIllMap = "TEXTURE2D

[out]
outColorIll = GBUFFER_COLOR_ILLUMINATION
outPosSpec = GBUFFER_POSITION_SPECULAR
outNormalGloss = GBUFFER_NORMAL_GLOSSINESS
