#include "geometry_node.hpp"
#include <iostream>

//#include "model.hpp"

LightNode::LightNode():
Node{},
m_lightIntensity{},
m_lightColor{}
{
  std::cout<<"created LightNode\n";
}

LightNode::LightNode(float intensity, glm::vec3 color):
Node{},
m_lightIntensity{intensity},
m_lightColor{color}
{
  std::cout<<"created LightNode\n";
}


void  LightNode::setLightNode(float intensity, glm::vec3 color){
  m_lightIntensity = intensity;
  m_lightColor = color;
}
