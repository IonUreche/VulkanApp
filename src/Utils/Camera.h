#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL 
#include <glm/glm.hpp>

namespace utils
{
namespace MoveDirectionMasks
{
	const uint32_t k_none = 0;
	const uint32_t k_forward = 1;
	const uint32_t k_backwards = 2;
	const uint32_t k_right = 4;
	const uint32_t k_left = 8;
	const uint32_t k_up = 16;
	const uint32_t k_down = 32;
};

class Camera
{
public:
	Camera(float width, float height, float nearPlane = 0.1f, float farPlane = 10000.0f, float fov = 60.0f);
	~Camera();

	void Update(float deltaTime);

	void SetMoveDirection(uint32_t moveDirMask);
	void SetFastSpeedMode(bool status);

	void OnMouseMoveX(float amount);
	void OnMouseMoveY(float amount);

	glm::mat4 GetView();
	glm::mat4 GetProjection();

protected:

	void ReconstructMatrices();
	void UpdatePosition(float deltaTime);

private:
	
	uint32_t m_moveDirectionMask;

	float m_moveSpeed = 1.0f;
	float m_cameraViewRotationSpeed = 90.0f;
	
	float m_width;
	float m_height;
	float m_nearPlane;
	float m_farPlane;
	float m_fov;

	float m_rotAngleX = 0.0f;
	float m_rotAngleY = 0.0f;

	glm::vec3 m_cameraPos;
	glm::vec3 m_cameraDirection;
	glm::vec3 m_cameraUp;
	glm::vec3 m_cameraRight;

	glm::mat4 m_view;
	glm::mat4 m_projection;
};
}