#include "window.h"

#include <QTimer>

#include "Utils/Camera.h"

Window::Window(int width, int height) : m_width(width), m_height(height),
	m_time(0.0f)
{
	SetFocus(reinterpret_cast<HWND>(this->winId()));
	m_camera = new utils::Camera(width, height);
	m_vulkanWindow = new VulkanWindow(m_width, m_height, static_cast<uint32_t>(this->winId()));
	m_moveDirMask = 0;

	QTimer* graphics_timer = new QTimer;
	graphics_timer->setInterval(15);
	connect(graphics_timer, &QTimer::timeout, this, &Window::update);
	graphics_timer->start();

	m_startTime = std::chrono::high_resolution_clock::now();
}
//======================================================================================
Window::~Window()
{
	delete m_vulkanWindow;
}
//======================================================================================
void Window::update()
{
	auto currentTime = std::chrono::high_resolution_clock::now();
	float deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - m_startTime).count();
	m_startTime = currentTime;

	m_time += deltaTime;

	m_camera->SetMoveDirection(m_moveDirMask);
	m_camera->Update(deltaTime);
	m_vulkanWindow->SetView(m_camera->GetView());
	m_vulkanWindow->SetProjection(m_camera->GetProjection());
	m_vulkanWindow->DrawFrame();
}
//======================================================================================
bool Window::event(QEvent *ev)
{
	//if (ev->type() == QEvent::Show)
	//{
	//}
	return QWindow::event(ev);
}
//======================================================================================
void Window::keyPressEvent(QKeyEvent* ev)
{
	QWindow::keyPressEvent(ev);

	if (ev->key() == Qt::Key::Key_W)
	{
		m_moveDirMask |= utils::MoveDirectionMasks::k_forward;
	}
	if (ev->key() == Qt::Key::Key_S)
	{
		m_moveDirMask |= utils::MoveDirectionMasks::k_backwards;
	}
	if (ev->key() == Qt::Key::Key_D)
	{
		m_moveDirMask |= utils::MoveDirectionMasks::k_right;
	}
	if (ev->key() == Qt::Key::Key_A)
	{
		m_moveDirMask |= utils::MoveDirectionMasks::k_left;
	}
	if (ev->key() == Qt::Key::Key_E)
	{
		m_moveDirMask |= utils::MoveDirectionMasks::k_up;
	}
	if (ev->key() == Qt::Key::Key_Q)
	{
		m_moveDirMask |= utils::MoveDirectionMasks::k_down;
	}
	if (ev->key() == Qt::Key::Key_Shift)
	{
		m_camera->SetFastSpeedMode(true);
	}
}
//======================================================================================
void Window::keyReleaseEvent(QKeyEvent* ev)
{
	if (ev->key() == Qt::Key::Key_W)
	{
		m_moveDirMask &= ~utils::MoveDirectionMasks::k_forward;
	}
	if (ev->key() == Qt::Key::Key_S)
	{
		m_moveDirMask &= ~utils::MoveDirectionMasks::k_backwards;
	}
	if (ev->key() == Qt::Key::Key_D)
	{
		m_moveDirMask &= ~utils::MoveDirectionMasks::k_right;
	}
	if (ev->key() == Qt::Key::Key_A)
	{
		m_moveDirMask &= ~utils::MoveDirectionMasks::k_left;
	}
	if (ev->key() == Qt::Key::Key_E)
	{
		m_moveDirMask &= ~utils::MoveDirectionMasks::k_up;
	}
	if (ev->key() == Qt::Key::Key_Q)
	{
		m_moveDirMask &= ~utils::MoveDirectionMasks::k_down;
	}
	if (ev->key() == Qt::Key::Key_Shift)
	{
		m_camera->SetFastSpeedMode(false);
	}
}
//======================================================================================
void Window::mouseMoveEvent(QMouseEvent* ev)
{
	QPointF currentMousePos = ev->screenPos();

	if (m_mouseLeftPressed)
	{
		QPointF delta = currentMousePos - m_lastMousePos;

		float dXf = static_cast<float>(delta.x()) / static_cast<float>(m_width);
		float dYf = static_cast<float>(delta.y()) / static_cast<float>(m_height);

		m_camera->OnMouseMoveX(dXf);
		m_camera->OnMouseMoveY(-dYf);
	}
	m_lastMousePos = currentMousePos;
}
//======================================================================================
void Window::mousePressEvent(QMouseEvent* ev)
{
	if (ev->button() == Qt::MouseButton::LeftButton)
	{
		m_mouseLeftPressed = true;
		m_leftClickPos = ev->screenPos();
	}
	if (ev->button() == Qt::MouseButton::RightButton)
	{
		m_mouseRightPressed = true;
		m_rightClickPos = ev->screenPos();
	}
}
//======================================================================================
void Window::mouseReleaseEvent(QMouseEvent* ev)
{
	if (ev->button() == Qt::MouseButton::LeftButton)
	{
		m_mouseLeftPressed = false;
	}
	if (ev->button() == Qt::MouseButton::RightButton)
	{
		m_mouseRightPressed = false;
	}
}
//======================================================================================
//======================================================================================