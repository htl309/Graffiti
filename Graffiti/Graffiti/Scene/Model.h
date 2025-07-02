#pragma once

#include"Graffiti/base/core.h"
#include "Vertex.h"
#include"Graffiti/base/Texture.h"
namespace Graffiti {

    struct BoundingBox {
            glm::vec3 minPos{ FLT_MAX }; 
            glm::vec3 maxPos{ -FLT_MAX };
    };
    class Model {
    public:
        std::vector<Vertex>  m_Vertices;
        std::vector<uint32_t>  m_Indices;
        BoundingBox boundingbox;
        static  void Model::LoadGLTFModel(std::string filepath, std::shared_ptr<Model>& model);
        static  void Model::LoadGLTFModel(std::string filepath, std::shared_ptr<Model>& model, std::shared_ptr<Texture>& texture );
   };
}