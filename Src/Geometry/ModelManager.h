#pragma once

#include <iostream>
#include <vector>
#include "stb_image.h"
#include "Maths/Math.h"
#include <openvr.h>
#include <map>

namespace TDVR 
{
	namespace GPCS { class Shader; }
	namespace MDL { class Model; }

}


namespace TDVR 
{ 
	namespace MDL 
	{
		using GPCS::Shader;
		using MDL::Model;


		class ModelManager 
		{
		public:
			//Public Member Variables

			//Public Member Functions
			static inline ModelManager* Instance() { return m_Instance == 0 ? m_Instance = new ModelManager() : m_Instance; }
			inline std::vector<Model*>& GetModelVector() { return m_Models; }
			void LoadModel(const char* ModelName);
			void AddCube();
			void Draw(Shader* shader, glm::mat4& mat);

			void VertexSelect(glm::vec4& controllerpos);
			void MoveVertex(const glm::vec4& ControllerPos);

			void Clear() 
			{
				for (int i = 0; i < m_Models.size(); i++)
					delete m_Models[i];
			}
		private:
			//Private Member Variables
			std::vector<Model*> m_Models;
			std::vector<Math::Vertex*> m_Selectedvertices;
			//Private Member Functions
			
			void ProcessNode(aiNode* node, const aiScene* scene, const char* name);
			Model* ProcessMesh(aiMesh* mesh, const aiScene* scene, glm::mat4 matrix, const char* name);
			

			//Essentials
			ModelManager() {}
			virtual ~ModelManager() {}

			static ModelManager* m_Instance;
		};
	}
}
