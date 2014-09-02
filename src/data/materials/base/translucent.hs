{
    "Normal" : {
        "Normal" : {
            "VertexShader" : "forward/forwardShader.vert",
            "FragmentShader" : "forward/forwardShader.frag",
            "OutputLayout" : [ "outColor", "outNormalDepth" ]
        },
        "Skinned" : {
            "VertexShader" : "forward/forwardShaderSkinned.vert",
            "FragmentShader" : "forward/forwardShader.frag",
            "OutputLayout" : [ "outColor", "outNormalDepth" ]
        },
        "Instanced" : {
            "VertexShader" : "forward/forwardShaderInstanced.vert",
            "FragmentShader" : "forward/forwardShader.frag",
            "OutputLayout" : [ "outColor", "outNormalDepth" ]
        }
    },
    "Options" : {
        "IsBlended" : true,
        "NoPost" : false,
        "CullFrontFace" : false,
        "DepthRead" : true,
        "DepthWrite" : true,
        "CastShadow" : false,
        "Parameters" : {
            "diffuseMap" : "0.5, 0.5, 0.5, 1.0",
            "normalMap" : "0.5, 0.5, 1.0, 1.0",
            "specGlossIllMap" : "0.2, 0.2, 0.0, 1.0"
        }
    }
}
