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
		return graphicsFamily >= 0 && presentFamily >= 0;
	}
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
    VulkanWindow(int width, int height);
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
	void CreateSwapChain();
	void CreateImageViews();
	void CreateGraphicsPipeline();
	void CreateRenderPass();
	VkShaderModule CreateShaderModule(const std::vector<char>& code);

	VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes);
	VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

	static std::vector<char> readFile(const std::string& filename);

private:

	int m_width;
	int m_height;

	QVector<const char *> m_instanceLayers;
	QVector<const char *> m_instanceExtensions;
	QVector<const char *> m_deviceLayers;
	QVector<const char *> m_deviceExtensions;

	VkRenderPass m_renderPass;
	VkPipelineLayout m_pipelineLayout;

	VkInstance					m_instance;
	VkPhysicalDevice			m_physicalDevice = VK_NULL_HANDLE;
	VkDevice					m_device;
	VkSurfaceKHR				m_surface;
	VkQueue						m_presentQueue;
	VkSwapchainKHR				m_swapChain;
	std::vector<VkImage>		m_swapChainImages;
	VkFormat					m_swapChainImageFormat;
	VkExtent2D					m_swapChainExtent;
	std::vector<VkImageView>	m_swapChainImageViews;
};