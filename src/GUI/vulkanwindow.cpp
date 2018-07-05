#include "vulkanwindow.h"

#include <cstdlib>
#include <stdexcept>
#include <functional>
#include <iostream>

#include "VulkanBase/VulkanTools.h"
#include "VulkanBase/VulkanDebug.h"

VulkanWindow::VulkanWindow()
{
	//uint32_t extensionCount = 0;
	//vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
	//std::cout << extensionCount << " extensions supported" << std::endl;
#if _DEBUG
	EnableDebugLayersAndExtensions();
#endif
	EnableExtensions();

	CreateInstance();
	vks::debug::setupDebugging(m_instance, VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT, VK_NULL_HANDLE);
}
//======================================================================================
VulkanWindow::~VulkanWindow()
{
	CleanUp();
}
//======================================================================================
#if _DEBUG
void VulkanWindow::EnableDebugLayersAndExtensions()
{
	// Specify debug report extension to be able to consume validation errors
	m_instanceExtensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);

	// Specify instance validation layers
	m_instanceLayers.push_back("VK_LAYER_LUNARG_standard_validation");

	// Specify device validation layers
	m_deviceLayers.push_back("VK_LAYER_LUNARG_standard_validation");
}
#endif
//======================================================================================
void VulkanWindow::EnableExtensions()
{
	// Enable extensions used in all build types
	m_instanceExtensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
	m_deviceExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

#if VK_USE_PLATFORM_WIN32_KHR
	m_instanceExtensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);

#elif VK_USE_PLATFORM_XCB_KHR
	m_instanceExtensions.push_back(VK_KHR_XCB_SURFACE_EXTENSION_NAME);
#endif
}
//======================================================================================
void VulkanWindow::CreateInstance()
{
	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Vulkan Demo App";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "No Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;
	createInfo.enabledExtensionCount = static_cast<uint32_t> (m_instanceExtensions.size());
	createInfo.ppEnabledExtensionNames = static_cast<const char *const *> (m_instanceExtensions.data());
	createInfo.enabledLayerCount = static_cast<uint32_t> (m_instanceLayers.size());
	createInfo.ppEnabledLayerNames = static_cast<const char *const *> (m_instanceLayers.data());

	VK_CHECK_RESULT(vkCreateInstance(&createInfo, nullptr, &m_instance));
}
//======================================================================================
void VulkanWindow::CleanUp()
{
	vks::debug::freeDebugCallback(m_instance);
	vkDestroyInstance(m_instance, nullptr);
}
//======================================================================================
//======================================================================================