[Shader]
vsPath = "deferred/pre/deferredPreShaderSkinned.vert"
fsPath = "deferred/pre/deferredPreShader.frag"

[inPerVertex]
inPosition = POSITION
inTexCoord = TEXCOORD
inNormal = NORMAL
inTangent = TANGENT
inBoneId = BONEIDS
inBoneWeight = BONEWEIGHTS

[uniform]
matWVP = WORLDVIEWPROJECTION
matBones = BONETRANSFORMS
matWorldView = WORLDVIEW
diffuseMap = TEXTURE2D
normalMap = TEXTURE2D
specGlossIllMap = TEXTURE2D

[out]
outColor = GBUFFER_COLOR
outSGI = GBUFFER_SGI
outNormal = GBUFFER_NORMAL
