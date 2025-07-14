#pragma once

#include"Graffiti/base/core.h"
#include"VulkanDevice.h"
namespace Graffiti {

	class VulkanDescriptorSetLayout {
    public:
        class Builder {
        public:
            Builder() {};

            Builder& addBinding(uint32_t binding, VkDescriptorType descriptorType,
                VkShaderStageFlags stageFlags, uint32_t count = 1);

            Builder& addImageBinding(uint32_t binding);

            std::shared_ptr<VulkanDescriptorSetLayout> build() const;

            std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings;
        };

        VulkanDescriptorSetLayout(std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> in_bindings);

        ~VulkanDescriptorSetLayout();

        VulkanDescriptorSetLayout(const VulkanDescriptorSetLayout&) = delete;
        VulkanDescriptorSetLayout& operator=(const VulkanDescriptorSetLayout&) = delete;

        VkDescriptorSetLayout getDescriptorSetLayout()  const {
            return descriptorSetLayout;
        }

    public:
        VkDescriptorSetLayout descriptorSetLayout;
        std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings;

        friend class VulkanDescriptorWriter;
	};

	class VulkanDescriptorPool {
    public:
        class Builder {
        public:
            Builder() {};

            Builder& addPoolSize(VkDescriptorType descriptorType, uint32_t count);
            Builder& setPoolFlags(VkDescriptorPoolCreateFlags flags);
            Builder& setMaxSets(uint32_t count);
            std::shared_ptr<VulkanDescriptorPool> build() const;

        private:
            std::vector<VkDescriptorPoolSize> poolSizes{};
            uint32_t maxSets = 10;
            VkDescriptorPoolCreateFlags poolFlags = 0;
        };

        VulkanDescriptorPool(uint32_t maxSets,
            VkDescriptorPoolCreateFlags poolFlags,
            const std::vector<VkDescriptorPoolSize>& poolSizes);

        ~VulkanDescriptorPool();

        VulkanDescriptorPool(const VulkanDescriptorPool&) = delete;
        VulkanDescriptorPool& operator=(const VulkanDescriptorPool&) = delete;

        bool allocateDescriptor(const VkDescriptorSetLayout descriptorSetLayout,
            VkDescriptorSet& descriptor) const;

        void freeDescriptors(std::vector<VkDescriptorSet>& descriptors) const;

        void resetPool();

    private:
        VkDescriptorPool descriptorPool;

        friend class VulkanDescriptorWriter;
    };

    class VulkanDescriptorWriter {
    public:
        VulkanDescriptorWriter(VulkanDescriptorSetLayout& setLayout,
            VulkanDescriptorPool& pool);

        VulkanDescriptorWriter& writeBuffer(uint32_t binding,VkDescriptorBufferInfo* bufferInfo);

        VulkanDescriptorWriter& writeImage(uint32_t binding,VkDescriptorImageInfo* imageInfo);

        bool build(VkDescriptorSet& set);
        void overwrite(VkDescriptorSet& set);

    private:
        VulkanDescriptorSetLayout& setLayout;
        VulkanDescriptorPool& pool;
        std::vector<VkWriteDescriptorSet> writes;
    };
}