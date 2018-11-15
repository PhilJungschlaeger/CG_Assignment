#version 150
#extension GL_ARB_explicit_attrib_location : require
// vertex attributes of VAO
layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec3 in_Normal;


//Matrix Uniforms as specified with glUniformMatrix4fv
uniform mat4 ModelMatrix;	//Modell
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 NormalMatrix;
uniform vec3 Planet_Color;

out vec3 pass_Normal;
out vec3 pass_Color;
out  vec3 pass_vert_pos;
out  vec3 pass_cam_dir;

void main(void)
{
	gl_Position = (ProjectionMatrix  * ViewMatrix * ModelMatrix) * vec4(in_Position, 1.0);

	vec4 vertPos4 = ModelMatrix * vec4(in_Position, 1.0);
	pass_vert_pos = vec3(vertPos4) / vertPos4.w;

	pass_vert_pos = vec3(ViewMatrix * vec4(pass_vert_pos,0.0) ).xyz;

	pass_Normal = vec3((NormalMatrix * vec4(in_Normal, 0.0)).xyz);

	pass_cam_dir= -vec3(ViewMatrix * vec4(in_Position, 0.0)).xyz;

	pass_Color = Planet_Color;
}
