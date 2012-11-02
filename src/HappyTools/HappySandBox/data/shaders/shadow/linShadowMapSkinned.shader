[Shader]
vsPath = "shadow/preShadowShaderSkinned.vert"
fsPath = "shadow/preShadowShader.frag"

[inPerVertex]
inPosition = POSITION
inBoneId = BONEIDS
inBoneWeight = BONEWEIGHTS

[uniform]
matWVP = WORLDVIEWPROJECTION
matBones = BONETRANSFORMS
matWV = WORLDVIEW
lightNearFar = NEARFAR
