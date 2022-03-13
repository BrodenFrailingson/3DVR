#pragma once
#include "Shader.h"

namespace TDVR 
{
	namespace GPCS 
	{
		Shader::Shader(const char* ShaderName) 
		{
			std::string Vshader = SHADER_DIR + (std::string)"/" + (std::string)ShaderName + "VS.txt";
			std::string FShader = SHADER_DIR + (std::string)"/" + (std::string)ShaderName + "FS.txt";

			std::string VSCode, FSCode;
			std::ifstream VSFile, FSFile;
			

			VSFile.exceptions(std::istream::failbit | std::istream::badbit);
			FSFile.exceptions(std::istream::failbit | std::istream::badbit);
			try 
			{
				VSFile.open(Vshader.c_str());
				FSFile.open(FShader.c_str());

				std::stringstream VSStream, FSStream;

				VSStream << VSFile.rdbuf();
				FSStream << FSFile.rdbuf();

				VSFile.close();
				FSFile.close();

				VSCode = VSStream.str();
				FSCode = FSStream.str();
			}
			catch (std::istream::failure& e) 
			{
				std::cerr << "ERROR: A Vital Shader Has refused to Load. " << e.what() << '\n';
			}
			const char* VSCharCode = VSCode.c_str();
			const char* FSCharCode = FSCode.c_str();

			unsigned int v, f;

			v = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(v, 1, &VSCharCode, NULL);
			glCompileShader(v);
			CheckCompileErrors(v, "VERTEX");

			f = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(f, 1, &FSCharCode, NULL);
			glCompileShader(f);
			CheckCompileErrors(f, "FRAGMENT");

			m_ShaderID = glCreateProgram();
			glAttachShader(m_ShaderID, v);
			glAttachShader(m_ShaderID, f);
			std::cout << m_ShaderID << '\n';
		}


		void Shader::CheckCompileErrors(GLuint shader, std::string type) 
		{
			GLint success;
			GLchar infoLog[1024];
			if (type != "PROGRAM")
			{
				glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
				if (!success)
				{
					glGetShaderInfoLog(shader, 1024, NULL, infoLog);
					std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << '\n';
				}
			}
			else
			{
				glGetProgramiv(shader, GL_LINK_STATUS, &success);
				if (!success)
				{
					glGetProgramInfoLog(shader, 1024, NULL, infoLog);
					std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << '\n';
				}
			}
		}
	}
}