
vec3 getPosition(in float p_depth, in vec2 p_ndc, in vec4 p_projParams)
{ 
    float depth = p_depth * 2.0f - 1.0f;
	float linDepth = p_projParams.w / (depth - p_projParams.z);

	return vec3((p_ndc * linDepth) / p_projParams.xy, linDepth);
}


//source: http://aras-p.info/texts/CompactNormalStorage.html#method04spheremap
vec3 getNormal(in vec2 packedNormal)
{
	vec4 nn = vec4(packedNormal * 2 + vec2(-1, -1), 1, -1);
	float l = dot(nn.xyz, -nn.xyw);
	nn.z = l;
	nn.xy *= sqrt(l);
	return normalize(nn.xyz * 2 + vec3(0, 0, -1));
}