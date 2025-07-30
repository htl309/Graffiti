#include "gfpch.h"
#include "VulkanBuffer.h"
#include "VulkanContext.h"
#include "Graffiti/Render/Renderer.h"

namespace Graffiti {

	VulkanVertexBuffer::VulkanVertexBuffer(std::vector<Vertex>& vertices, uint32_t count)
	{
		std::shared_ptr<VulkanDevice> Device = VulkanDevice::GetVulkanDevice();

		GF_ASSERT(count >= 3, "Vertex count must be at least 3");
		VkDeviceSize bufferSize = sizeof(vertices[0]) * count;
		uint32_t vertexSize = sizeof(vertices[0]);

		VulkanBuffer stagingBuffer{
			Device,
			vertexSize,
            count,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		};
		
		if(stagingBuffer.map() != VK_SUCCESS)  GF_CORE_INFO("vkMapMemory Failed!");
		stagingBuffer.writeToBuffer((void*)vertices.data());

		m_VertexBuffer = std::make_unique<VulkanBuffer>(
			Device,
			vertexSize,
            count,
			VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		Device->copyBuffer(stagingBuffer.getBuffer(), m_VertexBuffer->getBuffer(), bufferSize);
	}

	VulkanVertexBuffer::~VulkanVertexBuffer()
	{
	}

	void VulkanVertexBuffer::Bind() const
	{
		auto context = std::dynamic_pointer_cast<VulkanContext>(GraphicsContext::G_Context[Render::ContextIndex()]);
		std::shared_ptr<VulkanDevice> Device = VulkanDevice::GetVulkanDevice();
		VkBuffer buffers[] = { m_VertexBuffer->getBuffer() };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(context->getCurrentCommandBuffer(), 0, 1, buffers, offsets);
	}

	void VulkanVertexBuffer::Unbind() const
	{
	}

	void VulkanVertexBuffer::SetLayout(const BufferLayout& layout)
	{
	}

	const BufferLayout& VulkanVertexBuffer::GetLayout() const
	{
		return BufferLayout();
	}




	VulkanIndexBuffer::VulkanIndexBuffer(std::vector<uint32_t>& indices, uint32_t count)
		:m_Count(count)
	{
		std::shared_ptr<VulkanDevice> Device = VulkanDevice::GetVulkanDevice();
		GF_ASSERT(m_Count >= 3, "Indices count must be at least 3");

		VkDeviceSize bufferSize = sizeof(uint32_t) * m_Count;
		uint32_t indexSize = sizeof(uint32_t);

		VulkanBuffer stagingBuffer{
			Device,
			indexSize,
			m_Count,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		};
		GF_CORE_ASSERT(stagingBuffer.map() == VK_SUCCESS,"vkMapMemory Failed!");
		stagingBuffer.writeToBuffer(indices.data());

		m_IndexBuffer = std::make_unique<VulkanBuffer>(
			Device,
			indexSize,
			m_Count,
			VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		Device->copyBuffer(stagingBuffer.getBuffer(), m_IndexBuffer->getBuffer(), bufferSize);
	}

	VulkanIndexBuffer::~VulkanIndexBuffer()
	{
	}

	void VulkanIndexBuffer::Bind() const
	{
        auto context = std::dynamic_pointer_cast<VulkanContext>(GraphicsContext::G_Context[Render::ContextIndex()]);
        vkCmdBindIndexBuffer(context->getCurrentCommandBuffer(), m_IndexBuffer->getBuffer(), 0, VK_INDEX_TYPE_UINT32);
	}

	void VulkanIndexBuffer::Unbind() const
	{
	}
	
	VulkanUniformBuffer::VulkanUniformBuffer(uint32_t unitsize, uint32_t count, uint32_t set,uint32_t binding)
	{
		
		m_Set = set;
		m_Binding = binding;

		m_UniformBuffer= std::make_unique<VulkanBuffer>(
			VulkanDevice::GetVulkanDevice(),
			unitsize,
			count,
			VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		m_UniformBuffer->map();
		
	}

	VulkanUniformBuffer::~VulkanUniformBuffer()
	{
		
	}

	void VulkanUniformBuffer::SetData(const void* data, uint32_t size, uint32_t offset)
	{
		m_UniformBuffer->writeToBuffer(data, VK_WHOLE_SIZE, offset);
		m_UniformBuffer->flush(); 
	}

	VkDescriptorBufferInfo VulkanUniformBuffer::GetDescriptorInfo()
	{
		return m_UniformBuffer->descriptorInfo();
	}
	
	
	VulkanStorageBuffer::VulkanStorageBuffer(uint32_t unitsize, uint32_t count, uint32_t set, uint32_t binding)
	{

		m_Set = set;
		m_Binding = binding;

		m_StorageBuffer = std::make_unique<VulkanBuffer>(
			VulkanDevice::GetVulkanDevice(),
			unitsize,
			count,
			VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		m_StorageBuffer->map();

	}

	VulkanStorageBuffer::~VulkanStorageBuffer()
	{

	}

	void VulkanStorageBuffer::SetData(const void* data, uint32_t size, uint32_t offset)
	{
		m_StorageBuffer->writeToBuffer(data, VK_WHOLE_SIZE, offset);
		m_StorageBuffer->flush();
	}

	VkDescriptorBufferInfo VulkanStorageBuffer::GetDescriptorInfo()
	{
		return m_StorageBuffer->descriptorInfo();
	}
}

namespace Graffiti{
	VulkanBuffer::VulkanBuffer(std::shared_ptr<VulkanDevice> device, VkDeviceSize instanceSize, uint32_t instanceCount, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags, VkDeviceSize minOffsetAlignment)
		:Device{ device },
		instanceSize{ instanceSize },
		instanceCount{ instanceCount },
		usageFlags{ usageFlags },
		memoryPropertyFlags{ memoryPropertyFlags } 
	{
		alignmentSize = getAlignment(instanceSize, minOffsetAlignment);
		bufferSize = alignmentSize * instanceCount;
		Device->createBuffer(bufferSize, usageFlags, memoryPropertyFlags, buffer, memory);
	}

	VulkanBuffer::~VulkanBuffer()
	{
		unmap(); 
		vkDestroyBuffer(Device->device(), buffer, nullptr);
		vkFreeMemory(Device->device(), memory, nullptr);
	}

	VkResult VulkanBuffer::map(VkDeviceSize size, VkDeviceSize offset)
	{
		
		assert(buffer && memory && "Called map on buffer before create");
		return vkMapMemory(Device->device(), memory, offset, size, 0, &mapped);
	}

	void VulkanBuffer::unmap()
	{
		if (mapped) {
			vkUnmapMemory(Device->device(), memory);
			mapped = nullptr;
		}
	}

	void VulkanBuffer::writeToBuffer(const void* data, VkDeviceSize size, VkDeviceSize offset)
	{
		
		GF_CORE_ASSERT(mapped , "Cannot copy to unmapped buffer");

		if (size == VK_WHOLE_SIZE) {
			memcpy(mapped, data, bufferSize);
		}
		else {
			char* memOffset = (char*)mapped;
			memOffset += offset;
			memcpy(memOffset, data, size);
		}
	}

	VkResult VulkanBuffer::flush(VkDeviceSize size, VkDeviceSize offset)
	{
		VkMappedMemoryRange mappedRange = {};
		mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
		mappedRange.memory = memory;
		mappedRange.offset = offset;
		mappedRange.size = size;
		return vkFlushMappedMemoryRanges(Device->device(), 1, &mappedRange);
	}

	VkDescriptorBufferInfo VulkanBuffer::descriptorInfo(VkDeviceSize size, VkDeviceSize offset)
	{
		return VkDescriptorBufferInfo{
		   buffer,
		   offset,
		   size,
		};
	}

	VkResult VulkanBuffer::invalidate(VkDeviceSize size, VkDeviceSize offset)
	{
		VkMappedMemoryRange mappedRange = {};
		mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
		mappedRange.memory = memory;
		mappedRange.offset = offset;
		mappedRange.size = size;
		return vkInvalidateMappedMemoryRanges(Device->device(), 1, &mappedRange);
	}

	void VulkanBuffer::writeToIndex(void* data, int index)
	{
		writeToBuffer(data, instanceSize, index * alignmentSize);
	}

	VkResult VulkanBuffer::flushIndex(int index)
	{
		return flush(alignmentSize, index * alignmentSize); 
	}

	VkDescriptorBufferInfo VulkanBuffer::descriptorInfoForIndex(int index)
	{
		return descriptorInfo(alignmentSize, index * alignmentSize);
	}

	VkResult VulkanBuffer::invalidateIndex(int index)
	{
		return invalidate(alignmentSize, index * alignmentSize);
	}

	
	VkDeviceSize VulkanBuffer::getAlignment(VkDeviceSize instanceSize, VkDeviceSize minOffsetAlignment) {
		if (minOffsetAlignment > 0) {
			return (instanceSize + minOffsetAlignment - 1) & ~(minOffsetAlignment - 1);
		}
		return instanceSize;
	}
}
