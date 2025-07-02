#include "gfpch.h"
#include "OpenGLShader.h"


namespace Graffiti {

	OpenGLShader::OpenGLShader(const std::string& shadername)
		:m_Name(shadername)
	{
		m_RenderID = glCreateProgram();
		UploadSceneData();
	}
	OpenGLShader::OpenGLShader(const std::string& shadername, std::initializer_list<std::pair<ShaderType, const std::string>> list)
		:m_Name(shadername)
	{
		m_RenderID = glCreateProgram();
		UploadSceneData();

		for (auto& [Type, Path] : list) {
			Load(Type,Path);
		}
	}
	
	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_RenderID);
	}
	void OpenGLShader::Bind() const
	{
		glUseProgram(m_RenderID);
	}
	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}

	void OpenGLShader::Load(ShaderType shadertype, const std::string& filepath)
	{
		GLuint GL_Shader;
		switch (shadertype)
		{
		case VertexShader:
		{
			GL_Shader = glCreateShader(GL_VERTEX_SHADER);
			break;
		}
		case FragmentShader:
		{
			GL_Shader = glCreateShader(GL_FRAGMENT_SHADER);
			break;
		}
		case GeometryShader:
		{
			GL_Shader = glCreateShader(GL_GEOMETRY_SHADER);
			break;
		}
		case TaskShader:
			break;
		case MeshShader:
			break;
		default:
			break;
		}
		std::ifstream ShaderFile;
		ShaderFile.open("Shaders/OpenGL/"+filepath);
		
		std::stringstream ShaderStream;
		ShaderStream << ShaderFile.rdbuf();
		ShaderFile.close();
		std::string shaderCodestring = ShaderStream.str();
		const GLchar* source = shaderCodestring.c_str();
		
		glShaderSource(GL_Shader, 1, &source, 0);

		// Compile the vertex shader
		glCompileShader(GL_Shader);

		GLint isCompiled = 0;
		glGetShaderiv(GL_Shader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(GL_Shader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(GL_Shader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(GL_Shader);

			// Use the infoLog as you see fit.
			GF_CORE_ERROR("Shader Compliation Failed!");
			GF_CORE_ERROR("0", infoLog.data());
			// In this simple program, we'll just leave
			return;
		}
		GLuint program = m_RenderID;
		// Attach our shaders to our program
		glAttachShader(program, GL_Shader);
		m_OpenGLSourceCode.insert({ GL_Shader, shadertype });
	}

	void OpenGLShader::Link()
	{
		GLuint program = m_RenderID;
		// Link our program
		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);
			// Don't leak shaders either.
			for (const auto& [shaderID, type] : m_OpenGLSourceCode) {
				glDeleteShader(shaderID);
			}

			// Use the infoLog as you see fit.
			GF_CORE_ERROR("Shader Linked Failed!");
			GF_CORE_ERROR("0", infoLog.data());
			// In this simple program, we'll just leave
			return;
		}
		for (const auto& [shaderID, type] : m_OpenGLSourceCode) {
			glDetachShader(program, shaderID);
		}
		m_OpenGLSourceCode.clear();
	}

	void OpenGLShader::UploadSceneData()
	{
        UploadUniformBuffer("m_SceneData", sizeof(SceneData), 1, 0, 0);
	}

	void OpenGLShader::SetSceneData(const SceneData& scenedata)
	{
        SetUniformBuffer("m_SceneData", &scenedata);
	}

	void OpenGLShader::UploadTransform()
	{
	}

	void OpenGLShader::SetTransform(const glm::mat4& transform)
	{
		GLint location = glGetUniformLocation(m_RenderID, "u_transform");
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(transform));
	}

	void OpenGLShader::SetUniformBuffer(const std::string& name, const void* value)
	{
		m_Data[name]->SetData(value, m_Data[name]->m_Set, 0);
	}

	void OpenGLShader::UploadUniformBuffer(const std::string& name, uint32_t size, uint32_t count, uint32_t set, uint32_t binding)
	{
		//OpenGL没有set这个概念，统一设置为size的大小
		m_Data[name] = UniformBuffer::Create(size, count, 0, m_Binding);
        m_Binding++;
	}

	void OpenGLShader::UploadTexture(const std::string& name, std::shared_ptr<Texture> texture, uint32_t set, uint32_t binding)
	{
		texture->Bind(binding);
	}

    void OpenGLShader::SetTexture(std::shared_ptr<Texture> texture, uint32_t set, uint32_t binding)
    {
        texture->Bind(binding);
    }

}