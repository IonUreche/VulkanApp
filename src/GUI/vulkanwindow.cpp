#include "vulkanwindow.h"

#include <cstdlib>
#include <stdexcept>
#include <functional>
#include <iostream>
#include <set>

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
	CreateSurface();
	PickPhysicalDevice();
	CreateLogicalDevice();
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
void VulkanWindow::PickPhysicalDevice()
{
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(m_instance, &deviceCount, nullptr);

	if (deviceCount == 0)
	{
		throw std::runtime_error("failed to find GPUs with Vulkan support!");
	}

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(m_instance, &deviceCount, devices.data());

	for (const auto& device : devices)
	{
		if (IsDeviceSuitable(device))
		{
			m_physicalDevice = device;
			break;
		}
	}

	//VkPhysicalDeviceProperties deviceProperties;
	//VkPhysicalDeviceFeatures deviceFeatures;
	//vkGetPhysicalDeviceProperties(m_physicalDevice, &deviceProperties);
	//vkGetPhysicalDeviceFeatures(m_physicalDevice, &deviceFeatures);

	if (m_physicalDevice == VK_NULL_HANDLE)
	{
		throw std::runtime_error("failed to find a suitable GPU!");
	}
}
//======================================================================================
bool VulkanWindow::IsDeviceSuitable(VkPhysicalDevice device)
{
	QueueFamilyIndices indices = FindQueueFamilies(device);
	bool extensionsSupported = CheckDeviceExtensionSupport(device);
	bool swapChainAdequate = false;
	if (extensionsSupported)
	{
		SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(device);
		swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
	}
	return indices.isComplete() && extensionsSupported && swapChainAdequate;
}
//======================================================================================
bool VulkanWindow::CheckDeviceExtensionSupport(VkPhysicalDevice device)
{
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());
	std::set<std::string> requiredExtensions(m_deviceExtensions.begin(), m_deviceExtensions.end());
	for (const auto& extension : availableExtensions) {
		requiredExtensions.erase(extension.extensionName);
	}
	return requiredExtensions.empty();}
//======================================================================================
QueueFamilyIndices VulkanWindow::FindQueueFamilies(VkPhysicalDevice device) 
{
	QueueFamilyIndices indices;

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	int i = 0;
	for (const auto& queueFamily : queueFamilies) 
	{
		if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			indices.graphicsFamily = i;
		}

		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, m_surface, &presentSupport);

		if (queueFamily.queueCount > 0 && presentSupport)
		{
			indices.presentFamily = i;
		}

		if (indices.isComplete()) break;

		i++;
	}

	return indices;
}
//======================================================================================
void VulkanWindow::CreateSurface()
{
#if VK_USE_PLATFORM_WIN32_KHR
	VkWin32SurfaceCreateInfoKHR create_info = {};
	create_info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	create_info.hinstance = GetModuleHandle(nullptr);
	create_info.hwnd = reinterpret_cast<HWND> (this->winId());
	VK_CHECK_RESULT(vkCreateWin32SurfaceKHR(m_instance, &create_info, nullptr, &m_surface));
#elif VK_USE_PLATFORM_XCB_KHR
	VkXcbSurfaceCreateInfoKHR create_info = {};
	create_info.sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
	create_info.connection = QX11Info::connection();
	create_info.window = static_cast<xcb_window_t>(this->winId());
	VK_CHECK_RESULT(vkCreateXcbSurfaceKHR(m_instance, &create_info, nullptr, &m_surface));
#endif
}
//======================================================================================
void VulkanWindow::CreateLogicalDevice()
{
	QueueFamilyIndices indices = FindQueueFamilies(m_physicalDevice);
	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<int> uniqueQueueFamilies = { indices.graphicsFamily, indices.presentFamily };
	float queuePriority = 1.0f;	for (int queueFamily : uniqueQueueFamilies) {
		VkDeviceQueueCreateInfo queueCreateInfo = {};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.push_back(queueCreateInfo);
	}	VkPhysicalDeviceFeatures deviceFeatures = {};	VkDeviceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.pQueueCreateInfos = queueCreateInfos.data();	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());	if (m_instanceLayers.size() > 0)
	{
		createInfo.enabledLayerCount = static_cast<uint32_t>(m_instanceLayers.size());
		createInfo.ppEnabledLayerNames = m_instanceLayers.data();
	}
	else
	{
		createInfo.enabledLayerCount = 0;
	}	if(m_deviceExtensions.size() > 0)	{		createInfo.enabledExtensionCount = static_cast<uint32_t>(m_deviceExtensions.size());
		createInfo.ppEnabledExtensionNames = m_deviceExtensions.data();	}	else	{		createInfo.enabledExtensionCount = 0;	}	VK_CHECK_RESULT(vkCreateDevice(m_physicalDevice, &createInfo, nullptr, &m_device));	vkGetDeviceQueue(m_device, indices.presentFamily, 0, &m_presentQueue);}
//======================================================================================
SwapChainSupportDetails VulkanWindow::QuerySwapChainSupport(VkPhysicalDevice device)
{
	SwapChainSupportDetails details;

	VK_CHECK_RESULT(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, m_surface, &details.capabilities));

	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_surface, &formatCount, nullptr);
	if (formatCount != 0)
	{
		details.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_surface, &formatCount, details.formats.data());
	}	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_surface, &presentModeCount, nullptr);
	if (presentModeCount != 0)
	{
		details.presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_surface, &presentModeCount, details.presentModes.data());
	}

	return details;
}
//======================================================================================
void VulkanWindow::CleanUp()
{
	vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
	vkDestroyDevice(m_device, nullptr);
	vks::debug::freeDebugCallback(m_instance);
	vkDestroyInstance(m_instance, nullptr);
}
//======================================================================================
//======================================================================================