#include "camera.h"
glm::mat4 Camera::GetViewMatrix(){
	m_view_matrix = glm::lookAt(m_position, m_position + m_direction, m_world_up);
	return m_view_matrix;
}

//要求先set 相关成员变量再 get 投影矩阵，怎么改？？
glm::mat4 Camera::GetProjectionMatrix() const
{
	return m_project_matrix;
}

void Camera::set_position(glm::vec3 in_pos)
{
	m_position = in_pos;
}

glm::vec3 Camera::position() const {
	return m_position;
}

void Camera::set_direcction(glm::vec3 in_dir)
{
	m_direction = in_dir;
}
glm::vec3 Camera::direction() const {
	return m_direction;
}

void Camera::set_world_up(glm::vec3 in_up)
{
	m_world_up = in_up;
}

glm::vec3 Camera::world_up()
{
	return m_world_up;
}

// 
void Camera::SetEuler(float pitch, float yaw, float roll)
{
	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	m_direction = glm::normalize(direction);
}

void Camera::SetPerspective(float fovy, float aspect, float near, float far)
{
	m_fovy = fovy;
	m_aspect = aspect;
	m_near = near;
	m_far = far;
	m_project_matrix = glm::perspective<float>(m_fovy, m_aspect, m_near, m_far);

}
