#include "window.h"

#include <chrono>
#include <QTimer>

Window::Window(int width, int height) : m_width(width), m_height(height)
{
	m_vulkanWindow = new VulkanWindow(m_width, m_height, static_cast<uint32_t>(this->winId()));

	QTimer* graphics_timer = new QTimer;
	graphics_timer->setInterval(1);
	connect(graphics_timer, &QTimer::timeout, this, &Window::update);
	graphics_timer->start();
}
//======================================================================================
Window::~Window()
{
	delete m_vulkanWindow;
}
//======================================================================================
void Window::update()
{
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
//======================================================================================