[Shader]
vsPath = "forward/forwardShaderInstanced.vert"
fsPath = "forward/forwardShader.frag"

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

ambLight.color = AMBIENT_COLOR
dirLight.color = DIRECTIONAL_COLOR
dirLight.direction = DIRECTIONAL_DIRECTION

[out]
outColor = GBUFFER_COLOR
outNormal = GBUFFER_NORMALDEPTHS
