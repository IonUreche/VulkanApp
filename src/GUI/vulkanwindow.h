/*
 * A reimplemented QWindow that is displayed by the Qt GUI. This is where everything related to rendering happens.
 * */

#include "vulkan/vulkan.h"

#ifndef VULKANWINDOW_H
#define VULKANWINDOW_H
#include <QWindow>

class VulkanWindow : public QWindow
{
    Q_OBJECT
public:
    VulkanWindow();
    ~VulkanWindow();

	void CreateInstance();

#if _DEBUG
	void EnableDebugLayersAndExtensions();
#endif
	void EnableExtensions();

	void CleanUp();

private:

	QVector<const char *> m_instanceLayers;
	QVector<const char *> m_instanceExtensions;
	QVector<const char *> m_deviceLayers;
	QVector<const char *> m_deviceExtensions;

	VkInstance m_instance;
};

#endif // VULKANWINDOW_H
