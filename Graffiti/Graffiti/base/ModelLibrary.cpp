#include"gfpch.h"

#include"ModelLibrary.h"
#include"Graffiti/Render/RenderCommand.h"
namespace Graffiti {



    void ModelLibrary::Draw()
    {
        for (int i = 0; i < m_VertexArrays.size(); i++) {
            m_VertexArrays[i]->Bind();
            RenderCommand::DrawIndex(m_VertexArrays[i]);
        }
    }

    void ModelLibrary::LoadGLTFModel(std::string filepath, std::shared_ptr<ModelLibrary>& modellibrary)
    {
        //  if(modellibrary==nullptr)
        modellibrary = std::make_shared<ModelLibrary>();
        tinygltf::TinyGLTF loader;
        tinygltf::Model gltfmodel;
        std::string err, warn;

        // 加载 glTF 文件（ASCII）
        bool result = loader.LoadASCIIFromFile(&gltfmodel, &err, &warn, filepath);

        if (!warn.empty()) GF_CORE_WARN(warn);
        GF_CORE_ASSERT(err.empty(), err);
        GF_CORE_ASSERT(result, "GLTFModel Loading Failed");

        for (const auto& texture : gltfmodel.images) {
            modellibrary->m_Textures.push_back(Texture::Create(texture));
        }

        modellibrary->LoadGLTFModels(gltfmodel);


        modellibrary->CreateVertexArray();

        return;
    }



    void ModelLibrary::CreateVertexArray()
    {

        for (const auto model : m_Models)
        {
            auto VertexArray = VertexArray::Create();
            VertexArray->AddModel(model);
            m_VertexArrays.push_back(VertexArray);
        }
    }

    void ModelLibrary::ApplyTransformToBoundingBox(BoundingBox& box, const glm::mat4& transform)
    {
        glm::vec3 min = box.minPos;
        glm::vec3 max = box.maxPos;

        std::array<glm::vec3, 8> corners = {
            glm::vec3(min.x, min.y, min.z),
            glm::vec3(min.x, min.y, max.z),
            glm::vec3(min.x, max.y, min.z),
            glm::vec3(min.x, max.y, max.z),
            glm::vec3(max.x, min.y, min.z),
            glm::vec3(max.x, min.y, max.z),
            glm::vec3(max.x, max.y, min.z),
            glm::vec3(max.x, max.y, max.z)
        };

        glm::vec3 newMin(FLT_MAX);
        glm::vec3 newMax(-FLT_MAX);

        for (const auto& corner : corners) {
            glm::vec3 transformed = glm::vec3(transform * glm::vec4(corner, 1.0f));
            newMin = glm::min(newMin, transformed);
            newMax = glm::max(newMax, transformed);
        }

        box.minPos = newMin;
        box.maxPos = newMax;
    }

    //void ModelLibrary::GenerateTangentsFromVertices(std::shared_ptr<ModelLibrary> modellibrary)
    //{
    //    for (int i = 0; i < modellibrary->m_Models.size(); i++) {
    //        std::vector<Vertex>& vertices = modellibrary->m_Models[i]->m_Vertices;
    //        const std::vector<uint32_t>& indices = modellibrary->m_Models[i]->m_Indices;
    //        GenerateTangentsFromVertices(vertices, indices);
    //    }
    //}

    //void ModelLibrary::GenerateTangentsFromVertices(
    //    std::vector<Vertex>& vertices,
    //    const std::vector<uint32_t>& indices
    //) {
    //    size_t vertexCount = vertices.size();
    //    std::vector<glm::vec3> tan1(vertexCount, glm::vec3(0.0f));
    //    std::vector<glm::vec3> tan2(vertexCount, glm::vec3(0.0f));

    //    for (size_t i = 0; i + 2 < indices.size(); i += 3) {
    //        uint32_t i0 = indices[i];
    //        uint32_t i1 = indices[i + 1];
    //        uint32_t i2 = indices[i + 2];

    //        const glm::vec3& p0 = vertices[i0].position;
    //        const glm::vec3& p1 = vertices[i1].position;
    //        const glm::vec3& p2 = vertices[i2].position;

    //        const glm::vec2& uv0 = vertices[i0].texCoord;
    //        const glm::vec2& uv1 = vertices[i1].texCoord;
    //        const glm::vec2& uv2 = vertices[i2].texCoord;

    //        glm::vec3 edge1 = p1 - p0;
    //        glm::vec3 edge2 = p2 - p0;

    //        glm::vec2 deltaUV1 = uv1 - uv0;
    //        glm::vec2 deltaUV2 = uv2 - uv0;

    //        float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);

    //        glm::vec3 tangent = f * (edge1 * deltaUV2.y - edge2 * deltaUV1.y);
    //        glm::vec3 bitangent = f * (edge2 * deltaUV1.x - edge1 * deltaUV2.x);

    //        tan1[i0] += tangent;
    //        tan1[i1] += tangent;
    //        tan1[i2] += tangent;

    //        tan2[i0] += bitangent;
    //        tan2[i1] += bitangent;
    //        tan2[i2] += bitangent;
    //    }

    //    for (size_t i = 0; i < vertexCount; ++i) {
    //        const glm::vec3& n = vertices[i].normal;
    //        const glm::vec3& t = tan1[i];

    //        // 正交化 + 归一化
    //        glm::vec3 tangent = glm::normalize(t - n * glm::dot(n, t));
    //        vertices[i].tangent = tangent; // 存入你的 Vertex
    //    }
    //}

}