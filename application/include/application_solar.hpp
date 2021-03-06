#ifndef APPLICATION_SOLAR_HPP
#define APPLICATION_SOLAR_HPP

#include "application.hpp"
#include "model.hpp"
#include "structs.hpp"
#include "scene_graph.hpp"
#include "node.hpp"
#include "geometry_node.hpp"
#include "texture_loader.hpp"

// gpu representation of model
class ApplicationSolar : public Application {
 public:
  // allocate and initialize objects
  ApplicationSolar(std::string const& resource_path);
  // free allocated objects
  ~ApplicationSolar();
  // react to key input
  void keyCallback(int key, int action, int mods);
  //handle delta mouse movement input
  void mouseCallback(double pos_x, double pos_y);
  //handle resizing
  void resizeCallback(unsigned width, unsigned height);

  // draw all objects
  void render() const;
  void renderObjects() const;
  void renderStars() const;
  void renderOrbits() const;
  void renderSkyBox() const;
  void renderFrameBuffer() const;


 protected:
  void initializeSkyBox();

  void initializeTheStars();
  void initializeTheOrbits();
  void initializeScene();
  void initializeShaderPrograms();
  void initializeGeometry();
  void initzializeTextures();
  void initializeFrameBuffer();

  void loadTextures();
  // update uniform values
  void uploadUniforms();
  // upload projection matrix
  void uploadProjection();
  // upload view matrix
  void uploadView();

  // cpu representation of model
  model_object planet_object;
  model_object star_object;
  model_object orbit_object;
  model_object skybox_object;
  model_object quad_object;

  // camera transform matrix
  glm::fmat4 m_view_transform;
  // camera projection matrix
  glm::fmat4 m_view_projection;
  SceneGraph m_scene;
  model planet_model;
  model star_model;
  model orbit_model;
  model skybox_model;
  model quad_model;

  glm::vec4 m_effect_mode;
  std::vector<float> m_stars;
  std::vector<float> m_orbits;
  std::string m_shader_name;
  std::vector<texture> texture_container;
  std::vector<texture_object> tex_objects;
  texture_object skybox;
  std::vector<texture> skybox_textures;
  texture_object frame_buffer_object;
  texture_object frame_buffer_tex_obj;
  texture_object render_buffer_object;
};

#endif
