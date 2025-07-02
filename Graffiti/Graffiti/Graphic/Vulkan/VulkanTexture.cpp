#include "gfpch.h"
#include "VulkanTexture.h"
#include"Graffiti/Render/Renderer.h"
#include<stb_image.h>
namespace Graffiti {
	VulkanTexture::VulkanTexture(const std::string& path)
		:m_Path(path)
	{

		int width, height, channels;
		stbi_set_flip_vertically_on_load(true);
		auto data = stbi_load(m_Path.c_str(), &width, &height, &channels, 0);
		GF_CORE_ASSERT(data, "Failed to load image!");

		m_Width = width;
		m_Height = height;
	
		std::shared_ptr<VulkanDevice> Device = VulkanDevice::GetVulkanDevice();

		VkDeviceSize size = m_Width * m_Height * 4;
		VulkanBuffer stagingBuffer{
			Device,
			size,
			1,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		};
	
		GF_CORE_ASSERT(stagingBuffer.map() == VK_SUCCESS, "vkMapMemory Failed!");
		stagingBuffer.writeToBuffer((void*)data);

		VkImageCreateInfo imageInfo{};
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.imageType = VK_IMAGE_TYPE_2D;
		imageInfo.extent.width = m_Width;
		imageInfo.extent.height = m_Height;
		imageInfo.extent.depth = 1;
		imageInfo.mipLevels = 1;
		imageInfo.arrayLayers = 1;
		imageInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
		imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT ;
		imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		imageInfo.flags = 0;

		Device->createImageWithInfo(
			imageInfo,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			m_TextureImage,
			m_TextureImageMemory);
		Device->copyImage(stagingBuffer.getBuffer(), m_TextureImage, m_Width, m_Height);

		m_TextureImageView = Device->createImageView(m_TextureImage, VK_FORMAT_R8G8B8A8_SRGB);
		m_TextureSampler = Device->createTextureSampler();

		stbi_image_free(data); 	

		CreatetDescriptorImageInfo();
	}
	VulkanTexture::VulkanTexture(const tinygltf::Image& image)
	{
		
	
		auto data = image.image.data();
		GF_CORE_ASSERT(data, "Failed to load image!");

		m_Width = image.width;
		m_Height = image.height;

		std::shared_ptr<VulkanDevice> Device = VulkanDevice::GetVulkanDevice();

		VkDeviceSize size = m_Width * m_Height * 4;
		VulkanBuffer stagingBuffer{
			Device,
			size,
			1,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		};

		GF_CORE_ASSERT(stagingBuffer.map() == VK_SUCCESS, "vkMapMemory Failed!");
		stagingBuffer.writeToBuffer((void*)data);

		VkImageCreateInfo imageInfo{};
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.imageType = VK_IMAGE_TYPE_2D;
		imageInfo.extent.width = m_Width;
		imageInfo.extent.height = m_Height;
		imageInfo.extent.depth = 1;
		imageInfo.mipLevels = 1;
		imageInfo.arrayLayers = 1;
		imageInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
		imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
		imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		imageInfo.flags = 0;

		Device->createImageWithInfo(
			imageInfo,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			m_TextureImage,
			m_TextureImageMemory);
		Device->copyImage(stagingBuffer.getBuffer(), m_TextureImage, m_Width, m_Height);

		m_TextureImageView = Device->createImageView(m_TextureImage, VK_FORMAT_R8G8B8A8_SRGB);
		m_TextureSampler = Device->createTextureSampler();

		

		CreatetDescriptorImageInfo();
	}
	VulkanTexture::~VulkanTexture()
	{
	}
	void VulkanTexture::Bind(uint32_t slot) const
	{
		
	}
	void VulkanTexture::CreatetDescriptorImageInfo()
	{		
		m_DescriptorImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		m_DescriptorImageInfo.imageView = m_TextureImageView;  // 自己创建的 VkImageView
		m_DescriptorImageInfo.sampler = m_TextureSampler;    // 自己创建的 VkSampler
	}
	void VulkanTexture::CreateViewandSampler()
	{
	}
}