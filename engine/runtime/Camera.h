#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Defines several possible options for camera movement. Used as abstraction to
// stay away from window-system specific input methods
enum class Camera_Movement { FORWARD, BACKWARD, LEFT, RIGHT, UP, DOWN };
enum class Camera_Rotation {
    LEFT,
    RIGHT,
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.03f;
const float ZOOM = 45.0f;

// An abstract camera class that processes input and calculates the
// corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera {
   private:
    // camera Attributes
    glm::vec3 m_position;
    glm::vec3 m_front;
    glm::vec3 m_up;
    glm::vec3 m_right;
    glm::vec3 m_world_up;

    struct ProjectionParams {
        float FOV = 45.0f;
        float AspectRatio = 16.0f / 9.0f;
        float Near = 0.1f;
        float Far = 500.0f;
    };
    // euler Angles
    float m_yaw;
    float m_pitch;
    // camera options
    float m_movement_speed;
    float m_mouse_sensitivity;
    float m_zoom;

    ProjectionParams m_pjt_para;
    glm::mat4 m_projection;

   public:
    bool m_is_rotation_mode = false;
    // constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
           glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW,
           float pitch = PITCH);
    // constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ,
           float yaw, float pitch);

    void setWorldPosition(glm::vec3 worldPos) { m_position = worldPos; };
    glm::vec3 worldPosition() const { return m_position; };

    // returns the view matrix calculated using Euler Angles and the LookAt
    // Matrix
    glm::mat4 GetViewMatrix() {
        return glm::lookAt(m_position, m_position + m_front, m_up);
    }

    glm::mat4 GetProjectionMatrix() { return m_projection; }

    glm::vec3 position() const { return m_position; }

    glm::vec3 front() const { return m_front; }

    glm::vec3 up() const { return m_up; }

    glm::vec3 right() const { return m_right; }

    ProjectionParams pjt_para() const { return m_pjt_para; }

    // processes input received from any keyboard-like input system. Accepts
    // input parameter in the form of camera defined ENUM (to abstract it from
    // windowing systems)
    void ProcessMovement(Camera_Movement direction, float deltaTime);

    void ProcessRotation(Camera_Rotation direction, float deltaTime);

    // processes input received from a mouse input system. Expects the offset
    // value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset,
                              bool constrainPitch = true);

    // processes input received from a mouse scroll-wheel event. Only requires
    // input on the vertical wheel-axis
    void ProcessMouseScroll(float yoffset);

   private:
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors();
};
