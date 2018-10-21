#include "camera_node.hpp"


CameraNode::getPerspective(){
	return m_isPerspective;
}

CameraNode::getEnabled(){
	return m_isEnabled;
}

CameraNode::getProjectionMatrix(){
	return m_projectionMatrix
}

CameraNode::setEnabled(bool enabled_in){
	m_isEnabled = enabled_in;
}

CameraNode::setProjectionMatrix(glm::fmat4 mat_in){
	m_projectionMatrix = mat_in;
}