
//source: http://aras-p.info/texts/CompactNormalStorage.html#method04spheremap
//vec2 encodeNormal(vec3 normal)
//{
	//vec2 encNormal = normalize(normal.xy) * sqrt(-normal.z * 0.5f + 0.5f);
	//return encNormal * 0.5f + 0.5f;
//}
vec2 encodeNormal(vec3 normal)
{
	float f = sqrt(-normal.z * 8.0f + 8.0f);
    return normal.xy / f + 0.5f;
}