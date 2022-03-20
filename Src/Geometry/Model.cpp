#pragma once
#include "Model.h"

namespace TDVR 
{
	namespace MDL 
	{
		void Model::SetUpModel() 
		{
			glGenVertexArrays(1, &m_VAO);
			glGenBuffers(1, &m_VBO);
			glGenBuffers(1, &m_IBO);

			glBindVertexArray(m_VAO);
			// load data into vertex buffers
			glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
			// A great thing about structs is that their memory layout is sequential for all its items.
			// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
			// again translates to 3/2 floats which translates to a byte array.
			glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(Math::Vertex), &m_Vertices[0], GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(unsigned int), &m_Indices[0], GL_STATIC_DRAW);

			// set the vertex attribute pointers
			// vertex Positions
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Math::Vertex), (void*)0);
			// vertex normals
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Math::Vertex), (void*)offsetof(Math::Vertex, m_Normal));
			// vertex texture coords
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Math::Vertex), (void*)offsetof(Math::Vertex, m_UVs));
			// vertex tangent
			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Math::Vertex), (void*)offsetof(Math::Vertex, m_Tangent));
			// vertex bitangent
			glEnableVertexAttribArray(4);
			glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Math::Vertex), (void*)offsetof(Math::Vertex, m_BiTangent));
			// Vertex Color
			glEnableVertexAttribArray(5);
			glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE,  sizeof(Math::Vertex), (void*)offsetof(Math::Vertex, m_VertexColor));

			glBindVertexArray(0);
		}

		void Model::Draw()
		{
			glBindVertexArray(m_VAO);
			if(m_TextureID != NULL)
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, m_TextureID);
			}
			glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}

		glm::mat4 Model::GetTranslationMatrix() 
		{
			glm::mat4 mat = glm::translate(m_Transform.m_ModelMatrix, m_Transform.Pos);
			mat = glm::scale(mat, m_Transform.Scale);
			mat = glm::rotate(mat, m_Transform.Rot.x, glm::vec3(1.0f, 0.0f, 0.0f));
			mat = glm::rotate(mat, m_Transform.Rot.y, glm::vec3(0.0f, 1.0f, 0.0f));
			mat = glm::rotate(mat, m_Transform.Rot.z, glm::vec3(0.0f, 0.0f, 1.0f));
			return mat;
		}

	}
}