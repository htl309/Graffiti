#pragma once

#include"Graffiti/Render/GraphicsContext.h"
#include"VulkanDevice.h"
#include"VulkanSwapChain.h"
//前向声明，先声明了，之后传入具体的指针

namespace Graffiti {
	

	class VulkanContext:public GraphicsContext
	{
	public:
		VulkanContext(std::unique_ptr<Window>  windowHandle);
		~VulkanContext();

		virtual void Init() override;

		virtual void BeginFrame() override;
		virtual void BeginSwapChainRenderPass() override;
		virtual void SwapBuffers() override;
		virtual void EndFrame() override;


		VkRenderPass getSwapChainRenderPass() const { return m_SwapChain->getRenderPass(); }
		float getAspectRatio() const { return m_SwapChain->extentAspectRatio(); }
		VkFormat getSwapChainImageFormat() { return m_SwapChain->getSwapChainImageFormat(); }
		VkImageView getImageView(int index) { return  m_SwapChain->getImageView(index); }
		size_t imageCount() { return m_SwapChain->imageCount(); }
		VkExtent2D getSwapChainExtent() { return m_SwapChain->getSwapChainExtent(); }

		bool isFrameInProgress() const { return isFrameStarted; }
		VkCommandBuffer getCurrentCommandBuffer() const {
			assert(isFrameStarted && "Cannot get command buffer when frame not in progress");
			return m_CommandBuffers[currentFrameIndex];
		}

	private:

        void recreateSwapChain();
		void createCommandBuffers();
        void freeCommandBuffers();
	private:
		
		std::unique_ptr<VulkanSwapChain> m_SwapChain;
		std::vector<VkCommandBuffer> m_CommandBuffers;

		

		uint32_t currentImageIndex;
		int currentFrameIndex{ 0 };
		bool isFrameStarted{ false };
	};

}