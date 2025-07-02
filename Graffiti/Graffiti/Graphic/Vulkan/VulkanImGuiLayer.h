#pragma once

#include"vulkan/vulkan.h"
#include"Graffiti/base/ImGuiLayer.h"
#include "VulkanTexture.h"
#include "Graffiti/ImGui/imgui.h"
namespace Graffiti {

	class VulkanImGuiLayer : public ImGuiLayer {
	public:
		VulkanImGuiLayer();
		~VulkanImGuiLayer();
		
		
		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void Begin();
		virtual void End();
		static ImTextureID GetTextureID(std::shared_ptr<Texture> Texture);
	private:
		void createUIDescriptorPool();
		void createUICommandPool(VkCommandPool* cmdPool, VkCommandPoolCreateFlags flags);

		static void check_vk_result(VkResult err);

		VkDescriptorPool uiDescriptorPool;
		VkCommandPool uiCommandPool;
	};

}