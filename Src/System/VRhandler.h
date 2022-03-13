#pragma once
#include <glad/glad.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <glm/glm.hpp>
#include <openvr.h>
#include <vector>
#include <iostream>
#include "Maths/Math.h"
#include "Geometry/ModelManager.h"
#include "Geometry/Model.h"

namespace TDVR {
	namespace VR {

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
			const char* m_ModelName;
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

			//Public Member Variables

			//Public Member Functions
			void HandleInput();
			void RenderFrame();

			

			inline FrameBufferDesc GetEyeDesc(vr::Hmd_Eye nEye) 
			{
				switch (nEye) 
				{
				case vr::Eye_Left:
					return m_LeftEyeDesc;
					break;
				case vr::Eye_Right:
					return m_RightEyeDesc;
					break;
				}
			}

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

			~VRhandler()
			{
				if (m_HMD) {
					vr::VR_Shutdown();
					m_HMD = NULL;
				}

				if (m_Context)
				{

					if (m_ControllerShader)
					{
						glDeleteProgram(m_ControllerShader->m_ShaderID);
					}
					if (m_ModelShader)
					{
						glDeleteProgram(m_ModelShader->m_ShaderID);
					}
					if (m_WindowShader)
					{
						glDeleteProgram(m_WindowShader->m_ShaderID);
					}

					glDeleteRenderbuffers(1, &m_LeftEyeDesc.m_DepthBufferID);
					glDeleteTextures(1, &m_LeftEyeDesc.m_RenderTextureID);
					glDeleteFramebuffers(1, &m_LeftEyeDesc.m_RenderFrameBufferID);
					glDeleteTextures(1, &m_LeftEyeDesc.m_ResolveTextureID);
					glDeleteFramebuffers(1, &m_LeftEyeDesc.m_ResolveFrameBufferID);

					glDeleteRenderbuffers(1, &m_RightEyeDesc.m_DepthBufferID);
					glDeleteTextures(1, &m_RightEyeDesc.m_RenderTextureID);
					glDeleteFramebuffers(1, &m_RightEyeDesc.m_RenderFrameBufferID);
					glDeleteTextures(1, &m_RightEyeDesc.m_ResolveTextureID);
					glDeleteFramebuffers(1, &m_RightEyeDesc.m_ResolveFrameBufferID);

					if (m_CWindowVAO != 0)
					{
						glDeleteVertexArrays(1, &m_CWindowVAO);
					}
					if (m_ControllerVAO != 0)
					{
						glDeleteVertexArrays(1, &m_ControllerVAO);
					}
				}

				if (m_Window)
				{
					SDL_DestroyWindow(m_Window);
					m_Window = NULL;
				}

				SDL_Quit();

			}
		private:

			//Private member Variables
			SDL_Window* m_Window;
			SDL_GLContext m_Context;

			GPCS::Shader* m_ModelShader, *m_ControllerShader, *m_WindowShader;

			vr::IVRSystem* m_HMD;

			FrameBufferDesc m_LeftEyeDesc;
			FrameBufferDesc m_RightEyeDesc;

			VRControllerInfo m_Controllers[2];

			uint32_t m_RenderWidth, m_RenderHeight;

			vr::TrackedDevicePose_t m_DevicePose[vr::k_unMaxTrackedDeviceCount];
			glm::mat4 m_DeviceMatrix[vr::k_unMaxTrackedDeviceCount];

			vr::VRActionHandle_t m_ActionHideController = vr::k_ulInvalidActionHandle;
			vr::VRActionHandle_t m_ActionTriggerHaptic = vr::k_ulInvalidActionHandle;
			vr::VRActionHandle_t m_ActionAnalongInput = vr::k_ulInvalidActionHandle;

			vr::VRActionSetHandle_t m_actionsetDemo = vr::k_ulInvalidActionSetHandle;

			//OpenGl Specific Variables
			glm::mat4 m_HMDMat, m_LeftEyeMat, m_RightEyeMat, m_ProjectionLeft, m_ProjectionRight, m_ProjectionCentre;

			GLuint m_CWindowVAO, m_CWindowVBO, m_CWindowIBO;
			unsigned int m_CWindowVertexCount;
			GLuint m_ControllerVAO, m_ControllerVBO, m_ControllerVertexCount;
			int m_ControllerCount, m_ControllerCountLast, m_ValidPoseCount, m_ValidPoseCountLast;

			glm::vec2 m_AnalogValue;

			const char* m_PoseClasses;
			char m_DeviceClass[vr::k_unMaxTrackedDeviceCount];
			//privtae Member Functions
			bool Init();
			void InitCompanionWindow();
			void RenderAxis();
			void RenderScene(vr::Hmd_Eye eye);
			void RenderStereoTargets();
			void UpdateHMDPose();
			void RenderWindow();
			void ProcessVREvent(vr::VREvent_t event);
			std::string GetDeviceString(vr::TrackedDeviceIndex_t unDevice, vr::TrackedDeviceProperty prop, vr::TrackedPropertyError* peError = NULL);

			glm::mat4 GetHMDProjectionMatrix(vr::Hmd_Eye eye);
			glm::mat4 GetHMDEyePos(vr::Hmd_Eye eye);
			glm::mat4 GetViewProjectionMatrix(vr::Hmd_Eye eye);
			bool GetDigitalActionRisingEdge(vr::VRActionHandle_t action, vr::VRInputValueHandle_t* pDevicePath = nullptr);
			bool GetDigitalActionState(vr::VRActionHandle_t action, vr::VRInputValueHandle_t* pDevicePath = nullptr);
			bool SetupRenderStereoTargets();

			bool CreateFrameBuffer(int renderWidth, int renderHeight, FrameBufferDesc& bufferDesc);

			//Essentials
			


		};
	}
}
