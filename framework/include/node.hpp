#ifndef NODE_HPP
#define NODE_HPP

#include <string>
#include <list>
#include <glm/glm.hpp>


class Node {
 public:
  Node();
  Node*             getParent();
  void              setParent(Node* node_in);
  Node*             getChildren(std::string child_name_in);
  std::list<Node*>& getChildrenList();
  std::string       getName() const;
  std::string       getPath() const;
  int               getDepth() const;
  glm::fmat4&       getLocalTransform();
  glm::fmat4&       getWorldTransform();

  void setLocalTransform(glm::fmat4 mat_in);
  void setWorldTransform(glm::fmat4 mat_in);
  void addChildren(Node* child_in);
  Node* removeChildren(std::string child_name_in);

private:
  Node*             m_parent;
  std::list<Node*>  m_children;
  std::string       m_name;
  std::string       m_path;
  int               m_depth;
  glm::fmat4        m_localTransform;
  glm::fmat4        m_worldTransform;
};

#endif
