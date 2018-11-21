#version 150

in  vec3 pass_Normal;
in  vec3 pass_Color;
in  vec3 pass_vert_pos;
in  vec3 pass_vert_pos_world;

out vec4 out_Color;
#define PI 3.1415

void main() {
  out_Color = vec4(1.0,pass_Color.y,pass_Color.z, 1.0);
}
