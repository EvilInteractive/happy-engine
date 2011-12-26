[Shader]
vsPath = "forward/forwardShaderSkinned.vert"
fsPath = "forward/forwardShader.frag"

[in]
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

mtxDirLight0 = SHADOW_CASCADE_MATRIX0
mtxDirLight1 = SHADOW_CASCADE_MATRIX1
mtxDirLight2 = SHADOW_CASCADE_MATRIX2
mtxDirLight3 = SHADOW_CASCADE_MATRIX3

shadowMap0 = SHADOW_CASCADE0
shadowMap1 = SHADOW_CASCADE1
shadowMap2 = SHADOW_CASCADE2
shadowMap3 = SHADOW_CASCADE3

[out]
outColor = GBUFFER_COLOR
outNormal = GBUFFER_NORMAL
