#pragma once
#include"Graffiti/base/Texture.h"
#include"VulkanBuffer.h"
namespace Graffiti {
	class VulkanTexture : public Texture {
	public:
		VulkanTexture(const std::string& path);
		VulkanTexture(const tinygltf::Image& image);
		~VulkanTexture();

		virtual uint32_t GetWidth()const override { return m_Width; }
		virtual uint32_t GetHeight()const override { return m_Height; }

		virtual uint32_t GetTextureID() const override { return m_TextureID; }

		virtual void Bind(uint32_t slot = 0) const override;

		
		inline VkDescriptorImageInfo GetDescriptorImageInfo() { return m_DescriptorImageInfo; }
		inline uint32_t GetSet() { return m_Set; }
		inline uint32_t GetBinding() { return m_Binding; }
		inline void SetSet(uint32_t Set) { m_Set=Set; }
		inline void SetBinding(uint32_t Binding) { m_Binding=Binding; }

		inline VkImageView GetImageView() { return m_TextureImageView; }
		inline VkSampler GetSampler() { return m_TextureSampler; }
	private:

		void CreatetDescriptorImageInfo();
		void CreateViewandSampler();
	private:
		uint32_t m_Set = 0;
		uint32_t m_Binding = 0;

		uint32_t m_Width;
		uint32_t m_Height;
		std::string m_Path;
		uint32_t m_TextureID;

		VkImage m_TextureImage;
		VkDeviceMemory m_TextureImageMemory;

		VkImageView m_TextureImageView;//纹理图像的图像视图对象
		VkSampler m_TextureSampler;//采样器对象


		VkDescriptorImageInfo m_DescriptorImageInfo{};
	};

}


