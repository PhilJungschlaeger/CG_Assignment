#ifndef GEOMETRYNODE_HPP
#define GEOMETRYNODE_HPP

#include <string>
#include "node.hpp"
#include "model.hpp"

class GeometryNode : public Node {

 public:
  GeometryNode();
  model& getGeometry();
  void  setGeometry(model& model_in);

private:
  model              m_geometry;
};

#endif
