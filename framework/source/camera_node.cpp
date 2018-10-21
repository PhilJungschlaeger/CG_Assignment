#include "camera_node.hpp"


bool CameraNode::getPerspective(){
	return m_isPerspective;
}

bool CameraNode::getEnabled(){
	return m_isEnabled;
}

glm::fmat4 CameraNode::getProjectionMatrix(){
	return m_projectionMatrix;
}

void CameraNode::setEnabled(bool enabled_in){
	m_isEnabled = enabled_in;
}

void CameraNode::setProjectionMatrix(glm::fmat4 mat_in){
	m_projectionMatrix = mat_in;
}