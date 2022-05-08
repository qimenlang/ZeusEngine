#include "camera.h"
glm::mat4 Camera::GetViewMatrix(){
	view_matrix_ = glm::lookAt(position_, position_ + direction_, world_up_);
	return view_matrix_;
}

//要求先set 相关成员变量再 get 投影矩阵，怎么改？？
glm::mat4 Camera::GetProjectionMatrix() const
{
	return project_matrix_;
}

void Camera::set_position(glm::vec3 in_pos)
{
	position_ = in_pos;
}

glm::vec3 Camera::position() const {
	return position_;
}

void Camera::set_direcction(glm::vec3 in_dir)
{
	direction_ = in_dir;
}
glm::vec3 Camera::direction() const {
	return direction_;
}

void Camera::set_world_up(glm::vec3 in_up)
{
	world_up_ = in_up;
}

glm::vec3 Camera::world_up()
{
	return world_up_;
}

// 
void Camera::SetEuler(float pitch, float yaw, float roll)
{
	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction_ = glm::normalize(direction);
}

void Camera::SetPerspective(float fovy, float aspect, float near, float far)
{
	fovy_ = fovy;
	aspect_ = aspect;
	near_ = near;
	far_ = far;
	project_matrix_ = glm::perspective<float>(fovy_, aspect_, near_, far_);

}
