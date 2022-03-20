#pragma once
#include "ModelManager.h"

namespace TDVR 
{
	namespace MDL 
	{
		ModelManager* ModelManager::m_Instance = 0;
		/*unsigned int ModelManager::LoadTexture(const char* path, const char* directory, bool gamma = false)
		{
			std::string filename = MODEL_DIR + (std::string)directory + '/' + (std::string)path;


			unsigned int textureID;
			glGenTextures(1, &textureID);

			int width, height, nrComponents;
			unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
			if (data)
			{
				GLenum format;
				if (nrComponents == 1)
					format = GL_RED;
				else if (nrComponents == 3)
					format = GL_RGB;
				else if (nrComponents == 4)
					format = GL_RGBA;

				glBindTexture(GL_TEXTURE_2D, textureID);
				glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
				glGenerateMipmap(GL_TEXTURE_2D);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

				stbi_image_free(data);
			}
			else
			{
				std::cout << "Texture failed to load at path: " << path << std::endl;
				stbi_image_free(data);
			}

			return textureID;
		}*/

		void ModelManager::AddCube() 
		{
			std::vector<Math::Vertex> vertices;
			std::vector<unsigned int> indices;
			std::vector<aiFace> faces;

			Math::Vertex v1{ glm::vec4{-0.2f, 0.2f, 0.2f, 1.0f},glm::vec3{-0.5f, 0.5f, 0.5f},glm::vec2{0.0f,1.0f} };
			Math::Vertex v2{ glm::vec4{ 0.2f, 0.2f, 0.2f, 1.0f},glm::vec3{ 0.5f, 0.5f, 0.5f},glm::vec2{1.0f,1.0f} };
			Math::Vertex v3{ glm::vec4{-0.2f,-0.2f, 0.2f, 1.0f},glm::vec3{-0.5f,-0.5f, 0.5f},glm::vec2{0.0f,0.0f} };
			Math::Vertex v4{ glm::vec4{ 0.2f,-0.2f, 0.2f, 1.0f},glm::vec3{ 0.5f,-0.5f, 0.5f},glm::vec2{1.0f,0.0f} };
			Math::Vertex v5{ glm::vec4{ 0.2f, 0.2f,-0.2f, 1.0f},glm::vec3{ 0.5f, 0.5f,-0.5f},glm::vec2{0.0f,1.0f} };
			Math::Vertex v6{ glm::vec4{-0.2f, 0.2f,-0.2f, 1.0f},glm::vec3{-0.5f, 0.5f,-0.5f},glm::vec2{1.0f,1.0f} };
			Math::Vertex v7{ glm::vec4{ 0.2f,-0.2f,-0.2f, 1.0f},glm::vec3{ 0.5f,-0.5f,-0.5f},glm::vec2{0.0f,0.0f} };
			Math::Vertex v8{ glm::vec4{-0.2f,-0.2f,-0.2f, 1.0f},glm::vec3{-0.5f,-0.5f,-0.5f},glm::vec2{1.0f,0.0f} };

			vertices.push_back(v1);
			vertices.push_back(v2);
			vertices.push_back(v3);
			vertices.push_back(v4);
			vertices.push_back(v5);
			vertices.push_back(v6);
			vertices.push_back(v7);
			vertices.push_back(v8);

			indices.push_back(0);	//Front Face
			indices.push_back(1);
			indices.push_back(2);
			indices.push_back(3);
			indices.push_back(2);
			indices.push_back(1);

			indices.push_back(5);	//Left Face
			indices.push_back(0);
			indices.push_back(7);
			indices.push_back(2);
			indices.push_back(7);
			indices.push_back(0);

			indices.push_back(4);	//Back Face
			indices.push_back(5);
			indices.push_back(6);
			indices.push_back(7);
			indices.push_back(6);
			indices.push_back(5);

			indices.push_back(1);	//Right Face
			indices.push_back(4);
			indices.push_back(3);
			indices.push_back(6);
			indices.push_back(3);
			indices.push_back(4);

			indices.push_back(5);	//Top Face
			indices.push_back(4);
			indices.push_back(0);
			indices.push_back(1);
			indices.push_back(0);
			indices.push_back(4);

			indices.push_back(2);	//Bottom Face
			indices.push_back(3);
			indices.push_back(7);
			indices.push_back(6);
			indices.push_back(7);
			indices.push_back(3);

			aiFace f1 = aiFace();
			aiFace f2 = aiFace();
			aiFace f3 = aiFace();
			aiFace f4 = aiFace();
			aiFace f5 = aiFace();
			aiFace f6 = aiFace();

			f1.mNumIndices = 4;
			f1.mIndices = new unsigned int[4]{0,1,2,3};
			faces.push_back(f1);

			f2.mNumIndices = 4;
			f2.mIndices = new unsigned int[4]{0,2,5,7};
			faces.push_back(f2);

			f3.mNumIndices = 4;
			f3.mIndices = new unsigned int[4]{ 4,5,6,7 };
			faces.push_back(f3);

			f4.mNumIndices = 4;
			f4.mIndices = new unsigned int[4]{ 1,3,4,6 };
			faces.push_back(f4);

			f5.mNumIndices = 4;
			f5.mIndices = new unsigned int[4]{ 0,1,4,5 };
			faces.push_back(f5);

			f6.mNumIndices = 4;
			f6.mIndices = new unsigned int[4]{ 2,3,6,7 };
			faces.push_back(f6);

			m_Models.push_back(new Model("Cube", vertices, indices, faces));
		}

		// loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
		void ModelManager::LoadModel(const char* Name)
		{
			std::string FilePath = MODEL_DIR + (std::string)"/" + (std::string)Name + "/" + (std::string)Name + ".obj";

			// read file via ASSIMP
			Assimp::Importer importer;
			const aiScene* scene = importer.ReadFile(FilePath.c_str(), aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices | aiProcess_FlipUVs);
			// check for errors
			if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
			{
				std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
				return;
			}

			// process ASSIMP's root node recursively
			ProcessNode(scene->mRootNode, scene, Name);
		}

		

		// processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
		void ModelManager::ProcessNode(aiNode* node, const aiScene* scene, const char* name)
		{
			glm::mat4 m_GLMmatrix;
			aiMatrix4x4 m_AImatrix;
			// process each mesh located at the current node
			for (unsigned int i = 0; i < node->mNumMeshes; i++)
			{
				//Store the node transform matrices for each mesh
				if (node->mParent != NULL) 
				{ //If the node is not the root, transform the locar reference system of the node into the reference system of its parent
					m_AImatrix = node->mParent->mTransformation * node->mTransformation;
					Math::AIMat4ToGLmMat4(&m_AImatrix, m_GLMmatrix);
				}// We are basically expressing each mesh in the same reference system of the root node
				else {
					m_AImatrix = node->mTransformation;
					Math::AIMat4ToGLmMat4(&m_AImatrix, m_GLMmatrix);
				}//If the node is the root leave it be

				// the node object only contains indices to index the actual objects in the scene. 
				// the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
				aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
				m_Models.push_back(ProcessMesh(mesh, scene, m_GLMmatrix, name));
			}
			// after we've processed all of the meshes (if any) we then recursively process each of the children nodes
			for (unsigned int i = 0; i < node->mNumChildren; i++)
			{
				ProcessNode(node->mChildren[i], scene, node->mName.C_Str());
			}
		}

		Model* ModelManager::ProcessMesh(aiMesh* mesh, const aiScene* scene, glm::mat4 matrix, const char* name)
		{
			// data to fill
			std::vector<Math::Vertex> vertices;
			std::vector<unsigned int> indices;
			std::vector<aiFace> faces;

			// Walk through each of the mesh's vertices
			for (unsigned int i = 0; i < mesh->mNumVertices; i++)
			{
				Math::Vertex vertex;
				glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
								  // positions
				vector.x = mesh->mVertices[i].x;
				vector.y = mesh->mVertices[i].y;
				vector.z = mesh->mVertices[i].z;
				vertex.m_Pos = glm::vec4{ vector, 1.0f };
				// normals
				if (mesh->HasNormals())
				{
					vector.x = mesh->mNormals[i].x;
					vector.y = mesh->mNormals[i].y;
					vector.z = mesh->mNormals[i].z;
					vertex.m_Normal = vector;
				}
				// texture coordinates
				if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
				{
					glm::vec2 vec;
					// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
					// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
					vec.x = mesh->mTextureCoords[0][i].x;
					vec.y = mesh->mTextureCoords[0][i].y;
					vertex.m_UVs = vec;
				}
				else
					vertex.m_UVs = glm::vec2(0.0f, 0.0f);

				//Tangents and Bitangents
				if (mesh->HasTangentsAndBitangents())
				{
					// tangent
					vector.x = mesh->mTangents[i].x;
					vector.y = mesh->mTangents[i].y;
					vector.z = mesh->mTangents[i].z;
					vertex.m_Tangent = vector;
					// bitangent
					vector.x = mesh->mBitangents[i].x;
					vector.y = mesh->mBitangents[i].y;
					vector.z = mesh->mBitangents[i].z;
					vertex.m_BiTangent = vector;
				}
				vertices.push_back(vertex);
			}
			// now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
			for (unsigned int i = 0; i < mesh->mNumFaces; i++)
			{
				aiFace face = mesh->mFaces[i];
				faces.push_back(mesh->mFaces[i]);
				// retrieve all indices of the face and store them in the indices vector
				for (unsigned int j = 0; j < face.mNumIndices; j++) {
					indices.push_back(face.mIndices[j]);
				}
			}
			// process materials
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			// we assume a convention for sampler names in the shaders. Each diffuse texture should be named
			// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
			// Same applies to other texture as the following list summarizes:
			// diffuse: texture_diffuseN
			// specular: texture_specularN
			// normal: texture_normalN

			// 1. diffuse maps
			//std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
			//textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
			//// 2. specular maps
			//std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
			//textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
			//// 3. normal maps
			//std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
			//textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
			//// 4. height maps
			//std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
			//textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

			// return a mesh object created from the extracted mesh data
			return new Model(name, vertices, indices, faces, matrix);
		}

		// checks all material textures of a given type and loads the textures if they're not loaded yet.
		// the required info is returned as a Texture struct.
		//std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
		//{
		//	std::vector<Texture> textures;
		//	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
		//	{
		//		aiString str;
		//		mat->GetTexture(type, i, &str);
		//		// check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
		//		bool skip = false;
		//		for (unsigned int j = 0; j < textures.size(); j++)
		//		{
		//			if (std::strcmp(textures[j].path, str.C_Str()) == 0)
		//			{
		//				textures.push_back(textures[j]);
		//				skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
		//				break;
		//			}
		//		}
		//		if (!skip)
		//		{   // if texture hasn't been loaded already, load it
		//			GLuint id = LoadTexture(str.C_Str(), this->directory);
		//			Texture texture;
		//			texture.id = 
		//			texture.type = typeName;
		//			texture.path = str.C_Str();
		//			textures.push_back(texture);
		//			textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
		//		}
		//	}
		//	return textures;
		//}
	};
}