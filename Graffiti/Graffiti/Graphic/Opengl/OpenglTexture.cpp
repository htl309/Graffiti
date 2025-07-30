#include "gfpch.h"
#include "OpenGLTexture.h"

#include<stb_image.h>
namespace Graffiti {
	OpenGLTexture::OpenGLTexture(const std::string& path)
		:m_Path(path)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(true);
		auto data = stbi_load(m_Path.c_str(), &width, &height,&channels,0);
		GF_CORE_ASSERT(data, "Failed to load image!");

		
		GLenum internalFormat = channels == 4 ? GL_RGBA8: GL_RGB8;
		GLenum dataFormat     = channels == 4 ? GL_RGBA : GL_RGB;

		m_Width = width;
		m_Height = height;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureID);
		glTextureStorage2D(m_TextureID, 1, internalFormat, m_Width, m_Height);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTextureParameteri(m_TextureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_TextureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureSubImage2D(m_TextureID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);
	}
    OpenGLTexture::OpenGLTexture(const tinygltf::Image& image)
        :m_Width(image.width),m_Height(image.height)
    {
        glGenTextures(1, &m_TextureID);
        glBindTexture(GL_TEXTURE_2D, m_TextureID);

        // 图像数据格式
        GLenum format = GL_RGBA;
        switch (image.component) {
        case 1: format = GL_RED;  break;
        case 2: format = GL_RG;   break;
        case 3: format = GL_RGB;  break;
        case 4: format = GL_RGBA; break;
        default:
            printf("Unsupported image component count: %d\n", image.component);
            break;
        }

        // 数据类型
        GLenum type = GL_UNSIGNED_BYTE;
        if (image.bits == 16) {
            type = GL_UNSIGNED_SHORT;
        }

        // 上传纹理数据
        glTexImage2D(
            GL_TEXTURE_2D, 0,
            format,
            image.width,
            image.height,
            0,
            format,
            type,
            image.image.data()
        );

        // 设置采样参数
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // 生成 Mipmap
        glGenerateMipmap(GL_TEXTURE_2D);

        glBindTexture(GL_TEXTURE_2D, 0);
    }

	OpenGLTexture::~OpenGLTexture()
	{
		glDeleteTextures(1, &m_TextureID);
	}
	void OpenGLTexture::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_TextureID);

	}



   
}


