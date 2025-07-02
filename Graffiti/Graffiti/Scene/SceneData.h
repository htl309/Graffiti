#pragma once

#include"Graffiti/base/core.h"


namespace Graffiti {
    struct PointLight {
        glm::vec4 position = { 10,10,10,1 };

        glm::vec4 ambient = { 0.1f, 0.1f, 0.1f, 1.0f };
        glm::vec4 diffuse = { 0.8f, 0.8f, 0.8f, 1.0f };
        glm::vec4 specular = { 1.0f, 1.0f, 1.0f, 1.0f };

    };
    // 平行光定义
    struct DirectionalLight {
        glm::vec3 direction;
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
    };
	struct SceneData
	{
		glm::mat4 ViewProjectionMatrix;
        PointLight Light;
        glm::vec3 CameraPosition;
	};
}