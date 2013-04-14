[Shader]
vsPath = "deferred/pre/deferredPreShader.vert"
fsPath = "deferred/pre/deferredPreShader.frag"

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
camNearFar = NEARFAR

[out]
outColor = GBUFFER_COLOR
outSG = GBUFFER_SG
outNormalDepth = GBUFFER_NORMALDEPTH
