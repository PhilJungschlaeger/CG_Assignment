#version 150

in  vec3 pass_Normal;
in  vec3 pass_Color;
out vec4 out_Color;

void main() {
  out_Color = vec4(1.0,pass_Color.y,pass_Color.z, 1.0);
}
