#include "gfpch.h"
#include "VulkanImGuiLayer.h"


#include "Graffiti/ImGui/imgui_impl_glfw.h"
#include "Graffiti/ImGui/imgui_impl_vulkan.h"

#include "VulkanDevice.h"
#include "VulkanContext.h"
#include "Graffiti/Render/Renderer.h"
#include "Graffiti/base/Texture.h"

Graffiti::VulkanImGuiLayer::VulkanImGuiLayer():ImGuiLayer(){}

Graffiti::VulkanImGuiLayer::~VulkanImGuiLayer() {}


void Graffiti::VulkanImGuiLayer::OnAttach()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    SetStyle();

    createUIDescriptorPool();
    createUICommandPool(&uiCommandPool, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);

    std::shared_ptr<VulkanDevice> Device = VulkanDevice::GetVulkanDevice();
    auto context = std::dynamic_pointer_cast<VulkanContext>(GraphicsContext::G_Context[Render::ContextIndex()]);

    ImGui_ImplGlfw_InitForVulkan(static_cast<GLFWwindow*>(context->m_WindowHandle->GetNativeWindow()), true);
    ImGui_ImplVulkan_InitInfo init_info = {};
    init_info.Instance = Device->getinstance();
    init_info.PhysicalDevice = Device->getphysicalDevice();
    init_info.Device = Device->device();
    init_info.QueueFamily = Device->findPhysicalQueueFamilies().graphicsFamily;
    init_info.Queue = Device->graphicsQueue();



    init_info.DescriptorPool = uiDescriptorPool;
    init_info.RenderPass = context->getSwapChainRenderPass();
    init_info.Subpass = 0;
    init_info.MinImageCount = 2;
    init_info.ImageCount = 2;
    init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
    init_info.Allocator = nullptr;
    init_info.CheckVkResultFn = check_vk_result;
    ImGui_ImplVulkan_Init(&init_info);
}

void Graffiti::VulkanImGuiLayer::OnDetach()
{
    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    vkDestroyDescriptorPool(VulkanDevice::GetVulkanDevice()->device(), uiDescriptorPool, nullptr);

    vkDestroyCommandPool(VulkanDevice::GetVulkanDevice()->device(), uiCommandPool, nullptr);
}

void Graffiti::VulkanImGuiLayer::Begin()
{
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void Graffiti::VulkanImGuiLayer::End()
{
    ImGuiIO& io = ImGui::GetIO();
    auto context = std::dynamic_pointer_cast<VulkanContext>(GraphicsContext::G_Context[Render::ContextIndex()]);
    io.DisplaySize = ImVec2((float)context->m_WindowHandle->GetWidth(), (float)context->m_WindowHandle->GetHeight());

    // Rendering
    ImGui::Render();
    ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), context->getCurrentCommandBuffer());
}

ImTextureID  Graffiti::VulkanImGuiLayer::GetTextureID(std::shared_ptr<Texture> Texture)
{
    std::shared_ptr<VulkanTexture> texture = std::dynamic_pointer_cast<VulkanTexture>(Texture);
    VkDescriptorSet descriptorSet = ImGui_ImplVulkan_AddTexture(
        texture->GetSampler(),         // 采样器
        texture->GetImageView(),       // 图像视图
        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL // 纹理布局
    );
    return (ImTextureID)descriptorSet;
}




void Graffiti::VulkanImGuiLayer::createUIDescriptorPool()
{
    VkDescriptorPoolSize pool_sizes[] = {
                { VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
                { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
                { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
                { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
                { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
                { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
                { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
                { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
                { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
                { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
                { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
    };

    VkDescriptorPoolCreateInfo pool_info = {};
    pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
    pool_info.maxSets = 1000 * IM_ARRAYSIZE(pool_sizes);
    pool_info.poolSizeCount = static_cast<uint32_t>(IM_ARRAYSIZE(pool_sizes));
    pool_info.pPoolSizes = pool_sizes;
    if (vkCreateDescriptorPool(VulkanDevice::GetVulkanDevice()->device(), &pool_info, nullptr, &uiDescriptorPool) != VK_SUCCESS) {
        throw std::runtime_error("Cannot allocate UI descriptor pool!");
    }
}

void Graffiti::VulkanImGuiLayer::createUICommandPool(VkCommandPool* cmdPool, VkCommandPoolCreateFlags flags)
{
    std::shared_ptr<VulkanDevice> Device = VulkanDevice::GetVulkanDevice();
    VkCommandPoolCreateInfo commandPoolCreateInfo = {};
    commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    commandPoolCreateInfo.queueFamilyIndex = Device->findPhysicalQueueFamilies().graphicsFamily;
    commandPoolCreateInfo.flags = flags;

    if (vkCreateCommandPool(Device->device(), &commandPoolCreateInfo, nullptr, cmdPool) != VK_SUCCESS) {
        throw std::runtime_error("Could not create graphics command pool!");
    }
}

void Graffiti::VulkanImGuiLayer::check_vk_result(VkResult err)
{
    if (err == 0)
        return;
    fprintf(stderr, "[vulkan] Error: VkResult = %d\n", err);
    if (err < 0)
        abort();
}