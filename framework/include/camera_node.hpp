#ifndef CAMERANODE_HPP
#define CAMERANODE_HPP

//#include <string.h>
#include "model.hpp"
#include "node.hpp"
#include <glm/glm.hpp>

class CameraNode : public Node {


 public:
 CameraNode():
	Node{},
	m_isPerspective{false},
	m_isEnabled{false},
	m_projectionMatrix{0}
	{}

CameraNode(bool enabled_in, glm::fmat4 mat_in):
	m_isPerspective{false},
	m_isEnabled{enabled_in},
	m_projectionMatrix{mat_in}
	{}

  bool        getPerspective();
  bool        getEnabled();
  glm::fmat4  getProjectionMatrix();

  void        setEnabled(bool enabled_in);
  void        setProjectionMatrix(glm::fmat4 mat_in);

private:
  bool        m_isPerspective;
  bool        m_isEnabled;
  glm::fmat4  m_projectionMatrix;
};

#endif
