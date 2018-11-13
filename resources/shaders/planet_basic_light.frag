#version 150

in  vec3 pass_Normal;
in  vec3 pass_Color;
in  vec3 pass_light_pos;
in  vec3 pass_frag_pos;
in vec3 pass_cam_pos;
out vec4 out_Color;
#define PI 3.1415
void main() {

  /*
  todo:
  correct coordinates!
  -pow
  -pi
  -abs

  */

  //we need:
  // frag pos


  //camera
  vec3 camera_pos=pass_cam_pos; //is in center after transformation

  //frag
  vec3 frag_pos = pass_frag_pos;
  vec3 frag_normal= pass_Normal;

  //material:
  float reflectivity= 0.5;
  vec3 specular_color= pass_Color;
  float specularity=0.5;

  //light source:
  vec3 light_pos =pass_light_pos;//needs to be transformed
  float light_intensity=1.0;
  vec3 light_color= vec3(1.0,1.0,1.0);
  vec3 ambient_color= vec3(0.02,0.01,0.04);

  //derived
  vec3 cam_ray_in = normalize(frag_pos-camera_pos);
  vec3 light_ray_in= normalize(frag_pos-light_pos);

  //calculation

  vec3 light_actual=(light_intensity*light_color)/(4*PI*pow(length(light_pos-frag_pos),2.0f));

  //full formular
  //ambient:
  vec3 color_result = ambient_color;

  //specular
  float specular_factor= dot(cam_ray_in,reflect(light_ray_in,frag_normal));
  color_result +=light_actual*(specular_factor*specular_color);

  //diffuse
//  color_result +=light_actual*();


  out_Color = vec4(color_result.x,color_result.y,color_result.z, 1.0);
}
