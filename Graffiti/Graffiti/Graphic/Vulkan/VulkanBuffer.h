#pragma once
#include"Graffiti/Render/Buffer.h"
#include"VulkanDevice.h"
#include "Graffiti/Scene/Vertex.h"

namespace Graffiti {
    class VulkanBuffer {
    public:
        VulkanBuffer(
            std::shared_ptr<VulkanDevice> device,
            VkDeviceSize instanceSize,
            uint32_t instanceCount,
            VkBufferUsageFlags usageFlags,
            VkMemoryPropertyFlags memoryPropertyFlags,
            VkDeviceSize minOffsetAlignment = 1);
        ~VulkanBuffer();

        VulkanBuffer(const VulkanBuffer&) = delete;
        VulkanBuffer& operator=(const VulkanBuffer&) = delete;

        VkResult map(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
        void unmap();

        void writeToBuffer(const void* data, VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
        VkResult flush(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
        VkDescriptorBufferInfo descriptorInfo(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
        VkResult invalidate(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);

        void writeToIndex(void* data, int index);
        VkResult flushIndex(int index);
        VkDescriptorBufferInfo descriptorInfoForIndex(int index);
        VkResult invalidateIndex(int index);

        VkBuffer getBuffer() const { return buffer; }
        void* getMappedMemory() const { return mapped; }
        uint32_t getInstanceCount() const { return instanceCount; }
        VkDeviceSize getInstanceSize() const { return instanceSize; }
        VkDeviceSize getAlignmentSize() const { return instanceSize; }
        VkBufferUsageFlags getUsageFlags() const { return usageFlags; }
        VkMemoryPropertyFlags getMemoryPropertyFlags() const { return memoryPropertyFlags; }
        VkDeviceSize getBufferSize() const { return bufferSize; }

    private:
        static VkDeviceSize getAlignment(VkDeviceSize instanceSize, VkDeviceSize minOffsetAlignment);

        std::shared_ptr<VulkanDevice> Device;
        void* mapped = nullptr;
        VkBuffer buffer = VK_NULL_HANDLE;
        VkDeviceMemory memory = VK_NULL_HANDLE;

        VkDeviceSize bufferSize;
        uint32_t instanceCount;
        VkDeviceSize instanceSize;
        VkDeviceSize alignmentSize;
        VkBufferUsageFlags usageFlags;
        VkMemoryPropertyFlags memoryPropertyFlags;
    };

	class VulkanVertexBuffer :public VertexBuffer
	{
	public:
		VulkanVertexBuffer(std::vector<Vertex>& vertices, uint32_t count);

		virtual ~VulkanVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetLayout(const BufferLayout& layout) override;
        virtual const BufferLayout& GetLayout()const override;

    private:
        std::shared_ptr<VulkanBuffer> m_VertexBuffer;
       
	};

    class VulkanIndexBuffer :public IndexBuffer
    {
    public:
        VulkanIndexBuffer(std::vector<uint32_t>& indices, uint32_t count);

        virtual ~VulkanIndexBuffer();

        virtual void Bind() const override;
        virtual void Unbind() const override;

        virtual uint32_t GetCount() const { return m_Count; }

    private:
        std::shared_ptr<VulkanBuffer> m_IndexBuffer;
        uint32_t m_Count;
    };

    class VulkanUniformBuffer : public UniformBuffer
    {
    public:
        VulkanUniformBuffer(uint32_t unitsize, uint32_t count,uint32_t set, uint32_t binding);
        virtual ~VulkanUniformBuffer();

        virtual void SetData(const void* data, uint32_t size = 0, uint32_t offset = 0) override;

        VkDescriptorBufferInfo GetDescriptorInfo();
    private:
        std::unique_ptr<VulkanBuffer> m_UniformBuffer;
    };
    class VulkanStorageBuffer : public StorageBuffer
    {
    public:
        VulkanStorageBuffer(uint32_t unitsize, uint32_t count, uint32_t set, uint32_t binding);
        virtual ~VulkanStorageBuffer();

        virtual void SetData(const void* data, uint32_t size = 0, uint32_t offset = 0) override;

        VkDescriptorBufferInfo GetDescriptorInfo();
    private:
        std::unique_ptr<VulkanBuffer> m_StorageBuffer;
    };
    

   
}


