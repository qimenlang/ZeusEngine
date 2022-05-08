#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
public:
	Camera() {};
	Camera(glm::vec3 in_pos, glm::vec3 in_dir, glm::vec3 in_up) :position_(in_pos), direction_(in_dir), world_up_(in_up) {};
	~Camera() {};

	void set_position(glm::vec3 in_pos);
	glm::vec3 position() const;
	void set_direcction(glm::vec3 in_dir);
	glm::vec3 direction() const;
	void set_world_up(glm::vec3 in_up);
	glm::vec3 world_up();
	void SetEuler(float pitch, float yaw, float roll);
	void set_aspect(float aspect);
	float aspect() const;
	void set_fovy(float fovy);
	float fovy()const;

	void Update();
	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix() const;

private:
	glm::vec3 position_ = glm::vec3(0, 0, 0);
	glm::vec3 direction_ = glm::vec3(0, 0, -1);
	glm::vec3 world_up_ = glm::vec3(0, 1, 0);
	glm::mat4 view_matrix_;
	float fovy_ = 45.0;
	float aspect_;
	float near_ = 0.1;
	float far_ =100.0;
	glm::mat4 project_matrix_;
};