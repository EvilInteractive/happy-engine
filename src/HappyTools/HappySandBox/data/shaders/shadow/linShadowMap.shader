[Shader]
vsPath = "shadow/preShadowShader.vert"
fsPath = "shadow/preShadowShader.frag"

[inPerVertex]
inPosition = POSITION

[uniform]
matWVP = WORLDVIEWPROJECTION
matWV  = WORLDVIEW
lightNearFar = NEARFAR
