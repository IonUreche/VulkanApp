#pragma once

#include "vulkan/vulkan.h"
#include <vector>
#include <array>
#include <unordered_map>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>

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

struct Vertex {
	glm::vec3 pos;
	glm::vec3 color;
	glm::vec2 texCoord;

	bool operator==(const Vertex& other) const
	{
		return pos == other.pos && color == other.color && texCoord == other.texCoord;
	}

	static VkVertexInputBindingDescription
		getBindingDescription() {
		VkVertexInputBindingDescription bindingDescription = {};
		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(Vertex);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		return bindingDescription;
	}

	static std::array<VkVertexInputAttributeDescription, 3>
		getAttributeDescriptions() {
		std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions = {};
		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(Vertex, pos);

		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(Vertex, color);

		attributeDescriptions[2].binding = 0;
		attributeDescriptions[2].location = 2;
		attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

		return attributeDescriptions;
	}
};

namespace std {
	template<> struct hash<Vertex>
	{
		size_t operator()(Vertex const& vertex) const
		{
			return ((hash<glm::vec3>()(vertex.pos) ^
				(hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^
				(hash<glm::vec2>()(vertex.texCoord) << 1);
		}
	};
}

class VulkanWindow
{
public:
    VulkanWindow(int width, int height, uint32_t winId);
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

	void CleanUpSwapChain();
	void RecreateSwapChain();

	QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
	SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);

	void CreateSurface(uint32_t winId);
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

	VkImageView CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels);

	void UpdateUniformBuffer(uint32_t currentImage);

	// Helper functions
	void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, 
		VkBuffer& buffer, VkDeviceMemory& bufferMemory);
	void CreateImage(uint32_t width, uint32_t height, uint32_t mipLevels, VkFormat format,
		VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, 
		VkImage& image, VkDeviceMemory& imageMemory);
	VkCommandBuffer BeginSingleTimeCommands();
	void EndSingleTimeCommands(VkCommandBuffer commandBuffer);
	void TransitionImageLayout(VkImage image, VkFormat format, 
		VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels);
	void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

	void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

	void GenerateMipmaps(VkImage image, int32_t texWidth, int32_t texHeight, uint32_t mipLevels);

	void DrawFrame();
	
	VkShaderModule CreateShaderModule(const std::vector<char>& code);

	VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes);
	VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

	static std::vector<char> readFile(const std::string& filename);

	uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

	void LoadModel();

private:

	uint32_t m_winId;
	int m_width;
	int m_height;

	std::vector<const char*>		m_instanceLayers;
	std::vector<const char*>		m_instanceExtensions;
	std::vector<const char*>		m_deviceLayers;
	std::vector<const char*>		m_deviceExtensions;

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
	uint32_t						m_mipLevels;

	// Draw Data
	std::vector<Vertex> m_vertices;
	std::vector<uint32_t> m_indices;

	std::unordered_map<Vertex, uint32_t> m_uniqueVertices = {};

};