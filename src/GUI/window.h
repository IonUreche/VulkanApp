#pragma once

#include <QWindow>
#include <QEvent>
#include <QKeyEvent>
#include <QMouseEvent>

#include <chrono>

#include "Vulkan/vulkanwindow.h"
namespace utils
{
	class Camera;
}

class Window : public QWindow
{
    Q_OBJECT
public:
	Window(int width, int height);
    ~Window();

	bool event(QEvent* ev) override;
	virtual void keyPressEvent(QKeyEvent* ev) override;
	virtual void keyReleaseEvent(QKeyEvent* ev) override;
	virtual void mouseMoveEvent(QMouseEvent* ev) override;
	virtual void mousePressEvent(QMouseEvent* ev) override;
	virtual void mouseReleaseEvent(QMouseEvent* ev) override;

public slots:
	void update();

private:

	VulkanWindow* m_vulkanWindow;
	utils::Camera*	m_camera;

	int m_width;
	int m_height;

	float m_time;
	std::chrono::steady_clock::time_point m_startTime;

	uint32_t m_moveDirMask;

	QPointF m_lastMousePos;

	bool m_mouseLeftPressed = false;
	bool m_mouseRightPressed = false;
	QPointF m_leftClickPos;
	QPointF m_rightClickPos;
};