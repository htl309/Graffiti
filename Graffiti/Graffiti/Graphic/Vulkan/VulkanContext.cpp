#include "gfpch.h"

#include"VulkanContext.h"

namespace Graffiti {



    VulkanContext::VulkanContext(std::unique_ptr<Window>  windowHandle)
    {

        m_WindowHandle = std::move(windowHandle);
        VulkanDevice::Create(m_WindowHandle);
        std::shared_ptr<VulkanDevice> Device = VulkanDevice::GetVulkanDevice();
       
        PhysicalGPU_Name = Device->properties.deviceName;
        GraphicAPI_Version = "Vulkan" +
                            std::to_string(VK_VERSION_MAJOR(Device->properties.apiVersion)) + "." +
                            std::to_string(VK_VERSION_MINOR(Device->properties.apiVersion));
    }

    VulkanContext::~VulkanContext()
    {
        freeCommandBuffers();
    }

    void VulkanContext::Init()
    {
        recreateSwapChain();
        createCommandBuffers();
    }


    void VulkanContext::BeginFrame() {

        assert(!isFrameStarted && "Can't call beginFrame while already in progress");
        VkResult result = m_SwapChain->acquireNextImage(&currentImageIndex);
        if (result == VK_ERROR_OUT_OF_DATE_KHR) {
            recreateSwapChain();
            return;
        }

        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            throw std::runtime_error("failed to acquire swap chain image!");
        }

        isFrameStarted = true;

        VkCommandBuffer commandBuffer = getCurrentCommandBuffer();
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
            throw std::runtime_error("failed to begin recording command buffer!");
        }

        m_CommandBuffers[currentFrameIndex] = commandBuffer;
    }

    void VulkanContext::EndFrame() {
        assert(isFrameStarted && "Can't call endFrame while frame is not in progress");
        auto commandBuffer = getCurrentCommandBuffer();
        if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
            throw std::runtime_error("failed to record command buffer!");
        }

        auto result = m_SwapChain->submitCommandBuffers(&commandBuffer, &currentImageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
            m_WindowHandle->IsWindowResized()) {
            m_WindowHandle->ResetWindowResizedFlag();
            recreateSwapChain();
        }
        else if (result != VK_SUCCESS) {
            throw std::runtime_error("failed to present swap chain image!");
        }

        isFrameStarted = false;
        currentFrameIndex = (currentFrameIndex + 1) % VulkanSwapChain::MAX_FRAMES_IN_FLIGHT;
    }

    void VulkanContext::BeginSwapChainRenderPass() {
        assert(isFrameStarted && "Can't call beginSwapChainRenderPass if frame is not in progress");


        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = m_SwapChain->getRenderPass();
        renderPassInfo.framebuffer = m_SwapChain->getFrameBuffer(currentImageIndex);

        renderPassInfo.renderArea.offset = { 0, 0 };
        renderPassInfo.renderArea.extent = m_SwapChain->getSwapChainExtent();

        //±³¾°
        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = { 0.8f, 0.82f, 0.82f, 1.0f };
        clearValues[1].depthStencil = { 1.0f, 0 };
        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(m_CommandBuffers[currentFrameIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(m_SwapChain->getSwapChainExtent().width);
        viewport.height = static_cast<float>(m_SwapChain->getSwapChainExtent().height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        VkRect2D scissor{ {0, 0}, m_SwapChain->getSwapChainExtent() };
        vkCmdSetViewport(m_CommandBuffers[currentFrameIndex], 0, 1, &viewport);
        vkCmdSetScissor(m_CommandBuffers[currentFrameIndex], 0, 1, &scissor);
    }

    void VulkanContext::SwapBuffers() {
        m_WindowHandle->OnUpdate();
        GF_ASSERT(isFrameStarted, "Can't call endSwapChainRenderPass if frame is not in progress");
        GF_ASSERT(m_CommandBuffers[currentFrameIndex] == getCurrentCommandBuffer(),
            "Can't end render pass on command buffer from a different frame");
        vkCmdEndRenderPass(m_CommandBuffers[currentFrameIndex]);
    }




    void VulkanContext::recreateSwapChain()
    {
        VkExtent2D extent{ m_WindowHandle->GetWidth(), m_WindowHandle->GetHeight() };

        while (extent.width == 0 || extent.height == 0) {
            extent = { m_WindowHandle->GetWidth(), m_WindowHandle->GetHeight() };
            m_WindowHandle->WaitForEvents();
        }
        vkDeviceWaitIdle(VulkanDevice::GetVulkanDevice()->device());

        if (m_SwapChain == nullptr) {
            m_SwapChain = std::make_unique<VulkanSwapChain>(extent);
        }
        else {
            std::shared_ptr<VulkanSwapChain> oldSwapChain = std::move(m_SwapChain);
            m_SwapChain = std::make_unique<VulkanSwapChain>(extent, oldSwapChain);

            if (!oldSwapChain->compareSwapFormats(*m_SwapChain.get())) {
                throw std::runtime_error("Swap chain image(or depth) format has changed!");
            }
        }
    }

    void VulkanContext::createCommandBuffers()
    {

        m_CommandBuffers.resize(VulkanSwapChain::MAX_FRAMES_IN_FLIGHT);

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = VulkanDevice::GetVulkanDevice()->getCommandPool();
        allocInfo.commandBufferCount = static_cast<uint32_t>(m_CommandBuffers.size());

        if (vkAllocateCommandBuffers(VulkanDevice::GetVulkanDevice()->device(), &allocInfo, m_CommandBuffers.data()) !=
            VK_SUCCESS) {
            throw std::runtime_error("failed to allocate command buffers!");
        }

    }
    void VulkanContext::freeCommandBuffers()
    {
        std::shared_ptr<VulkanDevice> Device = VulkanDevice::GetVulkanDevice();
        vkFreeCommandBuffers(
            Device->device(),
            Device->getCommandPool(),
            static_cast<uint32_t>(m_CommandBuffers.size()),
            m_CommandBuffers.data());
        m_CommandBuffers.clear();
    }
}