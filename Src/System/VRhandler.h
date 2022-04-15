#pragma once
#ifndef __VRhandler__
#define __VRhandler__


#include <glad/glad.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <glm/glm.hpp>
#include <openvr.h>
#include <vector>
#include <iostream>


namespace TDVR {
	namespace INPT { class InputHandler; }
	namespace MDL { class Model; class ModelManager; }
	namespace GPCS { class Shader; }
}

namespace TDVR {
	namespace VR {

		using INPT::InputHandler;
		using MDL::Model;
		using MDL::ModelManager;
		using GPCS::Shader;

		enum EHand 
		{
			left =0,
			right =1,
		};

		struct VRControllerInfo
		{
			vr::VRInputValueHandle_t m_Source = vr::k_ulInvalidActionHandle;
			vr::VRActionHandle_t m_ActionPose = vr::k_ulInvalidActionHandle;
			vr::VRActionHandle_t m_ActionHaptic = vr::k_ulInvalidActionHandle;
			glm::mat4 m_ModelMatrix;
			MDL::Model* m_Model = nullptr;
			std::string m_ModelName;
			glm::vec4 m_SelectionPos;
			bool m_ShowController;
		
		};

		struct FrameBufferDesc 
		{
			GLuint m_DepthBufferID;
			GLuint m_RenderTextureID;
			GLuint m_RenderFrameBufferID;
			GLuint m_ResolveTextureID;
			GLuint m_ResolveFrameBufferID;
		};

		struct ScreenVertexData 
		{
			glm::vec2 m_Pos;
			glm::vec2 m_UVs;
		};

		class VRhandler
		{
		public:

			//Public Member Functions
			void HandleInput();
			void RenderFrame();

			//Essentials
			VRhandler() 
				: m_Window(NULL)
				, m_Context(NULL)
				, m_HMD(NULL)
				, m_ControllerVBO(0)
				, m_ControllerVAO(0)
				, m_ControllerCount(0)
				, m_ControllerCountLast(-1)
				, m_ValidPoseCount(0)
				, m_ValidPoseCountLast(-1)
				, m_PoseClasses("") { Init(); memset(m_DeviceClass, 0, sizeof(m_DeviceClass));
			}

			~VRhandler();
		private:
			//Device Specific Varibles
			vr::IVRSystem* m_HMD;
			VRControllerInfo m_Controllers[2];

			std::string m_PoseClasses;

			char m_DeviceClass[vr::k_unMaxTrackedDeviceCount];


			//Render Important Variables
			SDL_Window* m_Window;
			SDL_GLContext m_Context;

			GPCS::Shader* m_ModelShader;
			GPCS::Shader* m_ControllerShader;
			GPCS::Shader* m_WindowShader;
			GPCS::Shader* m_RenderModelShader;
			GPCS::Shader* m_LineShader;
			GPCS::Shader* m_PointShader;
			
			FrameBufferDesc m_LeftEyeDesc;
			FrameBufferDesc m_RightEyeDesc;

			std::vector<MDL::Model*> m_ControllerModels;

			uint32_t m_RenderWidth;
			uint32_t m_RenderHeight;

			GLuint m_CWindowVAO;
			GLuint m_CWindowVBO;
			GLuint m_CWindowIBO;

			unsigned int m_CWindowVertexCount;

			GLuint m_ControllerVAO;
			GLuint m_ControllerVBO;
			GLuint m_ControllerVertexCount;

			int m_ControllerCount;
			int m_ControllerCountLast;
			int m_ValidPoseCount;
			int m_ValidPoseCountLast;

			//Device Position Info Varibales
			vr::TrackedDevicePose_t m_DevicePose[vr::k_unMaxTrackedDeviceCount];
			glm::mat4 m_DeviceMatrix[vr::k_unMaxTrackedDeviceCount];

			glm::mat4 m_HMDMat;
			glm::mat4 m_LeftEyeMat;
			glm::mat4 m_RightEyeMat;
			glm::mat4 m_ProjectionLeft;
			glm::mat4 m_ProjectionRight;
			glm::mat4 m_ProjectionCentre;


			//Device Action Variables
			InputHandler* m_InputHandler;

			
			//Initialisation functions
			bool Init();
			void InitCompanionWindow();
			bool SetupRenderStereoTargets();
			bool CreateFrameBuffer(int renderWidth, int renderHeight, FrameBufferDesc& bufferDesc);
			void SetProjectionMatrix();

			//HMD and Window Render Functions
			void RenderAxis();
			void RenderScene(vr::Hmd_Eye eye);
			void RenderStereoTargets();
			void RenderWindow();
			
			

			//Device Matrix Retrieval/Calculation Functions
			void UpdateHMDPose();
			glm::mat4 GetHMDProjectionMatrix(vr::Hmd_Eye eye);
			glm::mat4 GetHMDEyePos(vr::Hmd_Eye eye);
			glm::mat4 GetViewProjectionMatrix(vr::Hmd_Eye eye);


			//Device Event Functions
			
			std::string GetDeviceString(vr::TrackedDeviceIndex_t unDevice, vr::TrackedDeviceProperty prop, vr::TrackedPropertyError* peError = NULL);


			//Controller Model Loading Functions
			MDL::Model* LoadVRModel(const char* ModelName);
			unsigned int LoadVRTexture(vr::RenderModel_TextureMap_t& pTexture);

		};
	}
}
#endif //!__VRhandler__
