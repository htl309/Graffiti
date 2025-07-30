#include "MeshShaderModel.h"

namespace Graffiti {
	void MeshShaderModel::createMeshLets(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices)
    {
        size_t max_meshlets = meshopt_buildMeshletsBound(indices.size(), max_vertices, max_triangles);
        meshlets.resize(max_meshlets);
        meshlet_vertices.resize(max_meshlets * max_vertices);
        meshlet_triangles.resize(max_meshlets * max_triangles * 3);

        size_t meshlet_count = meshopt_buildMeshlets(meshlets.data(), meshlet_vertices.data(), meshlet_triangles.data(), indices.data(),
            indices.size(), &vertices[0].position.x, vertices.size(), sizeof(Vertex), max_vertices, max_triangles, cone_weight);

        meshopt_Meshlet& rLastMeshlet = meshlets[meshlet_count - 1];

        meshlet_vertices.resize(rLastMeshlet.vertex_offset + size_t(rLastMeshlet.vertex_count));
        meshlet_triangles.resize(rLastMeshlet.triangle_offset + ((size_t(rLastMeshlet.triangle_count) * 3 + 3) & ~3));
        meshlets.resize(meshlet_count);

        //for (auto i : meshlets) {
        //     std::cout << "triangle_count: " << i.triangle_count << " , ";
        //     std::cout << "vertex_count: " << i.vertex_count << " , ";
        //     std::cout << "triangle_offset: " << i.triangle_offset << " ,  ";
        //     std::cout << "vertex_offset: " << i.vertex_offset << std::endl;
        // }

        // for (int i = 0; i < meshlet_vertices.size(); i++) {
        //     if (i % 64 == 0) std::cout << std::endl << std::endl;
        //     std::cout << int(meshlet_vertices[i]) << " , ";
        // }
        // std::cout << "vertexsize: " << meshlet_vertices.size();
        // for (int i = 0; i < meshlet_triangles.size(); i++) {
        //     if (i % 288 == 0) std::cout << std::endl << std::endl;
        //     std::cout << int(meshlet_triangles[i]) << " , ";
        // }
         //std::cout << "meshlet_vertices: " << meshlet_vertices.size() << std::endl;
         //std::cout << "meshlet_triangles: " << meshlet_triangles.size()<<std::endl;
         // std::cout << "meshlet_count: " << meshlet_count << std::endl;
    }
}