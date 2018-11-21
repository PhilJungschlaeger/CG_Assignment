#ifndef LIGHTNODE_HPP
#define LIGHTNODE_HPP

#include <string>
#include "node.hpp"


class LightNode : public Node {

 public:
  LightNode();
  LightNode(float intensity, glm::vec3 color);
  
  void  setLightNode(float intensity, glm::vec3 color);

private:
  float m_lightIntensity;
  glm::vec3 m_lightColor;
 
};

#endif
