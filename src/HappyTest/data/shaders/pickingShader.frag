#version 150 core
//out uint outId;
out vec4 outId;

//uniform uint id;
uniform vec4 id;

void main()
{
	outId = id;
}