#pragma once

#include "vulkan/vulkan.h"
#include <QWindow>
#include <vector>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

const int MAX_FRAMES_IN_FLIGHT = 2;

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

struct UniformBufferObject {
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 proj;
};

class VulkanWindow : public QWindow
{
    Q_OBJECT
public:
    VulkanWindow(int width, int height);
    ~VulkanWindow();

	bool event(QEvent *ev) override;

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

	void CleanUpSwapChain();
	void RecreateSwapChain();

	QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
	SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);

	void CreateSurface();
	void CreateSwapChain();
	void CreateImageViews();
	void CreateRenderPass();
	void CreateGraphicsPipeline();
	void CreateFramebuffers();
	void CreateCommandPool();
	void CreateCommandBuffers();
	void CreateSyncObjects();
	void CreateVertexBuffers();
	void CreateIndexBuffers();
	void CreateUniformBuffers();
	void CreateDescriptorSetLayout();
	void CreateDescriptorPool();
	void CreateDescriptorSets();
	void CreateTextureImage();
	void CreateTextureImageView();
	void CreateTextureSampler();
	void CreateDepthResources();

	VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
	VkFormat FindDepthFormat();
	bool HasStencilComponent(VkFormat format);

	VkImageView CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);

	void UpdateUniformBuffer(uint32_t currentImage);

	// Helper functions
	void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, 
		VkBuffer& buffer, VkDeviceMemory& bufferMemory);
	void CreateImage(uint32_t width, uint32_t height, VkFormat format, 
		VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, 
		VkImage& image, VkDeviceMemory& imageMemory);
	VkCommandBuffer BeginSingleTimeCommands();
	void EndSingleTimeCommands(VkCommandBuffer commandBuffer);
	void TransitionImageLayout(VkImage image, VkFormat format, 
		VkImageLayout oldLayout, VkImageLayout newLayout);
	void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

	void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

	void MainLoop();
	void DrawFrame();
	
	VkShaderModule CreateShaderModule(const std::vector<char>& code);

	VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes);
	VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

	static std::vector<char> readFile(const std::string& filename);

	uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

public slots:
	void update();

private:

	int m_width;
	int m_height;

	QVector<const char*>			m_instanceLayers;
	QVector<const char*>			m_instanceExtensions;
	QVector<const char*>			m_deviceLayers;
	QVector<const char*>			m_deviceExtensions;

	VkRenderPass					m_renderPass;
	VkDescriptorSetLayout			m_descriptorSetLayout;
	VkDescriptorPool				m_descriptorPool;
	std::vector<VkDescriptorSet>	m_descriptorSets;

	VkPipelineLayout				m_pipelineLayout;
	VkPipeline						m_graphicsPipeline;
	VkCommandPool					m_commandPool;

	std::vector<VkSemaphore>		m_imageAvailableSemaphores;
	std::vector<VkSemaphore>		m_renderFinishedSemaphores;
	std::vector<VkFence>			m_inFlightFences;

	VkInstance						m_instance;
	VkPhysicalDevice				m_physicalDevice = VK_NULL_HANDLE;
	VkDevice						m_device;
	VkSurfaceKHR					m_surface;
	VkQueue							m_presentQueue;
	VkQueue							m_graphicsQueue;
	VkSwapchainKHR					m_swapChain;
	std::vector<VkImage>			m_swapChainImages;
	VkFormat						m_swapChainImageFormat;
	VkExtent2D						m_swapChainExtent;
	std::vector<VkImageView>		m_swapChainImageViews;
	std::vector<VkFramebuffer>		m_swapChainFramebuffers;
	std::vector<VkCommandBuffer>	m_commandBuffers;

	VkBuffer						m_vertexBuffer;
	VkDeviceMemory					m_vertexBufferMemory;
	VkBuffer						m_indexBuffer;
	VkDeviceMemory					m_indexBufferMemory;
	VkImage							m_textureImage;
	VkImageView						m_textureImageView;
	VkSampler						m_textureSampler;
	VkDeviceMemory					m_textureImageMemory;

	VkImage							m_depthImage;
	VkDeviceMemory					m_depthImageMemory;
	VkImageView						m_depthImageView;

	std::vector<VkBuffer>			m_uniformBuffers;
	std::vector<VkDeviceMemory>		m_uniformBuffersMemory;

	size_t							m_currentFrame = 0;

	bool							m_createdCommandBuffers;

	bool							m_canDraw = false;
};