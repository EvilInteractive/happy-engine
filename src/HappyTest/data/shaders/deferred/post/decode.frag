
vec3 getPosition(in vec2 ndc, in vec2 bufferSize, in vec2 cameraRange, in float depth)
{ 
	//float linDepth = ((-cameraRange.y * cameraRange.x) / (cameraRange.y - cameraRange.x)) / (depth - cameraRange.y / (cameraRange.y - cameraRange.x));
	//vec3 viewRay = vec3((fragCoord / bufferSize) * 2.0f - vec2(1.0f, 1.0f), 1.0f);
    //vec3 screenCoord = 
                //vec3(
                        //((fragCoord.x / bufferSize.x) - 0.5) * 2.0, 
                        //((-fragCoord.y / bufferSize.y) + 0.5) * 2.0 / (bufferSize.x / bufferSize.y), 
                        //cameraRange.x / (cameraRange.y - depth * (cameraRange.y - cameraRange.x)) * cameraRange.y
                    //);
    //screenCoord.x *= screenCoord.z;
    //screenCoord.y *= -screenCoord.z;
    
    //return viewRay * linDepth;
	return vec3(0,0,0);
}

vec3 getNormal(in vec2 packedNormal)
{
	vec3 normal = vec3(packedNormal * 2.0f - 1.0f, 0.0f);
	normal.z = sqrt(1.0f - dot(normal.xy, normal.xy));
	return normalize(normal);
}