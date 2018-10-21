#ifndef GEOMETRYNODE_HPP
#define GEOMETRYNODE_HPP

//#include <string.h>
#include "node.hpp"
#include "model.hpp"

class GeometryNode : public Node {

 public:
  Model getGeometry();
  void  setGeometry(Model model_in);

private:
  Model              m_geometry;

};

#endif
