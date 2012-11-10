[Shader]
vsPath = "forward/skybox.vert"
fsPath = "forward/skybox.frag"

[inPerVertex]
inPosition = POSITION

[uniform]
matVP = VIEWPROJECTION
cubeMap = TEXTURECUBE

[out]
outColor = GBUFFER_COLOR
