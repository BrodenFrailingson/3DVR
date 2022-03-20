#pragma once
#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <SDL_opengl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <openvr.h>

namespace TDVR
{
	namespace Math 
	{
		struct Vertex 
		{
			glm::vec4 m_Pos;
			glm::vec3 m_Normal;
			glm::vec2 m_UVs;
			glm::vec3 m_Tangent{0.0f};
			glm::vec3 m_BiTangent{0.0f};
			glm::vec3 m_VertexColor{ 0.36f, 0.13f, 0.43f};
		};

		inline void AIMat4ToGLmMat4(const aiMatrix4x4* from, glm::mat4& to) 
		{
			to[0][0] = (GLfloat)from->a1; to[0][1] = (GLfloat)from->b1;  to[0][2] = (GLfloat)from->c1; to[0][3] = (GLfloat)from->d1;
			to[1][0] = (GLfloat)from->a2; to[1][1] = (GLfloat)from->b2;  to[1][2] = (GLfloat)from->c2; to[1][3] = (GLfloat)from->d2;
			to[2][0] = (GLfloat)from->a3; to[2][1] = (GLfloat)from->b3;  to[2][2] = (GLfloat)from->c3; to[2][3] = (GLfloat)from->d3;
			to[3][0] = (GLfloat)from->a4; to[3][1] = (GLfloat)from->b4;  to[3][2] = (GLfloat)from->c4; to[3][3] = (GLfloat)from->d4;
		}

		inline void GLMMat4ToAIMat4(const glm::mat4& from, aiMatrix4x4& to)
		{
			to.a1 = (float)from[0][0]; to.b1 = (float)from[0][1];  to.c1 = (float)from[0][2]; to.d1 = (float)from[0][3];
			to.a2 = (float)from[1][0]; to.b2 = (float)from[1][1];  to.c2 = (float)from[1][2]; to.d2 = (float)from[1][3];
			to.a3 = (float)from[2][0]; to.b3 = (float)from[2][1];  to.c3 = (float)from[2][2]; to.d3 = (float)from[2][3];
			to.a4 = (float)from[3][0]; to.b4 = (float)from[3][1];  to.c4 = (float)from[3][2]; to.d4 = (float)from[3][3];
		}

		inline glm::mat4 SteamMatrixToGlmMatrix(const vr::HmdMatrix34_t& vrmat) 
		{
			glm::mat4 outmat(vrmat.m[0][0], vrmat.m[1][0], vrmat.m[2][0], 0.0,
			                 vrmat.m[0][1], vrmat.m[1][1], vrmat.m[2][1], 0.0,
				             vrmat.m[0][2], vrmat.m[1][2], vrmat.m[2][2], 0.0,
				             vrmat.m[0][3], vrmat.m[1][3], vrmat.m[2][3], 1.0f);
			return outmat;
		}

	}
}