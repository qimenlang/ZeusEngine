#include "camera.h"
void Camera::Update()
{
	view_matrix_ = glm::lookAt(position_, position_ + direction_, world_up_);
	project_matrix_ = glm::perspective<float>(glm::radians(fovy_), aspect_, near_, far_);
}
glm::mat4 Camera::GetViewMatrix(){
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

void Camera::set_aspect(float aspect)
{
	aspect_ = aspect;
}

float Camera::aspect() const
{
	return aspect_;
}

void Camera::set_fovy(float fovy)
{
	fovy_ = fovy;
}

float Camera::fovy() const
{
	return fovy_;
}
