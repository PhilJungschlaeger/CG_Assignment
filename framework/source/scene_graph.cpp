#include "scene_graph.hpp"
#include <iostream>

//#include "node.hpp"
SceneGraph::SceneGraph():
m_name{},
m_root{}
{
  std::cout<<"Created: SceneGraph\n";
}

SceneGraph::SceneGraph(std::string name_in, Node* root_in):
m_name{name_in},
m_root{root_in}
{
  std::cout<<"Created: SceneGraph\n";
}

std::string   SceneGraph::getName()const{
  return m_name;
}

Node*          SceneGraph::getRoot()const{
  return m_root;
}

void          SceneGraph::setRoot(Node* node_in){
  m_root=node_in;
}

void          SceneGraph::setName(std::string name_in){
  m_name=name_in;
}
