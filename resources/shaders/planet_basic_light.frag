#version 150

in  vec3 pass_Normal;
in  vec3 pass_Color;
in  vec3 pass_vert_pos;
in vec3 pass_cam_dir;
out vec4 out_Color;
#define PI 3.1415
void main() {


  //we need:
  // frag pos

  //frag
  vec3 frag_pos = pass_vert_pos;
  vec3 frag_normal= pass_Normal;

  //material:
  float reflectivity= 0.5;
  vec3 specular_color = vec3(1.0,0.0,0.0);
  vec3 diffuse_color  = pass_Color;
  float m=30.0;
  float ks=1.0;

  //light source:
  vec3 light_pos =vec3(0,0,0);
  float light_intensity=30.0;
  vec3 light_color= vec3(1.0,1.0,1.0);
  vec3 ambient_color= vec3(0.02,0.01,0.04);

  //derived
  vec3 cam_ray_in = normalize(pass_cam_dir);
  vec3 light_ray_in= normalize(light_pos-frag_pos);

  //calculation

  vec3 light_actual=light_color;//(light_intensity*light_color)/(4*PI*pow(length(light_pos-frag_pos),2.0f));


  //full formular
  //ambient:
  vec3 color_result = ambient_color;

  //difuse
  float diffuse_factor = max(dot(light_ray_in, frag_normal), 0.0);
  color_result += diffuse_factor * diffuse_color;

  //specular
  if(diffuse_factor>0){
    // float specular_factor= pow( max( dot( cam_ray_in , normalize( reflect( light_ray_in , frag_normal ) ) ) , 0) , m );
    //
    // color_result += light_actual* (specular_factor * specular_color * ks);
  }


  //diffuse
//  color_result +=light_actual*();


  out_Color = vec4(color_result.x,color_result.y,color_result.z, 1.0);
}
