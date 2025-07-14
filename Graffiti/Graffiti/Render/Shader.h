#pragma once
#include<glm/glm.hpp>
#include<string>
#include"Graffiti/Scene/SceneData.h"
#include"Buffer.h"
#include"Graffiti/base/Texture.h"
namespace Graffiti {
    struct PipelineConfigInfo;

	enum  ShaderType {
		VertexShader = 0,
		FragmentShader,
		GeometryShader,
		TaskShader,
		MeshShader
	};
	class Shader
	{
	public:
		static std::shared_ptr <Shader> Create(const std::string& shadername);
		static std::shared_ptr <Shader> Create(const std::string& shadername, std::initializer_list<std::pair<ShaderType, const std::string>> list);
	public:
		
		virtual ~Shader() {};

        virtual void Bind() const = 0;
		virtual void Unbind() const =0;

		virtual void Load(ShaderType shadertype, const std::string& filepath) = 0;
        virtual void AddPipeline(std::shared_ptr<PipelineConfigInfo> configInfo) {};
		virtual void ClearPipeline() {};
		virtual void Link() = 0;

		
		//渲染循环时调用，用于更新每帧的新数据，
		//有一些静态的数据(比如MeshShader的数据)就不需要每帧调用,当然渲染循环之前需要调用一下
		virtual void SetUniformBuffer(const std::string& name, const void* value) = 0;
        //在创建管线之前就要调用这个函数
        virtual void UploadUniformBuffer(const std::string& name, uint32_t size, uint32_t count, uint32_t set, uint32_t binding) = 0;
       
		virtual void SetStorageBuffer(const std::string& name, const void* value) = 0;
		virtual void UploadStorageBuffer(const std::string& name, uint32_t size, uint32_t count, uint32_t set, uint32_t binding) = 0;
	
		virtual void UploadSceneData() = 0;
		virtual void SetSceneData(const SceneData& scenedata) = 0;

		virtual void UploadTransform() = 0;
		virtual void SetTransform(const glm::mat4& transform) = 0;
		
		//这里的两个texture不一定是同一个
		//创建管线时候的纹理不等于上传的纹理
		//创建时只是告诉管线，这里会有一个纹理
		virtual void UploadTexture(const std::string& name, std::shared_ptr<Texture> texture,uint32_t set, uint32_t binding) = 0;
        virtual void SetTexture(std::shared_ptr<Texture> texture, uint32_t set, uint32_t binding, const std::string modelname = {}) = 0;
	};

	class ShaderLibrary {
	public:
		void Add(const std::string& name, const std::shared_ptr <Shader> shader);
		void Load(const std::string& name, ShaderType& shadertype, const std::string& filepath);
		std::shared_ptr<Shader> Get(const std::string& name);

		bool Exists(const std::string& name) const;
	private:
		std::unordered_map<std::string, std::shared_ptr<Shader>> m_Shaders;
		
	};

}


