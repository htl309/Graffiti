#pragma once
#include"gfpch.h"
#include"Graffiti/base/core.h"
#include<tiny_gltf.h>
namespace Graffiti {
	class  Texture {
		public:
			virtual uint32_t GetWidth() const = 0;
			virtual uint32_t GetHeight() const = 0;

            virtual uint32_t GetTextureID() const = 0;

			virtual void Bind(uint32_t slot = 0) const = 0;

            static std::shared_ptr<Texture> Create(const std::string& path);
            static std::shared_ptr<Texture> Create(const tinygltf::Image& image);

			std::string m_Name;
	};

}