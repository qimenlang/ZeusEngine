#include "Camera.h"

#include "config.h"

Camera::Camera(glm::vec3 position, glm::vec3 worldUp, float yaw, float pitch)
    : m_front(glm::vec3(0.0f, 0.0f, -1.0f)),
      m_movement_speed(SPEED),
      m_mouse_sensitivity(SENSITIVITY),
      m_zoom(ZOOM) {
    m_position = position;
    m_world_up = worldUp;
    m_yaw = yaw;
    m_pitch = pitch;
    updateCameraVectors();

    m_pjt_para.AspectRatio = Zeus::SCR_WIDTH / float(Zeus::SCR_HEIGHT);
    m_projection =
        glm::perspective(glm::radians(m_pjt_para.FOV), m_pjt_para.AspectRatio,
                         m_pjt_para.Near, m_pjt_para.Far);
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY,
               float upZ, float yaw, float pitch)
    : m_front(glm::vec3(0.0f, 0.0f, -1.0f)),
      m_movement_speed(SPEED),
      m_mouse_sensitivity(SENSITIVITY),
      m_zoom(ZOOM) {
    m_position = glm::vec3(posX, posY, posZ);
    m_world_up = glm::vec3(upX, upY, upZ);
    m_yaw = yaw;
    m_pitch = pitch;
    updateCameraVectors();

    m_pjt_para.AspectRatio = Zeus::SCR_WIDTH / float(Zeus::SCR_HEIGHT);
    m_projection =
        glm::perspective(glm::radians(m_pjt_para.FOV), m_pjt_para.AspectRatio,
                         m_pjt_para.Near, m_pjt_para.Far);
}

void Camera::ProcessMovement(Camera_Movement direction, float deltaTime) {
    float velocity = m_movement_speed * deltaTime;
    if (direction == Camera_Movement::FORWARD) m_position += m_front * velocity;
    if (direction == Camera_Movement::BACKWARD)
        m_position -= m_front * velocity;
    if (direction == Camera_Movement::LEFT) m_position -= m_right * velocity;
    if (direction == Camera_Movement::RIGHT) m_position += m_right * velocity;
    if (direction == Camera_Movement::UP) m_position += m_up * velocity;
    if (direction == Camera_Movement::DOWN) m_position -= m_up * velocity;
}

void Camera::ProcessRotation(Camera_Rotation direction, float deltaTime) {
    float yawOffset = deltaTime * 10.0f;  // scale factor
    if (direction == Camera_Rotation::LEFT)
        m_yaw -= yawOffset;
    else if (direction == Camera_Rotation::RIGHT)
        m_yaw += yawOffset;

    // update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors();
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset,
                                  bool constrainPitch) {
    xoffset *= m_mouse_sensitivity;
    yoffset *= m_mouse_sensitivity;

    m_yaw += xoffset;
    m_pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch) {
        if (m_pitch > 89.0f) m_pitch = 89.0f;
        if (m_pitch < -89.0f) m_pitch = -89.0f;
    }

    // update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset) {
    m_zoom -= (float)yoffset;
    if (m_zoom < 1.0f) m_zoom = 1.0f;
    if (m_zoom > 45.0f) m_zoom = 45.0f;
}

void Camera::updateCameraVectors() {
    // calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    front.y = sin(glm::radians(m_pitch));
    front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_front = glm::normalize(front);
    // also re-calculate the Right and Up vector
    m_right = glm::normalize(glm::cross(
        m_front, m_world_up));  // normalize the vectors, because their length
                                // gets closer to 0 the more you look up or down
                                // which results in slower movement.
    m_up = glm::normalize(glm::cross(m_right, m_front));
}