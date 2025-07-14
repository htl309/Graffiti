#pragma once

#include<Graffiti.h>

namespace Graffiti {
    class TextureLayer : public Layer {
    public:

        TextureLayer();
        ~TextureLayer();

        void OnImGuiRender() override;
        void OnUpdate(TimeStep& ts) override;
        void OnEvent(Event& event) override;

    private:
        std::string m_Name;
        std::shared_ptr <ShaderLibrary> m_ShaderLibrary;

        SceneData m_Scene;

        std::shared_ptr <VertexArray> m_CubeVertexArray;
        std::shared_ptr <VertexArray> m_DeerVertexArray;


        std::shared_ptr<Texture> m_Texture_deer;
        std::shared_ptr<Texture> m_Texture;
        std::shared_ptr<Texture> m_Texture_Graffiti0;
        std::shared_ptr<Texture> m_Texture_Graffiti1;

        CameraControl m_CameraControl;


        bool IsWireFrame = 0;
        int model = 1;


        ImTextureID Texture_Graffiti0_ID;
        ImTextureID Texture_Graffiti1_ID;
        ImTextureID Texture_deer_ID ;
        ImTextureID Texture_ID;
    };


}