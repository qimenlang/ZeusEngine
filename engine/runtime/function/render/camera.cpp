#include "camera.h"
glm::mat4 Camera::GetViewMatrix() const {
	return m_view_matrix;
}

//要求先set 相关成员变量再 get 投影矩阵，怎么改？？
glm::mat4 Camera::GetProjectionMatrix() const
{
	return m_project_matrix;
}
void Camera::LookAt(glm::vec3 pos, glm::vec3 direction, glm::vec3 up) {
	m_position = pos;
	m_direction = direction;
	m_world_up = up;
	m_view_matrix = glm::lookAt(m_position, m_position+m_direction, m_world_up);
}

void Camera::SetPerspective(float fovy, float aspect, float near, float far)
{
	m_fovy = fovy;
	m_aspect = aspect;
	m_near = near;
	m_far = far;
	m_project_matrix = glm::perspective<float>(m_fovy, m_aspect, m_near, m_far);

}
