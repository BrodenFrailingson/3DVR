#pragma once
#include "Shader.h"

namespace TDVR 
{
	namespace GPCS 
	{
		Shader::Shader(const char* ShaderName) 
		{
			bool gshader = true;
			std::string Vshader = SHADER_DIR + (std::string)"/" + (std::string)ShaderName + "VS.txt";
			std::string FShader = SHADER_DIR + (std::string)"/" + (std::string)ShaderName + "FS.txt";
			std::string GShader = SHADER_DIR + (std::string)"/" + (std::string)ShaderName + "GS.txt";

			std::string VSCode, FSCode, GSCode;
			std::ifstream VSFile, FSFile, GSFile;
			

			VSFile.exceptions(std::istream::failbit | std::istream::badbit);
			FSFile.exceptions(std::istream::failbit | std::istream::badbit);
			GSFile.exceptions(std::istream::failbit | std::istream::badbit);
			try 
			{
				VSFile.open(Vshader.c_str());
				FSFile.open(FShader.c_str());
				try 
				{
					GSFile.open(GShader.c_str());
				}
				catch (std::istream::failure& e) 
				{
					std::cout << "No Geometry Shader found for: " << (std::string)ShaderName << '\n';
					gshader = false;
				}

				std::stringstream VSStream, FSStream, GSStream;

				VSStream << VSFile.rdbuf();
				FSStream << FSFile.rdbuf();

				VSFile.close();
				FSFile.close();

				VSCode = VSStream.str();
				FSCode = FSStream.str();

				if (gshader) 
				{
					GSStream << GSFile.rdbuf();

					GSFile.close();

					GSCode = GSStream.str();
				}
			}
			catch (std::istream::failure& e) 
			{
				std::cerr << "ERROR: A Vital Shader Has refused to Load. " << e.what() << '\n';
			}
			const char* GSCharCode;
			const char* VSCharCode = VSCode.c_str();
			const char* FSCharCode = FSCode.c_str();
			if (gshader)
				GSCharCode = GSCode.c_str();

			unsigned int v, f, g;

			v = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(v, 1, &VSCharCode, NULL);
			glCompileShader(v);
			CheckCompileErrors(v, "VERTEX");

			f = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(f, 1, &FSCharCode, NULL);
			glCompileShader(f);
			CheckCompileErrors(f, "FRAGMENT");

			if (gshader) 
			{
				g = glCreateShader(GL_GEOMETRY_SHADER);
				glShaderSource(g, 1, &GSCharCode, NULL);
				glCompileShader(g);
				CheckCompileErrors(g, "GEOMETRY");
			}

			m_ShaderID = glCreateProgram();
			glAttachShader(m_ShaderID, v);
			glAttachShader(m_ShaderID, f);
			if (gshader)
				glAttachShader(m_ShaderID, g);
			glLinkProgram(m_ShaderID);
			CheckCompileErrors(m_ShaderID, "PROGRAM");
			// delete the shaders as they're linked into our program now and no longer necessery
			glDeleteShader(v);
			glDeleteShader(f);
			if (gshader)
				glDeleteShader(g);
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