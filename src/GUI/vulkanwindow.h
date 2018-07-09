#pragma once

#include "vulkan/vulkan.h"
#include <QWindow>
#include <vector>

struct QueueFamilyIndices
{
	int graphicsFamily = -1;
	int presentFamily = -1;
	bool isComplete()
	{
		return graphicsFamily >= 0 && presentFamily >= 0;	}
};

struct SwapChainSupportDetails
{
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

class VulkanWindow : public QWindow
{
    Q_OBJECT
public:
    VulkanWindow();
    ~VulkanWindow();

	void CreateInstance();
	void PickPhysicalDevice();
	void CreateLogicalDevice();

	bool IsDeviceSuitable(VkPhysicalDevice device);
	bool CheckDeviceExtensionSupport(VkPhysicalDevice device);

#if _DEBUG
	void EnableDebugLayersAndExtensions();
#endif
	void EnableExtensions();

	void CleanUp();

	QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
	SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);

	void CreateSurface();

private:

	QVector<const char *> m_instanceLayers;
	QVector<const char *> m_instanceExtensions;
	QVector<const char *> m_deviceLayers;
	QVector<const char *> m_deviceExtensions;

	VkInstance					m_instance;
	VkPhysicalDevice			m_physicalDevice = VK_NULL_HANDLE;
	VkDevice					m_device;
	VkSurfaceKHR				m_surface;
	VkQueue						m_presentQueue;
};