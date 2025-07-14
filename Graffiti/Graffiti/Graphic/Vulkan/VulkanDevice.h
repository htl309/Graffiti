#pragma once
#include"Graffiti/base/core.h"
#include<vector>
#include<vulkan/vulkan.h>
#include"Graffiti/base/Window.h"
struct GLFWwindow;
namespace Graffiti {
    
    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    //队列索引
    //我们调好的队列的索引就放在这里面
    struct QueueFamilyIndices {

        //我们只需要两个队列就行了，一个支持图形渲染，一个支持图形呈现
        uint32_t graphicsFamily;
        uint32_t presentFamily;
        bool graphicsFamilyHasValue = false;
        bool presentFamilyHasValue = false;
        bool isComplete() { return graphicsFamilyHasValue && presentFamilyHasValue; }
    };
    class VulkanDevice {
    public:
        VulkanDevice(std::unique_ptr<Window>& input_window);
        ~VulkanDevice();


        VkCommandPool getCommandPool() { return commandPool; }
        VkDevice device() { return device_; }
        VkSurfaceKHR surface() { return surface_; }
        VkQueue graphicsQueue() { return graphicsQueue_; }
        VkQueue presentQueue() { return presentQueue_; }
        VkPhysicalDevice  getphysicalDevice() { return physicalDevice; }
        VkInstance  getinstance() { return instance; }

        SwapChainSupportDetails getSwapChainSupport() { return querySwapChainSupport(physicalDevice); }
        uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
        QueueFamilyIndices findPhysicalQueueFamilies() { return findQueueFamilies(physicalDevice); }
        VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

        void createBuffer(
            VkDeviceSize size,
            VkBufferUsageFlags usage,
            VkMemoryPropertyFlags properties,
            VkBuffer& buffer,
            VkDeviceMemory& bufferMemory);
        void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

        void createImageWithInfo(
            const VkImageCreateInfo& imageInfo,
            VkMemoryPropertyFlags properties,
            VkImage& image,
            VkDeviceMemory& imageMemory);
        void copyImage(VkBuffer srcBuffer, VkImage image, uint32_t width, uint32_t height);
        void transitionImageLayout(VkCommandBuffer commandBuffer, VkImage image,
            VkFormat format,
            VkImageLayout oldLayout, VkImageLayout newLayout);
        VkImageView createImageView(VkImage image, VkFormat format);
        VkSampler createTextureSampler();
        VkPhysicalDeviceProperties properties;

        VkCommandBuffer beginSingleTimeCommands();
        void endSingleTimeCommands(VkCommandBuffer commandBuffer);


    private:

        std::unique_ptr<Window>& m_Window;

#ifdef NDEBUG
        const bool enableValidationLayers = false;
#else
        const bool enableValidationLayers = false;
#endif
        void createInstance();
        void setupDebugMessenger();
        void createSurface();
        void pickPhysicalDevice();
        void createLogicalDevice();
        void createCommandPool();

        bool isDeviceSuitable(VkPhysicalDevice device);
        //寻找物理设备的队列
        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
        //查找物理设备是否有我们需要的扩展
        bool checkDeviceExtensionSupport(VkPhysicalDevice device);
        std::vector<const char*>  FilterSupportedExtensions(VkPhysicalDevice device,std::vector<std::string>* unsupportedExtensions = nullptr);

        SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
        std::vector<const char*> getRequiredExtensions();

       

        VkInstance instance;
        VkDebugUtilsMessengerEXT debugMessenger;
        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
        VkCommandPool commandPool;

        VkDevice device_ = VK_NULL_HANDLE;
        VkSurfaceKHR surface_;
        VkQueue graphicsQueue_;
        VkQueue presentQueue_;

        const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };
        const std::vector<const char*> deviceExtensions = { 
                                                            VK_KHR_SWAPCHAIN_EXTENSION_NAME,// 确保启用了 swapchain
                                                            VK_NV_MESH_SHADER_EXTENSION_NAME  // 启用 Mesh Shader 扩展 
                                                          };
  
    public:
        inline static std::shared_ptr<VulkanDevice>& GetVulkanDevice() { return m_device; }
        static void Create(std::unique_ptr<Window>& input_window);
    private:
        static std::shared_ptr<VulkanDevice> m_device;
        
    };

}