#version 150

out vec4 out_Color;

const float pixelsize_x = 1.0 / 640;
const float pixelsize_y = 1.0 / 480;
const mat3  kernel      = mat3(1.0/16, 1.0/8, 1.0/16, 1.0/8, 1.0/4, 1.0/8, 1.0/16, 1.0/8,1.0/16);

uniform sampler2D YourTexture;

in  vec4  effect_mode;
in  vec2 pass_TexCoord;
vec4 tmp;
vec2 texc=pass_TexCoord;

void main() {
  out_Color= texture(YourTexture, pass_TexCoord);

  if(effect_mode.y==1){
     texc.x     = 1 - texc.x;
     out_Color  = texture(YourTexture, texc);
  }

  if(effect_mode.z==1){
    texc.y = 1 - texc.y;
    out_Color = texture(YourTexture, texc);
  }

  if(effect_mode.w==1){
    vec4 sum = vec4(0.0);

     int j = 0;
     for (int x = -1; x <= 1; x++){
       int k = 0;
         for (int y = -1; y <= 1; y++){
             sum += texture(YourTexture, vec2(texc.x + x * pixelsize_x, texc.y + y * pixelsize_y)) * kernel[j][k];
             ++k;
         }
         ++j;
     }
     out_Color = sum;
  }

  if(effect_mode.x==1)
  {//at the end pls

    float avg = 0.2126 * out_Color.r + 0.7152 * out_Color.g + 0.0722 * out_Color.b;
    out_Color = vec4(avg, avg  , avg ,0);
  }
}
