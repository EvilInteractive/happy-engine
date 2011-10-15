
vec3 getPosition(in float p_depth, in vec2 p_ndc, in vec4 p_projParams)
{ 
    float depth = p_depth * 2.0f - 1.0f;
	float linDepth = p_projParams.w / (depth - p_projParams.z);

	return vec3((p_ndc * linDepth) / p_projParams.xy, linDepth);
}


//source: http://aras-p.info/texts/CompactNormalStorage.html#method04spheremap
vec3 decodeNormal(in vec2 packedNormal)
{
	vec4 nn = vec4(packedNormal * 2 + vec2(-1, -1), 1, -1);
	float l = dot(nn.xyz, -nn.xyw);
	nn.z = l;
	nn.xy *= sqrt(l);
	return normalize(nn.xyz * 2 + vec3(0, 0, -1));
}
//
//vec3 decodeNormal(in vec2 packedNormal)
//{
	//vec2 fenc = packedNormal * 4.0f - 2.0f;
    //float f = dot(fenc,fenc);
    //float g = sqrt(1.0f - f / 4.0f);
    //vec3 n;
    //n.xy = fenc*g;
    //n.z = 1.0f - f / 2.0f;
    //return n;
//}