#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
public:
	Camera() {};
	Camera(glm::vec3 in_pos, glm::vec3 in_dir, glm::vec3 in_up) :m_position(in_pos), m_direction(in_dir), m_world_up(in_up) {
		m_view_matrix = glm::lookAt(m_position, m_position + m_direction, m_world_up);
	};
	~Camera() {};

	void LookAt(glm::vec3 in_pos, glm::vec3 in_dir, glm::vec3 in_up);
	void SetPerspective(float fovy,float aspect,float near,float far);
	glm::mat4 GetViewMatrix() const;
	glm::mat4 GetProjectionMatrix() const;
private:
	glm::vec3 m_position;
	glm::vec3 m_direction;
	glm::vec3 m_world_up;
	glm::mat4 m_view_matrix;
	float m_fovy;
	float m_aspect;
	float m_near;
	float m_far;
	glm::mat4 m_project_matrix;
}; 