#pragma once

#include<Graffiti.h>
#include<meshoptimizer.h>
namespace Graffiti {
    class MeshShaderModel {

    public:
        MeshShaderModel() {};

        void createMeshLets(std::shared_ptr <VertexArray> model) {
            createMeshLets(model->m_Model->m_Vertices, model->m_Model->m_Indices);
        }

        void createMeshLets(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices);

        uint32_t meshletssize() { return meshlets.size(); }
        uint32_t meshlet_verticessize() { return meshlet_vertices.size(); }
        uint32_t meshlet_trianglessize() { return meshlet_triangles.size(); }

    public:
        std::vector<meshopt_Meshlet> meshlets;
        std::vector<uint32_t> meshlet_vertices;
        std::vector<uint8_t> meshlet_triangles;
    private:
        const size_t max_vertices = 64;
        const size_t max_triangles = 126;
        const float cone_weight = 0.0f;

    };


}