#pragma once
#include <assimp/Importer.hpp>
#include <glad/glad.h>
#include <vector>
#include "Maths/Math.h"
#include "Graphics/Shader.h"

namespace TDVR 
{ 
	namespace MDL 
    {
        struct Texture
        {
            GLuint id;
            const char* type, *path;
        };

        struct Transform 
        {
            glm::mat4 m_ModelMatrix = glm::mat4{ 1.0f };
            glm::vec3 Pos, Scale, Rot;
        };

        class Model 
        {
        public:
            //Public Member Variables
            std::vector<Math::Vertex> m_Vertices;
            std::vector<unsigned int> m_Indices;
            std::vector<aiFace> m_Faces;
            const char* m_FilePath;
            //Public Member Functions
            inline Transform GetTransform() { return m_Transform; }

            inline void SetScale(glm::vec3 scale) { m_Transform.Scale = scale; }
            inline void SetScale(int x, int y, int z) { m_Transform.Scale = glm::vec3{ x,y,z }; }
            
            inline void Scale(int xscale, int yscale, int zscale) { m_Transform.Scale.x += xscale; m_Transform.Scale.y += yscale; m_Transform.Scale.z += zscale; }
            
            inline void SetModelMatrix(glm::mat4& mat) { m_Transform.m_ModelMatrix = mat; }
            
            void Draw(GPCS::Shader* shader);
            //Essentials
            Model(const char* ModelName, std::vector<Math::Vertex> verts, std::vector<unsigned int> indices, std::vector<aiFace> faces, glm::mat4 modelmatrix = glm::mat4{1.0f})
            {
                this->m_Vertices = verts;
                this->m_Indices = indices;
                this->m_Faces = faces;
                this->m_FilePath = ModelName;
                this->m_Transform = Transform{ modelmatrix, glm::vec4{0.0f,0.0f,0.0f,1.0f}, glm::vec3{1.0f}, glm::vec3{0.0f} };
                SetUpModel();
            }

            virtual ~Model()
            {
                glDeleteBuffers(1, &m_IBO);
                glDeleteVertexArrays(1, &m_VAO);
                glDeleteBuffers(1, &m_VBO);
                m_IBO = m_VAO = m_VBO = 0;
            }

        private:
            //Private Member Variables
            GLuint m_VertexCount;
            Transform m_Transform;
            GLuint m_VAO, m_VBO, m_IBO;
            

            vr::RenderModel_t* VRModel;
            vr::RenderModel_TextureMap_t* VRTexture;

            //Private Member Functions
            void SetUpModel();
            
        };
    } 
}