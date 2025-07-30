#pragma once

#include"Graffiti/Scene/Model.h"
#include"Graffiti/Render/VertexArray.h"
#include"Graffiti/base/Texture.h"
namespace Graffiti {
  
	class ModelLibrary {
        friend class Render;
		public:
            int count = 0;
			void Draw();
			static void LoadGLTFModel(std::string filepath, std::shared_ptr<ModelLibrary>& modellibrary);
            void LoadGLTFModels(const tinygltf::Model& gltfmodel) {
                const auto& scene = gltfmodel.scenes[gltfmodel.defaultScene];
                for (int nodeIndex : scene.nodes) {
                    TraverseNode(gltfmodel, nodeIndex, glm::mat4(1.0f));
                }
            }

            // === 遍历节点 ===
            void TraverseNode(const tinygltf::Model& gltfmodel, int nodeIndex, const glm::mat4& parentTransform) {
                const auto& node = gltfmodel.nodes[nodeIndex];
                glm::mat4 localTransform = GetNodeTransform(node);
                glm::mat4 globalTransform = parentTransform * localTransform;

                if (node.mesh >= 0) {
                    const auto& mesh = gltfmodel.meshes[node.mesh];
              
    
                    for (const auto& primitive : mesh.primitives) {

                        auto model = std::make_shared<Model>();
                        model->m_Transform = globalTransform;
                        model->m_Name = mesh.name;
         
                        // === 顶点 POSITION ===
                        const auto& posIt = primitive.attributes.find("POSITION");
                        if (posIt == primitive.attributes.end()) continue;
                        const auto& posAccessor = gltfmodel.accessors[posIt->second];
                        const auto& posView = gltfmodel.bufferViews[posAccessor.bufferView];
                        const float* posBuffer = reinterpret_cast<const float*>(
                            &gltfmodel.buffers[posView.buffer].data[posView.byteOffset + posAccessor.byteOffset]
                            );

                        // === NORMAL ===
                        const float* normalBuffer = nullptr;
                        auto normIt = primitive.attributes.find("NORMAL");
                        if (normIt != primitive.attributes.end()) {
                            const auto& normAccessor = gltfmodel.accessors[normIt->second];
                            const auto& normView = gltfmodel.bufferViews[normAccessor.bufferView];
                            normalBuffer = reinterpret_cast<const float*>(
                                &gltfmodel.buffers[normView.buffer].data[normView.byteOffset + normAccessor.byteOffset]
                                );
                        }

                        // === TEXCOORD_0 ===
                        const float* texBuffer = nullptr;
                        auto texIt = primitive.attributes.find("TEXCOORD_0");
                        if (texIt != primitive.attributes.end()) {
                            const auto& texAccessor = gltfmodel.accessors[texIt->second];
                            const auto& texView = gltfmodel.bufferViews[texAccessor.bufferView];
                            texBuffer = reinterpret_cast<const float*>(
                                &gltfmodel.buffers[texView.buffer].data[texView.byteOffset + texAccessor.byteOffset]
                                );
                        }

                        // === 顶点填充 ===
                        size_t vertexCount = posAccessor.count;
                        model->m_Vertices.resize(vertexCount);
                        for (size_t i = 0; i < vertexCount; ++i) {
                            model->m_Vertices[i].position = glm::make_vec4(&posBuffer[i * 3]);
                            model->boundingbox.minPos = glm::min(model->boundingbox.minPos, glm::make_vec3(&posBuffer[i * 3]));
                            model->boundingbox.maxPos = glm::max(model->boundingbox.maxPos, glm::make_vec3(&posBuffer[i * 3]));
                            if (normalBuffer)
                                model->m_Vertices[i].normal = glm::make_vec4(&normalBuffer[i * 3]);
                            if (texBuffer);
                                model->m_Vertices[i].texCoord = glm::make_vec2(&texBuffer[i * 2]);
                        }

                        // === 索引填充 ===
                        if (primitive.indices >= 0) {
                            const auto& indexAccessor = gltfmodel.accessors[primitive.indices];
                            const auto& indexView = gltfmodel.bufferViews[indexAccessor.bufferView];
                            const void* indexBuffer = &gltfmodel.buffers[indexView.buffer].data[
                                indexView.byteOffset + indexAccessor.byteOffset];

                            size_t indexCount = indexAccessor.count;
                            model->m_Indices.resize(indexCount);

                            switch (indexAccessor.componentType) {
                            case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
                                for (size_t i = 0; i < indexCount; ++i)
                                    model->m_Indices[i] = static_cast<const uint8_t*>(indexBuffer)[i];
                                break;
                            case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
                                for (size_t i = 0; i < indexCount; ++i)
                                    model->m_Indices[i] = static_cast<const uint16_t*>(indexBuffer)[i];
                                break;
                            case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
                                memcpy(model->m_Indices.data(), indexBuffer, indexCount * sizeof(uint32_t));
                                break;
                            }
                        }

                        ////////////材质///////////////////////////////
                        if (primitive.material >= 0) {
                            const auto& material = gltfmodel.materials[primitive.material];

                            // Base Color
                            if (material.pbrMetallicRoughness.baseColorTexture.index >= 0) {
                                int texIndex = material.pbrMetallicRoughness.baseColorTexture.index;
                                const auto& tex = gltfmodel.textures[texIndex];
                                model->m_Material.baseColorTexID = tex.source;
                                if (m_Textures[model->m_Material.baseColorTexID]->m_Name.empty())
                                    m_Textures[model->m_Material.baseColorTexID]->m_Name = "TexSampler";
                            }

                            // Normal
                            if (material.normalTexture.index >= 0) {
                                int texIndex = material.normalTexture.index;
                                const auto& tex = gltfmodel.textures[texIndex];
                                model->m_Material.normalTexID = tex.source;
                                if (m_Textures[model->m_Material.normalTexID]->m_Name.empty())
                                    m_Textures[model->m_Material.normalTexID]->m_Name = "NormalTex";
                            }

                            // MetallicRoughness (ARM)
                            if (material.pbrMetallicRoughness.metallicRoughnessTexture.index >= 0) {
                                int texIndex = material.pbrMetallicRoughness.metallicRoughnessTexture.index;
                                const auto& tex = gltfmodel.textures[texIndex];
                                model->m_Material.metallicRoughTexID = tex.source;
                                if (m_Textures[model->m_Material.metallicRoughTexID]->m_Name.empty())
                                    m_Textures[model->m_Material.metallicRoughTexID]->m_Name = "ARMTex";
                            }

                            // AO (Ambient Occlusion)
                            if (material.occlusionTexture.index >= 0) {
                                int texIndex = material.occlusionTexture.index;
                                const auto& tex = gltfmodel.textures[texIndex];
                                model->m_Material.occlusionTexID = tex.source;
                            }

                            // Emissive
                            if (material.emissiveTexture.index >= 0) {
                                int texIndex = material.emissiveTexture.index;
                                const auto& tex = gltfmodel.textures[texIndex];
                                model->m_Material.emissiveTexID = tex.source;
                            }
                        
                        }
                        else {
                      
                        }

                            ApplyTransformToBoundingBox(model->boundingbox, model->m_Transform);
                            boundingbox.minPos = glm::min(model->boundingbox.minPos, boundingbox.minPos);
                            boundingbox.maxPos = glm::max(model->boundingbox.maxPos, boundingbox.maxPos);
                            m_Models.push_back(model);  // 存储该模型
                            count++;
                        
                    }
                }

                // === 遍历子节点 ===
                for (int child : node.children) {
                    TraverseNode(gltfmodel, child, globalTransform);
                }
            }

            glm::mat4 GetNodeTransform(const tinygltf::Node& node) {
                if (!node.matrix.empty()) {
                    return glm::make_mat4(node.matrix.data());
                }

                glm::vec3 T = node.translation.empty() ? glm::vec3(0.0f) : glm::make_vec3(node.translation.data());
                glm::vec3 S = node.scale.empty() ? glm::vec3(1.0f) : glm::make_vec3(node.scale.data());
                glm::quat R = node.rotation.empty() ? glm::quat(1, 0, 0, 0) : glm::make_quat(node.rotation.data());

                return glm::translate(glm::mat4(1.0f), T) * glm::mat4_cast(R) * glm::scale(glm::mat4(1.0f), S);
            }


			BoundingBox boundingbox;
		private:
			void CreateVertexArray();
            void ApplyTransformToBoundingBox(BoundingBox& box, const glm::mat4& transform);

            //static void  GenerateTangentsFromVertices(std::shared_ptr<ModelLibrary> modellibrary);

            //static void ModelLibrary::GenerateTangentsFromVertices(
            //    std::vector<Vertex>& vertices,
            //    const std::vector<uint32_t>& indices
            //);
            std::vector<std::shared_ptr<Model>> m_Models;
            std::vector< std::shared_ptr <VertexArray>> m_VertexArrays;
            std::vector< std::shared_ptr <Texture>> m_Textures;
	};


}