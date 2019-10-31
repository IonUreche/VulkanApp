#include "Camera.h"

#include <glm/gtx/rotate_vector.hpp>
#include <algorithm>

namespace utils
{
const float k_defaultMovementSpeed = 1.0f;
const float k_fastModeMovementSpeed = 3.0f;
const float k_deltaTimeFactor = 100.0f;

Camera::Camera(float width, float height, float nearPlane/* = 0.1f*/, float farPlane/* = 1000.0f*/, float fov/* = 45.0f*/) :
	m_width(width), m_height(height), m_nearPlane(nearPlane), m_farPlane(farPlane), m_fov(fov)
{
	m_cameraDirection = glm::vec3(1.0f, 0.0f, 0.0f);
	m_cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
}
//======================================================================================
Camera::~Camera()
{

}
//======================================================================================
void Camera::Update(float deltaTime)
{
	UpdatePosition(deltaTime);
	ReconstructMatrices();
}
//======================================================================================
void Camera::UpdatePosition(float deltaTime)
{
 	if (!m_moveDirectionMask) return;
	bool f = m_moveDirectionMask & MoveDirectionMasks::k_forward;
	bool b = m_moveDirectionMask & MoveDirectionMasks::k_backwards;
	bool r = m_moveDirectionMask & MoveDirectionMasks::k_right;
	bool l = m_moveDirectionMask & MoveDirectionMasks::k_left;
	bool u = m_moveDirectionMask & MoveDirectionMasks::k_up;
	bool d = m_moveDirectionMask & MoveDirectionMasks::k_down;
	bool moveF = f && !b;
	bool moveB = b && !f;
	bool moveR = r && !l;
	bool moveL = l && !r;
	bool moveU = u && !d;
	bool moveD = d && !u;
	float dx = (moveF ? 1.0f : (moveB ? -1.0f : 0.0f));
	float dy = (moveL ? 1.0f : (moveR ? -1.0f : 0.0f));
	float dz = (moveU ? 1.0f : (moveD ? -1.0f : 0.0f));
	
	glm::vec3 displacement;
	if (dx != 0.0f) displacement += dx * (m_moveSpeed * k_deltaTimeFactor * deltaTime) * m_cameraDirection;
	if (dy != 0.0f) displacement += dy * (m_moveSpeed * k_deltaTimeFactor * deltaTime) * m_cameraRight;
	if (dz != 0.0f) displacement += dz * (m_moveSpeed * k_deltaTimeFactor * deltaTime) * m_cameraUp;

	m_cameraPos += displacement;
}
//======================================================================================
void Camera::ReconstructMatrices()
{
	glm::vec3 rotCamDir = glm::vec3(1.0f, 0.0f, 0.0f);

	rotCamDir.x = cos(glm::radians(m_rotAngleY)) * cos(glm::radians(m_rotAngleX));
	rotCamDir.y = sin(glm::radians(m_rotAngleY));
	rotCamDir.z = cos(glm::radians(m_rotAngleY)) * sin(glm::radians(m_rotAngleX));
	rotCamDir = glm::normalize(rotCamDir);
	m_cameraDirection = rotCamDir;

	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	m_cameraRight = glm::normalize(glm::cross(up, rotCamDir));
	m_cameraUp = glm::normalize(glm::cross(rotCamDir, m_cameraRight));

	m_view = glm::lookAt(m_cameraPos,
		m_cameraPos + rotCamDir,
		m_cameraUp);

	m_projection = glm::perspective(glm::radians(m_fov),
		m_width / m_height, m_nearPlane, m_farPlane);
}
//======================================================================================
void Camera::SetMoveDirection(uint32_t moveDirMask)
{
	m_moveDirectionMask = moveDirMask;
}
//======================================================================================
void Camera::SetFastSpeedMode(bool status)
{
	m_moveSpeed = (status ? k_fastModeMovementSpeed : k_defaultMovementSpeed);
}
//======================================================================================
void Camera::OnMouseMoveX(float amount)
{
	m_rotAngleX += amount * m_cameraViewRotationSpeed;
}
//======================================================================================
void Camera::OnMouseMoveY(float amount)
{
	m_rotAngleY = std::min(85.0f, std::max(-85.0f, m_rotAngleY + amount * m_cameraViewRotationSpeed));
}
//======================================================================================
glm::mat4 Camera::GetView()
{
	return m_view;
}
//======================================================================================
glm::mat4 Camera::GetProjection()
{
	return m_projection;
}
}
//======================================================================================
//======================================================================================