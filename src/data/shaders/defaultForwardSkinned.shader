[Shader]
vsPath = "forward/forwardShaderSkinned.vert"
fsPath = "forward/forwardShader.frag"

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
matWV = WORLDVIEW
diffuseMap = TEXTURE2D
normalMap = TEXTURE2D
specGlossIllMap = TEXTURE2D

ambLight.color = AMBIENT_COLOR
dirLight.color = DIRECTIONAL_COLOR
dirLight.direction = DIRECTIONAL_DIRECTION

[out]
outColor = GBUFFER_COLOR
outNormal = GBUFFER_NORMALDEPTH
