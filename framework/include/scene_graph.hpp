#ifndef SCENE_GRAPH_HPP
#define SCENE_GRAPH_HPP

#include <string>
#include "node.hpp"
//class Node;


class SceneGraph {

 public:
  SceneGraph();
  SceneGraph(std::string name_in, Node* root_in);
  std::string   getName() const;
  Node*         getRoot();
//std::string printGraph();

private:
  void          setRoot(Node* node_in);
  void          setName(std::string name_in);

  std::string   m_name;
  Node*         m_root;

};
#endif
