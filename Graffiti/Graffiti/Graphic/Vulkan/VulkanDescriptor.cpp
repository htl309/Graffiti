#include"gfpch.h"
#include "VulkanDescriptor.h"

namespace Graffiti {
	VulkanDescriptorSetLayout::Builder& VulkanDescriptorSetLayout::Builder::addBinding(
		uint32_t binding, 
		VkDescriptorType descriptorType, 
		VkShaderStageFlags stageFlags, 
		uint32_t count){
		GF_CORE_ASSERT(bindings.count(binding) == 0 , "Binding already in use");
		VkDescriptorSetLayoutBinding layoutBinding{};
		layoutBinding.binding = binding;
		layoutBinding.descriptorType = descriptorType;
		layoutBinding.descriptorCount = count;
		layoutBinding.stageFlags = stageFlags;
		bindings[binding] = layoutBinding;
		return *this;
	}

	VulkanDescriptorSetLayout::Builder& VulkanDescriptorSetLayout::Builder::addImageBinding(uint32_t binding)
	{
		VkDescriptorSetLayoutBinding samplerLayoutBinding{};
		samplerLayoutBinding.binding = binding;
		samplerLayoutBinding.descriptorCount = 1;
		samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		samplerLayoutBinding.pImmutableSamplers = nullptr;
		samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

		bindings[binding] = samplerLayoutBinding;
		return *this;
	}

	std::shared_ptr<VulkanDescriptorSetLayout> VulkanDescriptorSetLayout::Builder::build() const {
		return std::make_shared<VulkanDescriptorSetLayout>(bindings);
	}
	VulkanDescriptorSetLayout::VulkanDescriptorSetLayout(std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> in_bindings)
		: descriptorSetLayout{}, bindings{ in_bindings } {
		std::vector<VkDescriptorSetLayoutBinding> setLayoutBindings{};
		for (auto kv : bindings) {
			setLayoutBindings.push_back(kv.second);
		}

		VkDescriptorSetLayoutCreateInfo descriptorSetLayoutInfo{};
		descriptorSetLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		descriptorSetLayoutInfo.bindingCount = static_cast<uint32_t>(setLayoutBindings.size());
		descriptorSetLayoutInfo.pBindings = setLayoutBindings.data();

		if (vkCreateDescriptorSetLayout(
			VulkanDevice::GetVulkanDevice()->device(),
			&descriptorSetLayoutInfo,
			nullptr,
			&descriptorSetLayout) != VK_SUCCESS) {
			throw std::runtime_error("failed to create descriptor set layout!");
		}
	}


	VulkanDescriptorSetLayout::~VulkanDescriptorSetLayout()
	{
		vkDestroyDescriptorSetLayout(VulkanDevice::GetVulkanDevice()->device(),descriptorSetLayout, nullptr);
	}

	// *************** Descriptor Pool Builder *********************
	VulkanDescriptorPool::Builder& VulkanDescriptorPool::Builder::addPoolSize(VkDescriptorType descriptorType, uint32_t count)
	{
		poolSizes.push_back({ descriptorType, count });
		return *this;
	}

	VulkanDescriptorPool::Builder& VulkanDescriptorPool::Builder::setPoolFlags(VkDescriptorPoolCreateFlags flags)
	{
		poolFlags = flags;
		return *this;
	}

	VulkanDescriptorPool::Builder& VulkanDescriptorPool::Builder::setMaxSets(uint32_t count)
	{
		maxSets = count;
		return *this;
	}

	std::shared_ptr<VulkanDescriptorPool> VulkanDescriptorPool::Builder::build() const {
		return std::make_shared<VulkanDescriptorPool>( maxSets, poolFlags, poolSizes);
	}

	VulkanDescriptorPool::VulkanDescriptorPool(
		uint32_t maxSets, 
		VkDescriptorPoolCreateFlags poolFlags, 
		const std::vector<VkDescriptorPoolSize>& poolSizes)
	{
		VkDescriptorPoolCreateInfo descriptorPoolInfo{};
		descriptorPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		descriptorPoolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
		descriptorPoolInfo.pPoolSizes = poolSizes.data();
		descriptorPoolInfo.maxSets = maxSets;
		descriptorPoolInfo.flags = poolFlags;

		if (vkCreateDescriptorPool(VulkanDevice::GetVulkanDevice()->device(), &descriptorPoolInfo, nullptr, &descriptorPool) !=
			VK_SUCCESS) {
			throw std::runtime_error("failed to create descriptor pool!");
		}
	}
	VulkanDescriptorPool::~VulkanDescriptorPool()
	{
		vkDestroyDescriptorPool(VulkanDevice::GetVulkanDevice()->device(), descriptorPool, nullptr);
	}
	bool VulkanDescriptorPool::allocateDescriptor(const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet& descriptor) const
	{
			VkDescriptorSetAllocateInfo allocInfo{};
			allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
			allocInfo.descriptorPool = descriptorPool;
			allocInfo.pSetLayouts = &descriptorSetLayout;
			allocInfo.descriptorSetCount = 1;

			// Might want to create a "DescriptorPoolManager" class that handles this case, and builds
			// a new pool whenever an old pool fills up. But this is beyond our current scope
			if (vkAllocateDescriptorSets(VulkanDevice::GetVulkanDevice()->device(), &allocInfo, &descriptor) != VK_SUCCESS) {
				return false;
			}
			return true;
	}
	void VulkanDescriptorPool::freeDescriptors(std::vector<VkDescriptorSet>& descriptors) const
	{
		vkFreeDescriptorSets(
			VulkanDevice::GetVulkanDevice()->device(),
			descriptorPool,
			static_cast<uint32_t>(descriptors.size()),
			descriptors.data());
	}
	void VulkanDescriptorPool::resetPool()
	{
		vkResetDescriptorPool(VulkanDevice::GetVulkanDevice()->device(), descriptorPool, 0);
	}

	// *************** VulkanDescriptorWriter*********************
	VulkanDescriptorWriter::VulkanDescriptorWriter(VulkanDescriptorSetLayout& setLayout, VulkanDescriptorPool& pool)
		: setLayout{ setLayout }, pool{ pool }, writes{} {}
	VulkanDescriptorWriter& VulkanDescriptorWriter::writeBuffer(uint32_t binding, VkDescriptorBufferInfo* bufferInfo)
	{
		GF_CORE_ASSERT(setLayout.bindings.count(binding) == 1 , "Layout does not contain specified binding");
		
		auto& bindingDescription = setLayout.bindings[binding];
	
		GF_CORE_ASSERT(bindingDescription.descriptorCount == 1 ,
			"Binding single descriptor info, but binding expects multiple");
	
		VkWriteDescriptorSet write{};
		write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		write.descriptorType = bindingDescription.descriptorType;
		write.dstBinding = binding;
		write.pBufferInfo = bufferInfo;
		write.descriptorCount = 1;

		writes.push_back(write);
 
		return *this;
	}
	VulkanDescriptorWriter& VulkanDescriptorWriter::writeImage(uint32_t binding, VkDescriptorImageInfo* imageInfo)
	{
		assert(setLayout.bindings.count(binding) == 1 && "Layout does not contain specified binding");

		auto& bindingDescription = setLayout.bindings[binding];

		assert(
			bindingDescription.descriptorCount == 1 &&
			"Binding single descriptor info, but binding expects multiple");

		VkWriteDescriptorSet write{};
		write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		write.descriptorType = bindingDescription.descriptorType;
		write.dstBinding = binding;
		write.pImageInfo = imageInfo;
		write.descriptorCount = 1;

		writes.push_back(write);
		return *this;
	}
	bool VulkanDescriptorWriter::build(VkDescriptorSet& set)
	{
		bool success = pool.allocateDescriptor(setLayout.getDescriptorSetLayout(), set);
		if (!success) {
			return false;
		}
		overwrite(set);
		return true;
	}
	void VulkanDescriptorWriter::overwrite(VkDescriptorSet& set)
	{
		for (auto& write : writes) {
			write.dstSet = set;
		}
		vkUpdateDescriptorSets(VulkanDevice::GetVulkanDevice()->device(), writes.size(), writes.data(), 0, nullptr);
	}
}
