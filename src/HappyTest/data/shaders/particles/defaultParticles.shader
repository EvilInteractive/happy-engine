[Shader]
vsPath = "particles/particles.vert"
fsPath = "particles/particles.frag"

[info]
translucent = true

[inPerVertex]
inPosition = POSITION

[inPerInstance]
inBlendColor = VEC4
inUvTile = VEC2
inWorld = MAT44

[uniform]
matVP = VIEWPROJECTION
diffuseMap = TEXTURE2D
uvTiles = VEC2

[out]
outColor = GBUFFER_COLOR
outColor = GBUFFER_NORMAL
