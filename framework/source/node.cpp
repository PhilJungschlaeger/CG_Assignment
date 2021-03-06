#include "node.hpp"
#include <iostream>


Node::Node():
m_parent{},
m_children{ std::list<Node*>() },
m_name{std::string("nonameset")},
m_path{},
m_depth{0},
m_localTransform{},
m_worldTransform{}
{
  std::cout<<"created Node\n";
}

Node::Node(std::string name):
m_parent{},
m_children{std::list<Node*>()},
m_name{name},
m_path{},
m_depth{0},
m_localTransform{},
m_worldTransform{}
{
  std::cout<<"created Node\n";
}


Node*            Node::getParent(){
  return m_parent;
}

void            Node::setParent(Node* node_in){
  m_parent= node_in;
}

Node*            Node::getChildren(std::string child_name_in){
  for (auto i : m_children) {
    if(i->getName()==child_name_in)
    {
      return i;
    }
  }
  std::cout<<"ERROR: Could not find Child Node named: "<<child_name_in<<"\n";
  Node* nnode= new Node();
  return nnode;
}

std::list<Node*>& Node::getChildrenList()
{
  return m_children;
}

std::string     Node::getName(){
  return m_name;
}

std::string     Node::getPath() const{
  return m_path;
}

int             Node::getDepth() const{
  return m_depth;
}

glm::fmat4&      Node::getLocalTransform(){
  return m_localTransform;
}

glm::fmat4&      Node::getWorldTransform(){
  return m_worldTransform;
}

void Node::setLocalTransform(glm::fmat4 mat_in){
  m_localTransform=mat_in;
}
void Node::setWorldTransform(glm::fmat4 mat_in)
{
  m_worldTransform= mat_in;
}
void Node::addChildren(Node* child_in){
  m_children.push_back(child_in);
}

Node* Node::removeChildren(std::string child_name_in){
  Node* tmp = getChildren(child_name_in);
  getChildrenList().remove(getChildren(child_name_in));
  return tmp;
}
