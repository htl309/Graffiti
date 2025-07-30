#pragma once

#include"Graffiti/base/core.h"


namespace Graffiti {
    struct PointLight {
        glm::vec4 position = glm::vec4(3.0f, 5.0f, 3.0f, 1.0f); // 点光源位置（右上前）

        glm::vec4 ambient = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f); // 更明显的环境光
        glm::vec4 diffuse = glm::vec4(1.8f, 1.65f, 1.5f, 1.0f); // 带点温暖色调
        glm::vec4 specular = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f); // 高光较亮，适合金属/玻璃


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