#include "geometry_node.hpp"
#include <iostream>

//#include "model.hpp"

GeometryNode::GeometryNode():
Node{},
m_geometry{}
{
  std::cout<<"created GeometryNode\n";
}
GeometryNode::GeometryNode(std::string name):
Node{name},
m_geometry{}
{
  std::cout<<"created GeometryNode\n";
}


model& GeometryNode::getGeometry(){
  return m_geometry;
}

void  GeometryNode::setGeometry(model& model_in){
  m_geometry=model_in;
}
