#version 150

out vec4 out_Color;

uniform sampler2D YourTexture;
in  vec2 pass_TexCoord;

void main() {
  out_Color = texture(YourTexture, pass_TexCoord);
}
