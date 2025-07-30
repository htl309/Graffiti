#include"gfpch.h"

#include"Model.h"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include<tiny_gltf.h>
namespace Graffiti {
    void Model::LoadGLTFModel(std::string filepath, std::shared_ptr<Model>& model)

    {
        model = std::make_shared<Model>();
        tinygltf::TinyGLTF loader;
        tinygltf::Model gltfmodel;
        std::string err, warn;

        // 加载 glTF 文件（ASCII）
        bool result = loader.LoadASCIIFromFile(&gltfmodel, &err, &warn, filepath);

        if (!warn.empty()) GF_CORE_WARN(warn);
        GF_CORE_ASSERT(err.empty(), err);
        GF_CORE_ASSERT(result, "GLTFModel Loading Failed");


        for (const auto& mesh : gltfmodel.meshes) {
            for (const auto& primitive : mesh.primitives) {
                // === 1. 获取顶点属性 ===
                // 位置属性 (必须)
                const auto& posIt = primitive.attributes.find("POSITION");
                if (posIt == primitive.attributes.end()) continue;
                const tinygltf::Accessor& posAccessor = gltfmodel.accessors[posIt->second];
                const tinygltf::BufferView& posView = gltfmodel.bufferViews[posAccessor.bufferView];
                const float* posBuffer = reinterpret_cast<const float*>(
                    &gltfmodel.buffers[posView.buffer].data[posView.byteOffset + posAccessor.byteOffset]
                    );

                // 法线属性 (可选)
                const float* normalBuffer = nullptr;
                const auto& normIt = primitive.attributes.find("NORMAL");
                if (normIt != primitive.attributes.end()) {
                    const tinygltf::Accessor& normAccessor = gltfmodel.accessors[normIt->second];
                    const tinygltf::BufferView& normView = gltfmodel.bufferViews[normAccessor.bufferView];
                    normalBuffer = reinterpret_cast<const float*>(
                        &gltfmodel.buffers[normView.buffer].data[normView.byteOffset + normAccessor.byteOffset]
                        );
                }

                // 纹理坐标属性 (可选)
                const float* texCoordBuffer = nullptr;
                const auto& texIt = primitive.attributes.find("TEXCOORD_0");
                if (texIt != primitive.attributes.end()) {
                    const tinygltf::Accessor& texAccessor = gltfmodel.accessors[texIt->second];
                    const tinygltf::BufferView& texView = gltfmodel.bufferViews[texAccessor.bufferView];
                    texCoordBuffer = reinterpret_cast<const float*>(
                        &gltfmodel.buffers[texView.buffer].data[texView.byteOffset + texAccessor.byteOffset]
                        );
                }

                // === 2. 创建顶点数组 ===
                const size_t vertexCount = posAccessor.count;
                model->m_Vertices.resize(vertexCount);

                for (size_t i = 0; i < vertexCount; ++i) {
                    // 位置 (vec3)
                    model->m_Vertices[i].position = glm::make_vec3(&posBuffer[i * 3]);

                    model->boundingbox.minPos = glm::min(model->boundingbox.minPos, model->m_Vertices[i].position);
                    model->boundingbox.maxPos = glm::max(model->boundingbox.maxPos, model->m_Vertices[i].position);
                    // 法线 (vec3)
                    if (normalBuffer) {
                        model->m_Vertices[i].normal = glm::make_vec3(&normalBuffer[i * 3]);
                    }

                    // 纹理坐标 (vec2)
                    if (texCoordBuffer) {
                        model->m_Vertices[i].texCoord = glm::make_vec2(&texCoordBuffer[i * 2]);
                    }
                }

                // === 3. 获取索引数据 ===
                if (primitive.indices >= 0) {
                    const tinygltf::Accessor& indexAccessor = gltfmodel.accessors[primitive.indices];
                    const tinygltf::BufferView& indexView = gltfmodel.bufferViews[indexAccessor.bufferView];
                    const void* indexBuffer = &gltfmodel.buffers[indexView.buffer].data[
                        indexView.byteOffset + indexAccessor.byteOffset
                    ];

                    const size_t indexCount = indexAccessor.count;
                    model->m_Indices.resize(indexCount);

                    // 根据索引类型转换数据
                    switch (indexAccessor.componentType) {
                    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
                        for (size_t i = 0; i < indexCount; ++i) {
                            model->m_Indices[i] = static_cast<const uint8_t*>(indexBuffer)[i];
                        }
                        break;

                    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
                        for (size_t i = 0; i < indexCount; ++i) {
                            model->m_Indices[i] = static_cast<const uint16_t*>(indexBuffer)[i];
                        }
                        break;

                    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
                        memcpy(model->m_Indices.data(), indexBuffer, indexCount * sizeof(unsigned int));
                        break;

                    default:
                        // 不支持的索引类型
                        return;
                    }
                }
            }
        }
        return;
    }
    void Model::LoadGLTFModel(std::string filepath, std::shared_ptr<Model>& model, std::shared_ptr<Texture>& texture)
    {
        model = std::make_shared<Model>();
        tinygltf::TinyGLTF loader;
        tinygltf::Model gltfmodel;
        std::string err, warn;

        // 加载 glTF 文件（ASCII）
        bool result = loader.LoadASCIIFromFile(&gltfmodel, &err, &warn, filepath);

        texture = Texture::Create(gltfmodel.images[0]);

        if (!warn.empty()) GF_CORE_WARN(warn);
        GF_CORE_ASSERT(err.empty(), err);
        GF_CORE_ASSERT(result, "GLTFModel Loading Failed");
        for (const auto& mesh : gltfmodel.meshes) {
            for (const auto& primitive : mesh.primitives) {
                // === 1. 获取顶点属性 ===
                // 位置属性 (必须)
                const auto& posIt = primitive.attributes.find("POSITION");
                if (posIt == primitive.attributes.end()) continue;
                const tinygltf::Accessor& posAccessor = gltfmodel.accessors[posIt->second];
                const tinygltf::BufferView& posView = gltfmodel.bufferViews[posAccessor.bufferView];
                const float* posBuffer = reinterpret_cast<const float*>(
                    &gltfmodel.buffers[posView.buffer].data[posView.byteOffset + posAccessor.byteOffset]
                    );

                // 法线属性 (可选)
                const float* normalBuffer = nullptr;
                const auto& normIt = primitive.attributes.find("NORMAL");
                if (normIt != primitive.attributes.end()) {
                    const tinygltf::Accessor& normAccessor = gltfmodel.accessors[normIt->second];
                    const tinygltf::BufferView& normView = gltfmodel.bufferViews[normAccessor.bufferView];
                    normalBuffer = reinterpret_cast<const float*>(
                        &gltfmodel.buffers[normView.buffer].data[normView.byteOffset + normAccessor.byteOffset]
                        );
                }

                // 纹理坐标属性 (可选)
                const float* texCoordBuffer = nullptr;
                const auto& texIt = primitive.attributes.find("TEXCOORD_0");
                if (texIt != primitive.attributes.end()) {
                    const tinygltf::Accessor& texAccessor = gltfmodel.accessors[texIt->second];
                    const tinygltf::BufferView& texView = gltfmodel.bufferViews[texAccessor.bufferView];
                    texCoordBuffer = reinterpret_cast<const float*>(
                        &gltfmodel.buffers[texView.buffer].data[texView.byteOffset + texAccessor.byteOffset]
                        );
                }

                // === 2. 创建顶点数组 ===
                const size_t vertexCount = posAccessor.count;
                model->m_Vertices.resize(vertexCount);

                for (size_t i = 0; i < vertexCount; ++i) {
                    // 位置 (vec3)
                    model->m_Vertices[i].position = glm::make_vec3(&posBuffer[i * 3]);

                    model->boundingbox.minPos = glm::min(model->boundingbox.minPos, model->m_Vertices[i].position);
                    model->boundingbox.maxPos = glm::max(model->boundingbox.maxPos, model->m_Vertices[i].position);
                    // 法线 (vec3)
                    if (normalBuffer) {
                        model->m_Vertices[i].normal = glm::make_vec3(&normalBuffer[i * 3]);
                    }

                    // 纹理坐标 (vec2)
                    if (texCoordBuffer) {
                        model->m_Vertices[i].texCoord = glm::make_vec2(&texCoordBuffer[i * 2]);
                    }
                }

                // === 3. 获取索引数据 ===
                if (primitive.indices >= 0) {
                    const tinygltf::Accessor& indexAccessor = gltfmodel.accessors[primitive.indices];
                    const tinygltf::BufferView& indexView = gltfmodel.bufferViews[indexAccessor.bufferView];
                    const void* indexBuffer = &gltfmodel.buffers[indexView.buffer].data[
                        indexView.byteOffset + indexAccessor.byteOffset
                    ];

                    const size_t indexCount = indexAccessor.count;
                    model->m_Indices.resize(indexCount);

                    // 根据索引类型转换数据
                    switch (indexAccessor.componentType) {
                    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
                        for (size_t i = 0; i < indexCount; ++i) {
                            model->m_Indices[i] = static_cast<const uint8_t*>(indexBuffer)[i];
                        }
                        break;

                    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
                        for (size_t i = 0; i < indexCount; ++i) {
                            model->m_Indices[i] = static_cast<const uint16_t*>(indexBuffer)[i];
                        }
                        break;

                    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
                        memcpy(model->m_Indices.data(), indexBuffer, indexCount * sizeof(unsigned int));
                        break;

                    default:
                        // 不支持的索引类型
                        return;
                    }
                }

            }
        }
        return;
    }


}

