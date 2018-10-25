#ifndef GEOMETRYNODE_HPP
#define GEOMETRYNODE_HPP

#include <string>
#include "node.hpp"
#include "model.hpp"

class GeometryNode : public Node {

 public:
  GeometryNode();
  GeometryNode(std::string name);
  model& getGeometry();
  void  setGeometry(model& model_in);

private:
  model              m_geometry;
};

#endif
