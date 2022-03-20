#pragma once
#include <iostream>

#include "Model.h"
#include "stb_image.h"
#include <openvr.h>


namespace TDVR 
{ 
	namespace MDL 
	{
		class ModelManager 
		{
		public:
			//Public Member Variables

			//Public Member Functions
			static inline ModelManager* Instance() { return m_Instance == 0 ? m_Instance = new ModelManager() : m_Instance; }
			inline std::vector<Model*>& GetModelVector() { return m_Models; }
			void LoadModel(const char* ModelName);
			void AddCube();
			void Draw(GPCS::Shader* shader,glm::mat4& mat)
			{ 
				shader->Use();
				for (int i = 0; i < m_Models.size(); i++)
				{
					mat = mat * m_Models[i]->GetTranslationMatrix();
					shader->SetMat4("matrix", mat);
					m_Models[i]->Draw();
				}
			}

			void Clear() 
			{
				for (int i = 0; i < m_Models.size(); i++)
					delete m_Models[i];
			}
		private:
			//Private Member Variables
			std::vector<Model*> m_Models;
			//Private Member Functions
			
			void ProcessNode(aiNode* node, const aiScene* scene, const char* name);
			Model* ProcessMesh(aiMesh* mesh, const aiScene* scene, glm::mat4 matrix, const char* name);
			//std::vector<Texture> LoadMaterialTextures(aiMaterial* material, aiTextureType type, const char* TexType);
			//GLuint LoadTexture(const char* TextureName, const char* TextureDir, bool gamma = false);

			//Essentials
			ModelManager() {}
			virtual ~ModelManager() {}

			static ModelManager* m_Instance;
		};
	}
}
