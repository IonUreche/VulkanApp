#pragma once

#include <QWindow>
#include "vulkan/vulkan.h"

#include "Vulkan/vulkanwindow.h"

class Window : public QWindow
{
    Q_OBJECT
public:
	Window(int width, int height);
    ~Window();

	bool event(QEvent *ev) override;

public slots:
	void update();

private:

	VulkanWindow* m_vulkanWindow;

	int m_width;
	int m_height;
};