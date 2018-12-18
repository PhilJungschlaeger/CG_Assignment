#version 150
#extension GL_ARB_explicit_attrib_location : require
// vertex attributes of VAO

//Matrix Uniforms as specified with glUniformMatrix4fv
layout(location = 0) in vec4 in_Position;
layout(location = 1) in vec2 in_TexCoord;
uniform vec4 Effect_Mode;

out  vec2 pass_TexCoord;
out  vec4 effect_mode;

void main(void)
{
	gl_Position = in_Position;
	pass_TexCoord = in_TexCoord;
	effect_mode =Effect_Mode;
}
