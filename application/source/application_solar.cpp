#include "application_solar.hpp"
#include "window_handler.hpp"
#include "camera_node.hpp"
#include "point_light.hpp"

#include "utils.hpp"
#include "shader_loader.hpp"
#include "model_loader.hpp"

#include <glbinding/gl/gl.h>
// use gl definitions from glbinding
using namespace gl;

//dont load gl bindings from glfw
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <math.h>       /* cos */

#define PI 3.14159265

ApplicationSolar::ApplicationSolar(std::string const& resource_path)
 :Application{resource_path}
 ,planet_object{}
 ,m_view_transform{glm::translate(glm::fmat4{}, glm::fvec3{0.0f, 0.0f, 4.0f})}
 ,m_view_projection{utils::calculate_projection_matrix(initial_aspect_ratio)}
 ,m_shader_name{"planet_mode_1"}
{
  initializeTheStars();
  initializeTheOrbits();
  initializeGeometry();
  initializeShaderPrograms();
  initializeScene();
  loadTextures();
  initzializeTextures();
  initializeSkyBox();

}

ApplicationSolar::~ApplicationSolar() {
  glDeleteBuffers(1, &planet_object.vertex_BO);
  glDeleteBuffers(1, &planet_object.element_BO);
  glDeleteVertexArrays(1, &planet_object.vertex_AO);
}

void ApplicationSolar::render() const{
  renderSkyBox();
  renderStars();
  renderOrbits();
  renderObjects();
}

void ApplicationSolar::uploadView() {
  // vertices are transformed in camera space, so camera transform must be inverted
  glm::fmat4 view_matrix = glm::inverse(m_view_transform);
  // upload matrix to gpu
  // bind shader to which to upload unforms
  glUseProgram(m_shaders.at(m_shader_name).handle);
  glUniformMatrix4fv(m_shaders.at(m_shader_name).u_locs.at("ViewMatrix"),
                     1, GL_FALSE, glm::value_ptr(view_matrix));

  glUseProgram(m_shaders.at("stars").handle);
  glUniformMatrix4fv(m_shaders.at("stars").u_locs.at("ViewMatrix"),
                     1, GL_FALSE, glm::value_ptr(view_matrix));

  glUseProgram(m_shaders.at("orbits").handle);
  glUniformMatrix4fv(m_shaders.at("orbits").u_locs.at("ViewMatrix"),
                     1, GL_FALSE, glm::value_ptr(view_matrix));

  glUseProgram(m_shaders.at("skybox").handle);
  glUniformMatrix4fv(m_shaders.at("skybox").u_locs.at("ViewMatrix"),
                     1, GL_FALSE, glm::value_ptr(view_matrix));
}

void ApplicationSolar::uploadProjection() {
  // upload matrix to gpu
  glUseProgram(m_shaders.at(m_shader_name).handle);
  glUniformMatrix4fv(m_shaders.at(m_shader_name).u_locs.at("ProjectionMatrix"),
                     1, GL_FALSE, glm::value_ptr(m_view_projection));

  glUseProgram(m_shaders.at("stars").handle);
  glUniformMatrix4fv(m_shaders.at("stars").u_locs.at("ProjectionMatrix"),
                     1, GL_FALSE, glm::value_ptr(m_view_projection));

    glUseProgram(m_shaders.at("orbits").handle);
  glUniformMatrix4fv(m_shaders.at("orbits").u_locs.at("ProjectionMatrix"),
                     1, GL_FALSE, glm::value_ptr(m_view_projection));

    glUseProgram(m_shaders.at("skybox").handle);
  glUniformMatrix4fv(m_shaders.at("skybox").u_locs.at("ProjectionMatrix"),
                     1, GL_FALSE, glm::value_ptr(m_view_projection));
}

// update uniform locations
void ApplicationSolar::uploadUniforms() {
  // upload uniform values to new locations
  uploadView();
  uploadProjection();
}

///////////////////////////// intialisation functions /////////////////////////
// load shader sources
void ApplicationSolar::initializeSkyBox(){
  glActiveTexture(GL_TEXTURE0);
  glGenTextures(1, &skybox.handle);
  glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.handle);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  for(unsigned int idx = 0; idx < skybox_textures.size(); ++idx){
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + idx, 0, skybox_textures[idx].m_pixelData.channels, (GLsizei)skybox_textures[idx].m_pixelData.width,
                (GLsizei)skybox_textures[idx].m_pixelData.height, 0, skybox_textures[idx].m_pixelData.channels,
                skybox_textures[idx].m_pixelData.channel_type, skybox_textures[idx].m_pixelData.ptr());
  }

  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BASE_LEVEL, 0);
glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, 0);
}

void ApplicationSolar::initializeTheStars() {
  m_stars = std::vector<float>();
  srand(time(0));
  int totalStarAmount =100000;
  float posMin = -100;
  float posMax = 100;

  float colorMin = 0;
  float colorMax = 1;

  for( int i = 0; i < totalStarAmount; i++){

      for (int pos = 0; pos < 3; pos++){
        float new_coordinat = posMin + rand() * ( posMax - posMin ) / RAND_MAX;
        m_stars.push_back(new_coordinat );
      }

      //I like randomness between green and blue
      m_stars.push_back(0.0f);
      m_stars.push_back(0.8f);
      float new_color = colorMin + rand() * ( colorMax - colorMin ) / RAND_MAX;
      m_stars.push_back(new_color);
  }
}


void ApplicationSolar::initializeTheOrbits() {
  m_orbits = std::vector<float>();
  int number_of_orbits = 7;
  float ref_radius = 1.0f * 8;
  int orbit_iterations = 10;


  for( int i = 1; i <number_of_orbits; i++){
    float radius = ref_radius *i;
    for (int it = 0; it < orbit_iterations; it++){

      float x = radius* cos(2.0*PI* ((float)  (it-1)/((float) orbit_iterations)));
      float z = radius* sin(2.0*PI* ((float)  (it-1)/((float) orbit_iterations)));
      float y = 0;
       m_orbits.push_back(x);
       m_orbits.push_back(y);
       m_orbits.push_back(z);

      x = radius* cos(2.0*PI* ((float)  it/((float) orbit_iterations)));
      z = radius* sin(2.0*PI* ((float)  it/((float) orbit_iterations)));
      y = 0;
       m_orbits.push_back(x);
       m_orbits.push_back(y);
       m_orbits.push_back(z);
    }
  }


  /*
  float posMin = -10;
  float posMax = 10;

  for( int i = 0; i < number_of_orbits*orbit_iterations; i++){

      for (int pos = 0; pos < 3; pos++){
        float new_coordinat = posMin + rand() * ( posMax - posMin ) / RAND_MAX;
        m_orbits.push_back(new_coordinat );
      }
  }
  */
}

void ApplicationSolar::initializeShaderPrograms() {
  // store shader program objects in container
    m_shaders.emplace("planet_mode_1", shader_program{{{GL_VERTEX_SHADER,m_resource_path + "shaders/planet_basic_light.vert"},
                                             {GL_FRAGMENT_SHADER, m_resource_path + "shaders/planet_basic_light.frag"}}});

    m_shaders.emplace("planet_mode_2", shader_program{{{GL_VERTEX_SHADER,m_resource_path + "shaders/planet_cell_shading.vert"},
                                             {GL_FRAGMENT_SHADER, m_resource_path + "shaders/planet_cell_shading.frag"}}});

  // store shader program objects in container

  m_shaders.emplace("stars", shader_program{{{GL_VERTEX_SHADER,m_resource_path + "shaders/stars.vert"},
                                          {GL_FRAGMENT_SHADER, m_resource_path + "shaders/stars.frag"}}});
  // store shader program objects in container

  m_shaders.emplace("orbits", shader_program{{{GL_VERTEX_SHADER,m_resource_path + "shaders/orbits.vert"},
                                          {GL_FRAGMENT_SHADER, m_resource_path + "shaders/orbits.frag"}}});

  m_shaders.emplace("skybox", shader_program{{{GL_VERTEX_SHADER,m_resource_path + "shaders/skybox.vert"},
                                          {GL_FRAGMENT_SHADER, m_resource_path + "shaders/skybox.frag"}}});

  // request uniform locations for shader program
  m_shaders.at("planet_mode_1").u_locs["NormalMatrix"] = -1;
  m_shaders.at("planet_mode_1").u_locs["ModelMatrix"] = -1;
  m_shaders.at("planet_mode_1").u_locs["ViewMatrix"] = -1;
  m_shaders.at("planet_mode_1").u_locs["ProjectionMatrix"] = -1;
  m_shaders.at("planet_mode_1").u_locs["YourTexture"] = -1;

  m_shaders.at("planet_mode_2").u_locs["NormalMatrix"] = -1;
  m_shaders.at("planet_mode_2").u_locs["ModelMatrix"] = -1;
  m_shaders.at("planet_mode_2").u_locs["ViewMatrix"] = -1;
  m_shaders.at("planet_mode_2").u_locs["ProjectionMatrix"] = -1;
  m_shaders.at("planet_mode_2").u_locs["Planet_Color"]= -1;

  m_shaders.at("stars").u_locs["ViewMatrix"] = -1;
  m_shaders.at("stars").u_locs["ProjectionMatrix"] = -1;

  m_shaders.at("orbits").u_locs["ViewMatrix"] = -1;
  m_shaders.at("orbits").u_locs["ProjectionMatrix"] = -1;

  m_shaders.at("skybox").u_locs["YourTexture"]=-1;
  m_shaders.at("skybox").u_locs["ProjectionMatrix"]=-1;
  m_shaders.at("skybox").u_locs["ViewMatrix"]=-1;
}

// load models
void ApplicationSolar::initializeGeometry() {
  std::cout << "resouce: "<< m_resource_path << "\n";
  planet_model = model_loader::obj(m_resource_path + "models/sphere.obj", model::NORMAL| model::TEXCOORD);
  star_model = model{m_stars, (model::POSITION + model::NORMAL), {0}};
  orbit_model = model{m_orbits, (model::POSITION), {0}};
  skybox_model = model_loader::obj(m_resource_path + "models/skybox.obj");
  // generate vertex array object
  glGenVertexArrays(1, &planet_object.vertex_AO);
  // bind the array for attaching buffers
  glBindVertexArray(planet_object.vertex_AO);

  // generate generic buffer
  glGenBuffers(1, &planet_object.vertex_BO);
  // bind this as an vertex array buffer containing all attributes
  glBindBuffer(GL_ARRAY_BUFFER, planet_object.vertex_BO);
  // configure currently bound array buffer
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * planet_model.data.size(), planet_model.data.data(), GL_STATIC_DRAW);

  // activate first attribute on gpu
  glEnableVertexAttribArray(0);
  // first attribute is 3 floats with no offset & stride
  glVertexAttribPointer(0, model::POSITION.components, model::POSITION.type, GL_FALSE, planet_model.vertex_bytes, planet_model.offsets[model::POSITION]);
  // activate second attribute on gpu
  glEnableVertexAttribArray(1);
  // second attribute is 3 floats with no offset & stride
  glVertexAttribPointer(1, model::NORMAL.components, model::NORMAL.type, GL_FALSE, planet_model.vertex_bytes, planet_model.offsets[model::NORMAL]);

  glEnableVertexAttribArray(2);
  // second attribute is 3 floats with no offset & stride
  glVertexAttribPointer(2, model::TEXCOORD.components, model::TEXCOORD.type, GL_FALSE, planet_model.vertex_bytes, planet_model.offsets[model::TEXCOORD]);

   // generate generic buffer
  glGenBuffers(1, &planet_object.element_BO);
  // bind this as an vertex array buffer containing all attributes
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, planet_object.element_BO);
  // configure currently bound array buffer
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, model::INDEX.size * planet_model.indices.size(), planet_model.indices.data(), GL_STATIC_DRAW);

  // store type of primitive to draw
  planet_object.draw_mode = GL_TRIANGLES;
  // transfer number of indices to model object
  planet_object.num_elements = GLsizei(planet_model.indices.size());


  /////////////Stars now:
  // generate vertex array object
  glGenVertexArrays(1, &star_object.vertex_AO);
  // bind the array for attaching buffers
  glBindVertexArray(star_object.vertex_AO);

  // generate generic buffer
  glGenBuffers(1, &star_object.vertex_BO);
  // bind this as an vertex array buffer containing all attributes
  glBindBuffer(GL_ARRAY_BUFFER, star_object.vertex_BO);
  // configure currently bound array buffer
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * star_model.data.size(), star_model.data.data(), GL_STATIC_DRAW);

  // activate first attribute on gpu
  glEnableVertexAttribArray(0);
  // first attribute is 3 floats with no offset & stride
  glVertexAttribPointer(0, model::POSITION.components, model::POSITION.type, GL_FALSE, star_model.vertex_bytes, star_model.offsets[model::POSITION]);
  // activate second attribute on gpu
  glEnableVertexAttribArray(1);
  // second attribute is 3 floats with no offset & stride
  glVertexAttribPointer(1, model::NORMAL.components, model::NORMAL.type, GL_FALSE, star_model.vertex_bytes, star_model.offsets[model::NORMAL]);

   // generate generic buffer
  glGenBuffers(1, &star_object.element_BO);
  // bind this as an vertex array buffer containing all attributes
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, star_object.element_BO);
  // configure currently bound array buffer
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, model::INDEX.size * star_model.indices.size(), star_model.indices.data(), GL_STATIC_DRAW);

  // store type of primitive to draw
  star_object.draw_mode = GL_POINTS;  //changed to points
  // transfer number of indices to model object
  star_object.num_elements = GLsizei(star_model.indices.size()/6);


  /////////////Orbits now:
  // generate vertex array object
  glGenVertexArrays(1, &orbit_object.vertex_AO);
  // bind the array for attaching buffers
  glBindVertexArray(orbit_object.vertex_AO);

  // generate generic buffer
  glGenBuffers(1, &orbit_object.vertex_BO);
  // bind this as an vertex array buffer containing all attributes
  glBindBuffer(GL_ARRAY_BUFFER, orbit_object.vertex_BO);
  // configure currently bound array buffer
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * orbit_model.data.size(), orbit_model.data.data(), GL_STATIC_DRAW);

  // activate first attribute on gpu
  glEnableVertexAttribArray(0);
  // first attribute is 3 floats with no offset & stride
  glVertexAttribPointer(0, model::POSITION.components, model::POSITION.type, GL_FALSE, orbit_model.vertex_bytes, orbit_model.offsets[model::POSITION]);

  //delted attribute.. we set clr in frag shader

   // generate generic buffer
  glGenBuffers(1, &orbit_object.element_BO);
  // bind this as an vertex array buffer containing all attributes
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, orbit_object.element_BO);
  // configure currently bound array buffer
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, model::INDEX.size * orbit_model.indices.size(), orbit_model.indices.data(), GL_STATIC_DRAW);

  // store type of primitive to draw
  orbit_object.draw_mode = GL_LINES;  //changed to Line
  // transfer number of indices to model object
  orbit_object.num_elements = GLsizei(orbit_model.indices.size()/3);

  //skybox:

 // generate vertex array object
 glGenVertexArrays(1, &skybox_object.vertex_AO);
 // bind the array for attaching buffers
 glBindVertexArray(skybox_object.vertex_AO);

 // generate generic buffer
 glGenBuffers(1, &skybox_object.vertex_BO);
 // bind this as an vertex array buffer containing all attributes
 glBindBuffer(GL_ARRAY_BUFFER, skybox_object.vertex_BO);
 // configure currently bound array buffer
 glBufferData(GL_ARRAY_BUFFER, sizeof(float) * skybox_model.data.size(), skybox_model.data.data(), GL_STATIC_DRAW);

 // activate first attribute on gpu
 glEnableVertexAttribArray(0);
 // first attribute is 3 floats with no offset & stride
 glVertexAttribPointer(0, model::POSITION.components, model::POSITION.type, GL_FALSE, skybox_model.vertex_bytes, skybox_model.offsets[model::POSITION]);
 // generate generic buffer
 glGenBuffers(1, &skybox_object.element_BO);

 // bind this as an vertex array buffer containing all attributes
 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skybox_object.element_BO);
 // configure currently bound array buffer
 glBufferData(GL_ELEMENT_ARRAY_BUFFER, model::INDEX.size * skybox_model.indices.size(), skybox_model.indices.data(), GL_STATIC_DRAW);

 // store type of primitive to draw
 skybox_object.draw_mode = GL_TRIANGLES;
 // transfer number of indices to model object
 skybox_object.num_elements = GLsizei(skybox_model.indices.size());
}

///////////////////////////// callback functions for window events ////////////
// handle key input
void ApplicationSolar::keyCallback(int key, int action, int mods) {
  //move forward
  if (key == GLFW_KEY_W  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.0f, 0.0f, -0.1f});
    uploadView();
  }
  //move backward
  else if (key == GLFW_KEY_S  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.0f, 0.0f, 0.1f});
    uploadView();
  }
  //move up
  else if (key == GLFW_KEY_UP  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.0f, 1.0f, 0.0f});
    uploadView();
  }
  //move down
  else if (key == GLFW_KEY_DOWN  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.0f, -1.0f, 0.0f});
    uploadView();
  }
  //move left multi key
  else if (key == GLFW_KEY_A  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{-1.0f, 0.0f, 0.0f});
    uploadView();
  }
  else if (key == GLFW_KEY_LEFT  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{-1.0f, 0.0f, 0.0f});
    uploadView();
  }
  //move right mutli key
  else if (key == GLFW_KEY_D  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{1.0f, 0.0f, 0.0f});
    uploadView();
  }
  else if (key == GLFW_KEY_RIGHT  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{1.0f, 0.0f, 0.0f});
    uploadView();
  }
  if(key == GLFW_KEY_1  && (action == GLFW_PRESS )) {
    std::cout<<"Selected Shader Mode: "<<1<<" [Basic Lighting]\n";
    m_shader_name = "planet_mode_1";
    uploadView();
    uploadProjection();
  }
  else if(key == GLFW_KEY_2  && (action == GLFW_PRESS )){
    std::cout<<"Selected Shader Mode: "<<2<<" [Cell Shading]\n";
    m_shader_name = "planet_mode_2";
    uploadView();
    uploadProjection();
  }
}

//handle delta mouse movement input
void ApplicationSolar::mouseCallback(double pos_x, double pos_y) {
  // mouse handling
  //std::cout<<"mouse update: "<<pos_x<<" , "<<pos_y<<"\n";
  m_view_transform = glm::rotate(m_view_transform, float(pos_x)/100, glm::fvec3{0.0f, -1.0f, 0.0f});
  m_view_transform = glm::rotate(m_view_transform, float(pos_y)/100, glm::fvec3{1.0f, 0.0f, 0.0f});
  uploadView(); //we always need to upload, because camera is center
}

//handle resizing
void ApplicationSolar::resizeCallback(unsigned width, unsigned height) {
  // recalculate projection matrix for new aspect ration
  m_view_projection = utils::calculate_projection_matrix(float(width) / float(height));
  // upload new projection matrix
  uploadProjection();
}

void ApplicationSolar::initializeScene() {
  //init root Node
  Node* rootNode     =     new Node("root");
  //init scene Graph
  m_scene           =     SceneGraph("solarsystem", rootNode);
  GeometryNode* sun  =     new GeometryNode("sun");


  rootNode->addChildren(sun);
  //create 8 planets and add as child to sun
  for(int i = 0; i < 8; i++){
    GeometryNode* planet = new GeometryNode("test");
    planet->setGeometry(planet_model);
    sun->addChildren(planet);
  }
  GeometryNode* stars  =     new GeometryNode("stars");
  stars->setGeometry(star_model);
  GeometryNode* orbits  =     new GeometryNode("orbits");
  orbits->setGeometry(orbit_model);
  LightNode* light = new LightNode(50, glm::vec3{1.0, 1.0, 1.0});
}

void ApplicationSolar::renderSkyBox() const {
  glDepthMask(GL_FALSE);
  glUseProgram(m_shaders.at("skybox").handle);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.handle);
  glBindVertexArray(skybox_object.vertex_AO);
  glDrawElements(skybox_object.draw_mode, skybox_object.num_elements, model::INDEX.type, NULL);
  glDepthMask(GL_TRUE);
}

void ApplicationSolar::renderObjects() const{

  // bind shader to upload uniforms

  glUseProgram(m_shaders.at(m_shader_name).handle);

  for(unsigned int i = 0; i < m_scene.getRoot()->getChildren("sun")->getChildrenList().size(); i++){

    glm::fmat4 model_matrix = glm::rotate(glm::fmat4{}, float(glfwGetTime()/2*i/2), glm::fvec3{0.0f, 1.0f, 0.0f});
    model_matrix = glm::translate(model_matrix, glm::fvec3{0.0f, 0.0f, -1.0f * i * 8});
    model_matrix = glm::rotate(model_matrix, float(glfwGetTime()), glm::fvec3{0.0f, 1.0f, 0.0f});
    glm::fvec3 scale {(9-i)/3, (9-i)/3, (9-i)/3};
    model_matrix = glm::scale(model_matrix, scale);
    glm::fvec3 planet_color {1-i/(m_scene.getRoot()->getChildren("sun")->getChildrenList().size()-1),0.2,i/(m_scene.getRoot()->getChildren("sun")->getChildrenList().size()-1)};
    if("planet_mode_2"==m_shader_name)
      glUniform3f(m_shaders.at("planet_mode_2").u_locs.at("Planet_Color"), planet_color.x, planet_color.y, planet_color.z);


    glUniformMatrix4fv(m_shaders.at(m_shader_name).u_locs.at("ModelMatrix"), 1, GL_FALSE, glm::value_ptr(model_matrix));

    // extra matrix for normal transformation to keep them orthogonal to surface
    glm::fmat4 normal_matrix = glm::inverseTranspose(glm::inverse(m_view_transform) * model_matrix);
    glUniformMatrix4fv(m_shaders.at(m_shader_name).u_locs.at("NormalMatrix"), 1, GL_FALSE, glm::value_ptr(normal_matrix));
    // bind the VAO to draw
    glBindVertexArray(planet_object.vertex_AO);

    // draw bound vertex array using bound shader
    glDrawElements(planet_object.draw_mode, planet_object.num_elements, model::INDEX.type, NULL);

    if("planet_mode_1"==m_shader_name)
    {
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, tex_objects[0].handle);
      glUniform1i(m_shaders.at("planet_mode_1").u_locs.at("YourTexture"), 0);
    }

  }
}

void ApplicationSolar::renderStars() const{
  // bind shader to upload uniforms
  glUseProgram(m_shaders.at("stars").handle);
  glBindVertexArray(star_object.vertex_AO);
  glPointSize(3.0);
  glDrawArrays(star_object.draw_mode, 0, (int)m_stars.size());
}

void ApplicationSolar::renderOrbits() const{
  // bind shader to upload uniforms
  glUseProgram(m_shaders.at("orbits").handle);
  glBindVertexArray(orbit_object.vertex_AO);
  glLineWidth(10.0);  //fat and retro
  glDrawArrays(orbit_object.draw_mode, 0, (int)m_orbits.size());
}

void ApplicationSolar::loadTextures(){
  // load textures, datatype textures defined in struct, consists of name and pixel_data
  texture basic   ("basic", texture_loader::file(m_resource_path + "textures/sushi.png"));
  texture_container.push_back(basic);
  //6times:
  texture basic2   ("basic", texture_loader::file(m_resource_path + "textures/salmontexture.png"));
  skybox_textures.push_back(basic2);
  skybox_textures.push_back(basic2);
  skybox_textures.push_back(basic2);
  skybox_textures.push_back(basic2);
  skybox_textures.push_back(basic2);
  skybox_textures.push_back(basic2);
}

void ApplicationSolar::initzializeTextures(){
  //inizia
  for(int i = 0; i < 9; i++){
    texture_object tex;
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &tex.handle);
    glBindTexture(GL_TEXTURE_2D, tex.handle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, texture_container[0].m_pixelData.channels, texture_container[0].m_pixelData.width, texture_container[0].m_pixelData.height, 0, texture_container[0].m_pixelData.channels, texture_container[0].m_pixelData.channel_type, texture_container[0].m_pixelData.ptr());

    tex_objects.push_back(tex);
  }
}

// exe entry point
int main(int argc, char* argv[]) {
  Application::run<ApplicationSolar>(argc, argv, 3, 2);
}
