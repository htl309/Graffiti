#pragma once
#include"VertexArray.h"

#include<glm/glm.hpp>

namespace Graffiti {

	
	class RenderAPI {
	public:
        enum class API {
            OpenGL = 0,
            Vulkan = 1
        };

        static uint32_t ContextIndex;
        virtual ~RenderAPI() = default;
    public:
        virtual void Init(uint32_t  contextindex = 0) = 0;
		inline static API GetRenderAPI() { return s_API; }

        virtual void SetDepthtest(bool set) =0;
        virtual void SetMeshShader(bool set) {};
        virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) =0;
        virtual void SetClearColor(const glm::vec4& color) = 0;
        virtual void Clear() = 0;
        virtual void DrawIndex(const std::shared_ptr<VertexArray>& vertexarray) = 0;
        virtual void MeshShaderDraw(uint32_t taskcount) = 0;
        //�߿�ģʽ���Ƕ����ģʽ��Ĭ���Ƕ����ģʽ
        virtual void WireframeMode() = 0;
        virtual void PolygonMode() = 0;

       
	private:
		static API s_API;
        
	};
}