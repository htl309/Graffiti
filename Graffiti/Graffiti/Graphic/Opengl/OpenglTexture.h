#pragma once
#include"Graffiti/base/Texture.h"

namespace Graffiti {
	class OpenGLTexture : public Texture {

	public:
		OpenGLTexture(const std::string& path);
		OpenGLTexture(const tinygltf::Image& image);
		~OpenGLTexture();

		virtual uint32_t GetWidth()const override  { return m_Width; }
		virtual uint32_t GetHeight()const override { return m_Height; }

        virtual uint32_t GetTextureID() const override  { return m_TextureID; }

		virtual void Bind(uint32_t slot = 0) const override;
	private:
		uint32_t m_Width;
		uint32_t m_Height;
		std::string m_Path;
		uint32_t m_TextureID;
		
	};

}


