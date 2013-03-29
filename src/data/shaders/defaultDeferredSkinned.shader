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
camNearFar = NEARFAR

[out]
outColor = GBUFFER_COLOR
outSG = GBUFFER_SG
outNormalDepth = GBUFFER_NORMALDEPTH
