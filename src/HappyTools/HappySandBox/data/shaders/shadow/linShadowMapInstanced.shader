[Shader]
vsPath = "shadow/preShadowShaderInstanced.vert"
fsPath = "shadow/preShadowShader.frag"

[inPerVertex]
inPosition = POSITION

[inPerInstance]
inWorld = MAT44

[uniform]
matV = VIEW
matVP = VIEWPROJECTION
lightNearFar = NEARFAR
