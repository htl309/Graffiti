#pragma once

#include"Graffiti/base/core.h"

namespace Graffiti {

    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal = { 0,0,1 };
        glm::vec2 texCoord;
    };
}