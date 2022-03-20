#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>
#include <fstream>
#include <typeinfo>
#include <sstream>
#include <iostream>

namespace TDVR
{
	namespace GPCS 
	{
		class Shader 
		{
		public:
			//Public Member Variables
			unsigned int m_ShaderID;

			//Public Member Functions
			void Use() { glUseProgram(this->m_ShaderID); }

			/*template<typename T>
			void SetShaderVariable(const char* VarName, const T& Val) const
			{
				if (typeid(Val) == typeid(glm::mat4))
				{
					glUniformMatrix4fv(glGetUniformLocation(m_ShaderID, VarName), 1, GL_FALSE, &Val[0][0]);
					return;
				}
				else if (typeid(Val) == typeid(glm::mat3))
				{
					glUniformMatrix3fv(glGetUniformLocation(m_ShaderID, VarName), 1, GL_FALSE, &Val[0][0]);
					return;
				}
				else if (typeid(Val) == typeid(glm::vec4))
				{
					glUniform4fv(glGetUniformLocation(m_ShaderID, VarName), 1, &Val[0]);
					return;
				}
				else if (typeid(Val) == typeid(glm::vec3))
				{
					glUniform3fv(glGetUniformLocation(m_ShaderID, VarName), 1, &Val[0]);
					return;
				}
				else if (typeid(Val) == typeid(glm::vec2))
				{
					glUniform2fv(glGetUniformLocation(m_ShaderID, VarName), 1, &Val[0]);
					return;
				}
				else if (typeid(Val) == typeid(bool))
				{
					glUniform1i(glGetUniformLocation(m_ShaderID, VarName), (int)Val);
					return;
				}
				else if (typeid(Val) == typeid(float))
				{
					glUniform1f(glGetUniformLocation(m_ShaderID, VarName), Val);
					return;
				}
				else if (typeid(Val) == typeid(int))
				{
					glUniform1i(glGetUniformLocation(m_ShaderID, VarName), Val);
					return;
				}
				else{
					std::cerr << "Variable of Type: " << (std::string)typeid(Val).name() << " Not Found." << '\n';
					return;
				}
			}*/

			void SetMat4(const char* varName, const glm::mat4& Var)const 
			{
				glUniformMatrix4fv(glGetUniformLocation(this->m_ShaderID, varName), 1, GL_FALSE, &Var[0][0]);
			}

			//Essentials
			Shader(const char* ShaderName);

		private:
			//Private Member Variables

			//Private Member Functions
			void CheckCompileErrors(GLuint shader, std::string type);

			//Essentials

		};
	}
}