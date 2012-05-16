
uniform sampler2D depthMap;
uniform sampler2D colorMap;
uniform sampler2D normalMap;
uniform sampler2D noiseMap;
uniform vec2 screenSize;

//Ambient Occlusion form factor:
float getAo(in vec3 ddiff, in vec3 normal, in vec3 dnorm)
{
    float dist      = length(ddiff);
    vec3  pixelVect = ddif / dist;
    return (1.0f - clamp(dot(dnorm, -pixelVect), 0.0f, 1.0f))) *
           clamp(dot(normal, pixelVect), 0.0f, 1.0f)           *
           (1.0f - 1.0f / sqrt(1.0f / (dist * dist) + 1.0f));
}
//GI form factor:
float getGi(in vec3 ddiff, in vec3 normal, in vec3 dnorm)
{
    float dist      = length(ddiff);
    vec3  pixelVect = ddiff / dist;
    return 1.0f * clamp(dot( dnorm, -pixelVect), 0.0f, 1.0) *
                  clamp(dot(normal,  pixelVect), 0.0f, 1.0f) / (dist * dist + 1.0f);  
}

void getAoAndGI(in vec3 position, in vec3 normal, in float depth, out float ao, out vec3 gi)
{
    //randomization texture
    const vec2 randomMapSize = vec2(4, 4);
    const vec2 randomOffset = vec2(screenSize.x / randomMapSize.x * 5.0f, screenSize.y / randomMapSize.y * 5.0f);
    vec3 random = texture2DLod(noiseMap, texCoord * randomOffset.xy, 0);
    random = random * 2.0f - 1.0f;

    //initialize variables:
    ao = 0.0f;
    gi = vec3(0.0f, 0.0f, 0.0f);
    const float offsetIncrementX = 1.0f / screenSize.x * 0.1f;
    const float offsetIncrementY = 1.0f / screenSize.y * 0.1f;
    float pixelOffsetX = incx;
    float pixelOffsetY = incy;

    //3 rounds of 8 samples each. 
    for(float i = 0.0f; i < 3.0f; ++i) 
    {
       float normalPixelOffsetX = (pixelOffsetX + 0.0007f * random.x) / depth;
       float normalPixelOffsetY = (pixelOffsetY + 0.0007f * random.y) / depth;

       vec3 ddiff1 = getPosition(texCoord + vec2( normalPixelOffsetX,  normalPixelOffsetY), nDc, projParams) - position;
       vec3 ddiff2 = getPosition(texCoord + vec2( normalPixelOffsetX, -normalPixelOffsetY), nDc, projParams) - position;
       vec3 ddiff3 = getPosition(texCoord + vec2(-normalPixelOffsetX,  normalPixelOffsetY), nDc, projParams) - position;
       vec3 ddiff4 = getPosition(texCoord + vec2(-normalPixelOffsetX, -normalPixelOffsetY), nDc, projParams) - position;
       vec3 ddiff5 = getPosition(texCoord + vec2(   0,                 normalPixelOffsetY), nDc, projParams) - position;
       vec3 ddiff6 = getPosition(texCoord + vec2(   0,                -normalPixelOffsetY), nDc, projParams) - position;
       vec3 ddiff7 = getPosition(texCoord + vec2( normalPixelOffsetX,                   0), nDc, projParams) - position;
       vec3 ddiff8 = getPosition(texCoord + vec2(-normalPixelOffsetX,                   0), nDc, projParams) - position;

       vec3 dnorm1 = decodeNormal(textureLod(normalMap, texCoord + vec2( normalPixelOffsetX,  normalPixelOffsetY)).xy, 0);
       vec3 dnorm2 = decodeNormal(textureLod(normalMap, texCoord + vec2( normalPixelOffsetX, -normalPixelOffsetY)).xy, 0);
       vec3 dnorm3 = decodeNormal(textureLod(normalMap, texCoord + vec2(-normalPixelOffsetX,  normalPixelOffsetY)).xy, 0);
       vec3 dnorm4 = decodeNormal(textureLod(normalMap, texCoord + vec2(-normalPixelOffsetX, -normalPixelOffsetY)).xy, 0);
       vec3 dnorm5 = decodeNormal(textureLod(normalMap, texCoord + vec2(   0,                 normalPixelOffsetY)).xy, 0);
       vec3 dnorm6 = decodeNormal(textureLod(normalMap, texCoord + vec2(   0,                -normalPixelOffsetY)).xy, 0);
       vec3 dnorm7 = decodeNormal(textureLod(normalMap, texCoord + vec2( normalPixelOffsetX,                   0)).xy, 0);
       vec3 dnorm8 = decodeNormal(textureLod(normalMap, texCoord + vec2(-normalPixelOffsetX,                   0)).xy, 0);

       ao +=  getAo(ddiff1, normal, dnorm1);
       ao +=  getAo(ddiff2, normal, dnorm2);
       ao +=  getAo(ddiff3, normal, dnorm3);
       ao +=  getAo(ddiff4, normal, dnorm4);
       ao +=  getAo(ddiff5, normal, dnorm5);
       ao +=  getAo(ddiff6, normal, dnorm6);
       ao +=  getAo(ddiff7, normal, dnorm7);
       ao +=  getAo(ddiff8, normal, dnorm8);

       gi +=  getGi(ddiff1, normal, dnorm1) * textureLod(colorMap, texCoord + vec2( normalPixelOffsetX,  normalPixelOffsetY), 0).rgb;
       gi +=  getGi(ddiff2, normal, dnorm2) * textureLod(colorMap, texCoord + vec2( normalPixelOffsetX, -normalPixelOffsetY), 0).rgb;
       gi +=  getGi(ddiff3, normal, dnorm3) * textureLod(colorMap, texCoord + vec2(-normalPixelOffsetX,  normalPixelOffsetY), 0).rgb;
       gi +=  getGi(ddiff4, normal, dnorm4) * textureLod(colorMap, texCoord + vec2(-normalPixelOffsetX, -normalPixelOffsetY), 0).rgb;
       gi +=  getGi(ddiff5, normal, dnorm5) * textureLod(colorMap, texCoord + vec2(0,                    normalPixelOffsetY), 0).rgb;
       gi +=  getGi(ddiff6, normal, dnorm6) * textureLod(colorMap, texCoord + vec2(0,                   -normalPixelOffsetY), 0).rgb;
       gi +=  getGi(ddiff7, normal, dnorm7) * textureLod(colorMap, texCoord + vec2( normalPixelOffsetX,                   0), 0).rgb;
       gi +=  getGi(ddiff8, normal, dnorm8) * textureLod(colorMap, texCoord + vec2(-normalPixelOffsetX,                   0), 0).rgb;

       //increase sampling area:
       pixelOffsetX += offsetIncrementX;  
       pixelOffsetY += offsetIncrementY;    
    } 
    ao /= 24.0;
    gi /= 24.0;
    //gl_FragColor = vec4(col-vec3(ao)+gi*5.0, 1.0);
}
