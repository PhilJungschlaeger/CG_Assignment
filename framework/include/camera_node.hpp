#ifndef CAMERANODE_HPP
#define CAMERANODE_HPP

#include <string>
#include "model.hpp"
#include "node.hpp"
#include <glm/glm.hpp>

class CameraNode : public Node {

 public:
  CameraNode();
  bool        getPerspective();
  bool        getEnabled();
  glm::fmat4  getProjectionMatrix();

  void        setEnabled(bool enabled_in);
  void        setProjectionMatrix(glm::fmat4);

private:
  bool        m_isPerspective;
  bool        m_isEnabled;
  glm::fmat4  m_projectionMatrix;
};

#endif
