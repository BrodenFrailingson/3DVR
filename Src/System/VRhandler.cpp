#pragma once
#include "VRhandler.h"

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif //_WIN32

namespace TDVR 
{
	namespace VR 
	{
		bool VRhandler::Init() 
		{
			vr::EVRInitError error = vr::VRInitError_None;
			m_HMD = vr::VR_Init(&error, vr::VRApplication_Scene);

			if (error != vr::VRInitError_None) 
			{
				m_HMD = NULL;
				char buf[1024];
				sprintf_s(buf, sizeof(buf), "Unable to init VR runtime: %s", vr::VR_GetVRInitErrorAsEnglishDescription(error));
				SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "VR_Init Failed", buf, NULL);
				return false;
			}

			m_Window = SDL_CreateWindow("3DVR", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 800, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
			if (!m_Window)
			{
				std::cerr << "ERROR: SDL Window Creation Failed. Cause: " << SDL_GetError() << '\n';
				return false;
			}

			m_Context = SDL_GL_CreateContext(m_Window);

			if (!m_Context)
			{
				std::cerr << "ERROR: SDL Context Creation Failed. Cause: " << SDL_GetError() << '\n';
				return false;
			}

			SDL_GL_MakeCurrent(m_Window, m_Context);

			if (!gladLoadGL())
			{
				std::cerr << "ERROR: GLAD Init Failed. Cause: " << glGetError() << '\n';
				return false;
			}

			if (SDL_GL_SetSwapInterval(0) != 0)
			{
				std::cerr << "ERROR: SDL VSync Init Failed. Cause: " << SDL_GetError() << '\n';
				return false;
			}
			
			m_ModelShader = new GPCS::Shader("Model");
			m_ControllerShader = new GPCS::Shader("Controller");
			m_WindowShader = new GPCS::Shader("Window");
			m_RenderModelShader = new GPCS::Shader("RenderModel");
			m_PointShader = new GPCS::Shader("Point");

			m_ProjectionLeft = GetHMDProjectionMatrix(vr::Eye_Left);
			m_ProjectionRight = GetHMDProjectionMatrix(vr::Eye_Right);
			m_LeftEyeMat = GetHMDEyePos(vr::Eye_Left);
			m_RightEyeMat = GetHMDEyePos(vr::Eye_Right);

			SetupRenderStereoTargets();
			InitCompanionWindow();
			
			

			MDL::ModelManager::Instance()->AddCube();
			
			if (!vr::VRCompositor()) 
			{
				std::cout <<"Compositor initialization failed. See log file for details" << '\n';
				return false;
			}

			std::string strpath = SRC_DIR + (std::string)"/" + "Bin/" + "example_actions.json";
			const char* filepath = strpath.c_str();
			vr::VRInput()->SetActionManifestPath(filepath);

			vr::VRInput()->GetActionHandle("/actions/demo/in/HideThisController", &m_ActionHideController);
			vr::VRInput()->GetActionHandle("/actions/demo/in/TriggerHaptic", &m_ActionTriggerHaptic);
			vr::VRInput()->GetActionHandle("/actions/demo/in/AnalogInput", &m_ActionAnalongInput);

			vr::VRInput()->GetActionSetHandle("/actions/demo", &m_actionsetDemo);

			vr::VRInput()->GetActionHandle("/actions/demo/out/Haptic_Left", &m_Controllers[left].m_ActionHaptic);
			vr::VRInput()->GetInputSourceHandle("/user/hand/left", &m_Controllers[left].m_Source);
			vr::VRInput()->GetActionHandle("/actions/demo/in/Hand_Left", &m_Controllers[left].m_ActionPose);

			vr::VRInput()->GetActionHandle("/actions/demo/out/Haptic_Right", &m_Controllers[right].m_ActionHaptic);
			vr::VRInput()->GetInputSourceHandle("/user/hand/right", &m_Controllers[right].m_Source);
			vr::VRInput()->GetActionHandle("/actions/demo/in/Hand_Right", &m_Controllers[right].m_ActionPose);
			return true;
		}

		bool VRhandler::SetupRenderStereoTargets() 
		{
			if (!m_HMD)
				return false;
			
			m_HMD->GetRecommendedRenderTargetSize(&m_RenderWidth, &m_RenderHeight);

			CreateFrameBuffer(m_RenderWidth, m_RenderHeight, m_LeftEyeDesc);
			CreateFrameBuffer(m_RenderWidth, m_RenderHeight, m_RightEyeDesc);
		
			return true;
		}

		bool VRhandler::CreateFrameBuffer(int renderWidth, int renderHeight, FrameBufferDesc &bufferDesc) 
		{
			glGenFramebuffers(1, &bufferDesc.m_RenderFrameBufferID);
			glBindFramebuffer(GL_FRAMEBUFFER, bufferDesc.m_RenderFrameBufferID);

			glGenRenderbuffers(1, &bufferDesc.m_DepthBufferID);
			glBindRenderbuffer(GL_RENDERBUFFER, bufferDesc.m_DepthBufferID);
			glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH_COMPONENT, renderWidth, renderHeight);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, bufferDesc.m_DepthBufferID);

			glGenTextures(1, &bufferDesc.m_RenderTextureID);
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, bufferDesc.m_RenderTextureID);
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGBA8, renderWidth, renderHeight, true);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, bufferDesc.m_RenderTextureID, 0);

			glGenFramebuffers(1, &bufferDesc.m_ResolveFrameBufferID);
			glBindFramebuffer(GL_FRAMEBUFFER, bufferDesc.m_ResolveFrameBufferID);

			glGenTextures(1, &bufferDesc.m_ResolveTextureID);
			glBindTexture(GL_TEXTURE_2D, bufferDesc.m_ResolveTextureID);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, renderWidth, renderHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bufferDesc.m_ResolveTextureID, 0);

			// check FBO status
			GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
			if (status != GL_FRAMEBUFFER_COMPLETE)
			{
				return false;
			}

			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			return true;
		}

		void VRhandler::InitCompanionWindow()
		{
			if (!m_HMD)
				return;

			std::vector<ScreenVertexData> Verts;

			// left eye verts
			Verts.push_back(ScreenVertexData{glm::vec2(-1,-1), glm::vec2(0, 0)});	//Bottom Left
			Verts.push_back(ScreenVertexData{glm::vec2( 0,-1), glm::vec2(1, 0)});	//Bottom Right
			Verts.push_back(ScreenVertexData{glm::vec2(-1, 1), glm::vec2(0, 1)});	//Top Left
			Verts.push_back(ScreenVertexData{glm::vec2( 0, 1), glm::vec2(1, 1)});	//Top Right

			// right eye verts
			Verts.push_back(ScreenVertexData{glm::vec2(0,-1), glm::vec2(0, 0)});	//Bottom Left
			Verts.push_back(ScreenVertexData{glm::vec2(1,-1), glm::vec2(1, 0)});	//Bottom Right
			Verts.push_back(ScreenVertexData{glm::vec2(0, 1), glm::vec2(0, 1)});	//Top Left
			Verts.push_back(ScreenVertexData{glm::vec2(1, 1), glm::vec2(1, 1)});	//Top Right

			GLushort Indices[] = { 0, 1, 3,   0, 3, 2,   4, 5, 7,   4, 7, 6 };
			m_CWindowVertexCount = _countof(Indices);

			glGenVertexArrays(1, &m_CWindowVAO);
			glBindVertexArray(m_CWindowVAO);

			glGenBuffers(1, &m_CWindowVBO);
			glBindBuffer(GL_ARRAY_BUFFER, m_CWindowVBO);
			glBufferData(GL_ARRAY_BUFFER, Verts.size() * sizeof(ScreenVertexData), &Verts[0], GL_STATIC_DRAW);

			glGenBuffers(1, &m_CWindowIBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_CWindowIBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_CWindowVertexCount * sizeof(GLushort), &Indices[0], GL_STATIC_DRAW);

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(ScreenVertexData), (void*)offsetof(ScreenVertexData, m_Pos));

			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(ScreenVertexData), (void*)offsetof(ScreenVertexData, m_UVs));

			glBindVertexArray(0);

			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(1);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}

		void VRhandler::RenderFrame() 
		{
			if (m_HMD) 
			{
				RenderAxis();
				RenderStereoTargets();
				RenderWindow();

				vr::Texture_t LeftEyeTexture = { (void*)(uintptr_t)m_LeftEyeDesc.m_ResolveTextureID, vr::TextureType_OpenGL, vr::ColorSpace_Gamma };
				vr::VRCompositor()->Submit(vr::Eye_Left, &LeftEyeTexture);
				vr::Texture_t RightEyeTexture = {(void*)(uintptr_t)m_RightEyeDesc.m_ResolveTextureID, vr::TextureType_OpenGL, vr::ColorSpace_Gamma };
				vr::VRCompositor()->Submit(vr::Eye_Right, &RightEyeTexture);
			}

			{
				SDL_GL_SwapWindow(m_Window);
			}

			{
				glClearColor(0, 0, 0, 1);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			}

			if (m_ControllerCount != m_ControllerCountLast || m_ValidPoseCount != m_ValidPoseCountLast)
			{
				m_ValidPoseCountLast = m_ValidPoseCount;
				m_ControllerCountLast = m_ControllerCount;

				std::cout << "PoseCount: " << m_ValidPoseCount << "\nPose Classes: "<< m_PoseClasses << "\nControllers: " << m_ControllerCount << '\n';
			}

			UpdateHMDPose();
		}

		void VRhandler::RenderAxis() 
		{
			if (!m_HMD->IsInputAvailable()) 
				return;
			std::vector<float> vertexdata;

			m_ControllerVertexCount = 0;
			m_ControllerCount = 0;

			for (EHand hand = left; hand <= right; ((int&)hand)++) 
			{
				if (!m_Controllers[hand].m_ShowController)
					continue;

				const glm::mat4& mat = m_Controllers[hand].m_ModelMatrix;
				glm::vec4 centre = mat * glm::vec4{ 0,0,0,1 };

				for (int i = 0; i < 3; i++) 
				{
					glm::vec3 color( 0.0f );
					glm::vec4 point( 0,0,0,1 );
					point[i] += 0.05f;
					color[i] = 1.0f;
					point = mat * point;
					vertexdata.push_back(centre.x);
					vertexdata.push_back(centre.y);
					vertexdata.push_back(centre.z);

					vertexdata.push_back(color.x);
					vertexdata.push_back(color.y);
					vertexdata.push_back(color.z);

					vertexdata.push_back(point.x);
					vertexdata.push_back(point.y);
					vertexdata.push_back(point.z);

					vertexdata.push_back(color.x);
					vertexdata.push_back(color.y);
					vertexdata.push_back(color.z);

					m_ControllerVertexCount += 2;
				}

				glm::vec4 start = mat * glm::vec4{ 0,0, -0.02f,1 };
				glm::vec4 end = mat * glm::vec4{ 0,0,-39.f, 1 };
				glm::vec3 color{ .92f,.92f,.71f };

				vertexdata.push_back(start.x);
				vertexdata.push_back(start.y);
				vertexdata.push_back(start.z);
				vertexdata.push_back(color.x);
				vertexdata.push_back(color.y);
				vertexdata.push_back(color.z);

				vertexdata.push_back(end.x);
				vertexdata.push_back(end.y);
				vertexdata.push_back(end.z);
				vertexdata.push_back(color.x);
				vertexdata.push_back(color.y);
				vertexdata.push_back(color.z);

				m_ControllerVertexCount += 2;
			}

			if (m_ControllerVAO == 0) 
			{
				glGenVertexArrays(1, &m_ControllerVAO);
				glBindVertexArray(m_ControllerVAO);

				glGenBuffers(1, &m_ControllerVBO);
				glBindBuffer(GL_ARRAY_BUFFER, m_ControllerVBO);

				GLuint stride = 2 * 3 * sizeof(float);
				uintptr_t offset = 0;

				glEnableVertexAttribArray(0);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (const void*)offset);

				offset += sizeof(glm::vec3);
				glEnableVertexAttribArray(1);
				glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (const void*)offset);

				glBindVertexArray(0);
			}

			glBindBuffer(GL_ARRAY_BUFFER, m_ControllerVBO);

			if (vertexdata.size() != 0)
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexdata.size(), &vertexdata[0], GL_STREAM_DRAW);
		}

		void VRhandler::RenderStereoTargets() 
		{
			glClearColor(0, 0, 0, 1.0f);
			glEnable(GL_MULTISAMPLE);

			glBindFramebuffer(GL_FRAMEBUFFER, m_LeftEyeDesc.m_RenderFrameBufferID);
			glViewport(0, 0, m_RenderWidth, m_RenderHeight);
			RenderScene(vr::Eye_Left);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			glDisable(GL_MULTISAMPLE);

			glBindFramebuffer(GL_READ_FRAMEBUFFER, m_LeftEyeDesc.m_RenderFrameBufferID);
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_LeftEyeDesc.m_ResolveFrameBufferID);

			glBlitFramebuffer(0, 0, m_RenderWidth, m_RenderHeight, 0, 0, m_RenderWidth, m_RenderHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);

			glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

			glEnable(GL_MULTISAMPLE);

			glBindFramebuffer(GL_FRAMEBUFFER, m_RightEyeDesc.m_RenderFrameBufferID);
			glViewport(0, 0, m_RenderWidth, m_RenderHeight);
			RenderScene(vr::Eye_Right);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			glDisable(GL_MULTISAMPLE);

			glBindFramebuffer(GL_READ_FRAMEBUFFER, m_RightEyeDesc.m_RenderFrameBufferID);
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_RightEyeDesc.m_ResolveFrameBufferID);

			glBlitFramebuffer(0, 0, m_RenderWidth, m_RenderHeight, 0, 0, m_RenderWidth, m_RenderHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);

			glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		}
		
		void VRhandler::RenderWindow() 
		{
			glDisable(GL_DEPTH_TEST);
			glViewport(0, 0, 1280, 800);

			glBindVertexArray(m_CWindowVAO);
			m_WindowShader->Use();

			// render left eye (first half of index array )
			glBindTexture(GL_TEXTURE_2D, m_LeftEyeDesc.m_ResolveTextureID);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glDrawElements(GL_TRIANGLES, m_CWindowVertexCount / 2, GL_UNSIGNED_SHORT, 0);

			// render right eye (second half of index array )
			glBindTexture(GL_TEXTURE_2D, m_RightEyeDesc.m_ResolveTextureID);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glDrawElements(GL_TRIANGLES, m_CWindowVertexCount / 2, GL_UNSIGNED_SHORT, (const void*)(uintptr_t)(m_CWindowVertexCount));

			glBindVertexArray(0);
			glUseProgram(0);
		
		}
		
		void VRhandler::RenderScene(vr::Hmd_Eye eye) 
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glEnable(GL_DEPTH_TEST);

			//Loaded Model Rendering
			MDL::ModelManager::Instance()->Draw(m_ModelShader, GetViewProjectionMatrix(eye));
			MDL::ModelManager::Instance()->Draw(m_PointShader, GetViewProjectionMatrix(eye));
			
			bool InputReady = m_HMD->IsInputAvailable();

			//X,Y,Z Axis line Rendering
			if (InputReady) 
			{
				m_ControllerShader->Use();
				m_ControllerShader->SetMat4("matrix", GetViewProjectionMatrix(eye));
				glBindVertexArray(m_ControllerVAO);
				glDrawArrays(GL_LINES, 0, m_ControllerVertexCount);
				glBindVertexArray(0);
			}


			m_RenderModelShader->Use();
			
			//Controller Model Rendering
			for (EHand Hand = left; Hand <= right; ((int&)Hand)++)
			{
				if (!m_Controllers[Hand].m_ShowController || !m_Controllers[Hand].m_Model)
					continue;
				const glm::mat4 &mat = GetViewProjectionMatrix(eye) * m_Controllers[Hand].m_ModelMatrix;
				m_RenderModelShader->SetMat4("matrix", mat);
				m_Controllers[Hand].m_Model->Draw();
			}

			glUseProgram(0);
			
		}
		
		void VRhandler::HandleInput() 
		{
			vr::VREvent_t event;
			while (m_HMD->PollNextEvent(&event, sizeof(event)))
				ProcessVREvent(event);

			vr::VRActiveActionSet_t actionSet = { 0 };
			actionSet.ulActionSet = m_actionsetDemo;
			vr::VRInput()->UpdateActionState(&actionSet, sizeof(actionSet), 1);

			vr::VRInputValueHandle_t ulHapticDevice;
			if (GetDigitalActionRisingEdge(m_ActionTriggerHaptic, &ulHapticDevice))
			{
				if (ulHapticDevice == m_Controllers[left].m_Source)
				{
					vr::VRInput()->TriggerHapticVibrationAction(m_Controllers[left].m_ActionHaptic, 0, 1, 4.f, 1.0f, vr::k_ulInvalidInputValueHandle);
				}
				if (ulHapticDevice == m_Controllers[right].m_Source)
				{
					vr::VRInput()->TriggerHapticVibrationAction(m_Controllers[right].m_ActionHaptic, 0, 1, 4.f, 1.0f, vr::k_ulInvalidInputValueHandle);
				}
			}

			vr::InputAnalogActionData_t analogData;
			if (vr::VRInput()->GetAnalogActionData(m_ActionAnalongInput, &analogData, sizeof(analogData), vr::k_ulInvalidInputValueHandle) == vr::VRInputError_None && analogData.bActive)
			{
				m_AnalogValue.x = analogData.x;
				m_AnalogValue.y = analogData.y;
			}

			m_Controllers[left].m_ShowController = true;
			m_Controllers[right].m_ShowController = true;

			vr::VRInputValueHandle_t ulHideDevice;
			if (GetDigitalActionState(m_ActionHideController, &ulHideDevice))
			{
				if (ulHideDevice == m_Controllers[left].m_Source)
				{
					m_Controllers[left].m_ShowController = false;
				}
				if (ulHideDevice == m_Controllers[right].m_Source)
				{
					m_Controllers[right].m_ShowController = false;
				}
			}

			for (EHand hand = left; hand <= right; ((int&)hand)++)
			{
				vr::InputPoseActionData_t poseData;
				if (vr::VRInput()->GetPoseActionDataForNextFrame(m_Controllers[hand].m_ActionPose, vr::TrackingUniverseStanding, &poseData, sizeof(poseData), vr::k_ulInvalidInputValueHandle) != vr::VRInputError_None
					|| !poseData.bActive || !poseData.pose.bPoseIsValid)
				{
					m_Controllers[hand].m_ShowController = false;
				}
				else
				{
					m_Controllers[hand].m_ModelMatrix = Math::SteamMatrixToGlmMatrix(poseData.pose.mDeviceToAbsoluteTracking);

					vr::InputOriginInfo_t originInfo;
					if (vr::VRInput()->GetOriginTrackedDeviceInfo(poseData.activeOrigin, &originInfo, sizeof(originInfo)) == vr::VRInputError_None
						&& originInfo.trackedDeviceIndex != vr::k_unTrackedDeviceIndexInvalid)
					{

						std::string sRenderModelName = GetDeviceString(originInfo.trackedDeviceIndex, vr::Prop_RenderModelName_String);
						if (sRenderModelName != m_Controllers[hand].m_ModelName)
						{
							m_Controllers[hand].m_Model = LoadVRModel(sRenderModelName.c_str());
							m_Controllers[hand].m_ModelName = sRenderModelName;
						}
					}
				}
			}
		}

		void VRhandler::ProcessVREvent(vr::VREvent_t event) 
		{
			switch (event.eventType) 
			{
			case vr::VREvent_TrackedDeviceActivated:
				std::cout << "Device Activated: " << event.trackedDeviceIndex << '\n';
				break;
			case vr::VREvent_TrackedDeviceDeactivated:
				std::cout << "Device Dectivated: " << event.trackedDeviceIndex << '\n';
				break;
			case vr::VREvent_TrackedDeviceUpdated:
				std::cout << "Device Updated: " << event.trackedDeviceIndex << '\n';
				break;
			}
		}
		
		void VRhandler::UpdateHMDPose() 
		{
			if (!m_HMD)
				return;

			vr::VRCompositor()->WaitGetPoses(m_DevicePose, vr::k_unMaxTrackedDeviceCount, NULL, 0);

			m_ValidPoseCount = 0;
			m_PoseClasses = "";
			for (int nDevice = 0; nDevice < vr::k_unMaxTrackedDeviceCount; ++nDevice)
			{
				if (m_DevicePose[nDevice].bPoseIsValid)
				{
					m_ValidPoseCount++;
					m_DeviceMatrix[nDevice] = Math::SteamMatrixToGlmMatrix(m_DevicePose[nDevice].mDeviceToAbsoluteTracking);
					
					if (m_DeviceClass[nDevice] == 0)
					{
						std::cout << m_HMD->GetTrackedDeviceClass(nDevice) << '\n';
						switch (m_HMD->GetTrackedDeviceClass(nDevice))
						{
						case vr::TrackedDeviceClass_Controller:        m_DeviceClass[nDevice] = 'C'; break;
						case vr::TrackedDeviceClass_HMD:               m_DeviceClass[nDevice] = 'H'; break;
						case vr::TrackedDeviceClass_Invalid:           m_DeviceClass[nDevice] = 'I'; break;
						case vr::TrackedDeviceClass_GenericTracker:    m_DeviceClass[nDevice] = 'G'; break;
						case vr::TrackedDeviceClass_TrackingReference: m_DeviceClass[nDevice] = 'T'; break;
						default:                                       m_DeviceClass[nDevice] = '?'; break;
						}
					}
					m_PoseClasses += m_DeviceClass[nDevice];
				}
			}

			if (m_DevicePose[vr::k_unTrackedDeviceIndex_Hmd].bPoseIsValid)
			{
				m_HMDMat = m_DeviceMatrix[vr::k_unTrackedDeviceIndex_Hmd];
				m_HMDMat = glm::inverse(m_HMDMat);
			}
		}
		
		std::string VRhandler::GetDeviceString(vr::TrackedDeviceIndex_t unDevice, vr::TrackedDeviceProperty prop, vr::TrackedPropertyError* peError)
		{
			uint32_t unRequiredBufferLen = vr::VRSystem()->GetStringTrackedDeviceProperty(unDevice, prop, NULL, 0, peError);
			if (unRequiredBufferLen == 0)
				return "";

			char* pchBuffer = new char[unRequiredBufferLen];
			unRequiredBufferLen = vr::VRSystem()->GetStringTrackedDeviceProperty(unDevice, prop, pchBuffer, unRequiredBufferLen, peError);
			std::string sResult = pchBuffer;
			delete[] pchBuffer;
			return sResult;
		}

		glm::mat4 VRhandler::GetViewProjectionMatrix(vr::Hmd_Eye eye) 
		{
			glm::mat4 matMVP;
			if (eye == vr::Eye_Left)
			{
				matMVP = m_ProjectionLeft * m_LeftEyeMat * m_HMDMat;
			}
			else if (eye == vr::Eye_Right)
			{
				matMVP = m_ProjectionRight * m_RightEyeMat * m_HMDMat;
			}

			return matMVP;
		}
		
		glm::mat4 VRhandler::GetHMDProjectionMatrix(vr::Hmd_Eye eye) 
		{
			if (!m_HMD)
				return glm::mat4{ 1.0f };
			vr::HmdMatrix44_t mat = m_HMD->GetProjectionMatrix(eye, 0.1f, 30.0f);

			return glm::mat4(
				mat.m[0][0], mat.m[1][0], mat.m[2][0], mat.m[3][0],
				mat.m[0][1], mat.m[1][1], mat.m[2][1], mat.m[3][1],
				mat.m[0][2], mat.m[1][2], mat.m[2][2], mat.m[3][2],
				mat.m[0][3], mat.m[1][3], mat.m[2][3], mat.m[3][3]  );
		}

		glm::mat4 VRhandler::GetHMDEyePos(vr::Hmd_Eye eye) 
		{
			if (!m_HMD)
				return glm::mat4{ 1.0f };
			vr::HmdMatrix34_t mat = m_HMD->GetEyeToHeadTransform(eye);

			return glm::mat4(
				mat.m[0][0], mat.m[1][0], mat.m[2][0], 0.0,
				mat.m[0][1], mat.m[1][1], mat.m[2][1], 0.0,
				mat.m[0][2], mat.m[1][2], mat.m[2][2], 0.0,
				mat.m[0][3], mat.m[1][3], mat.m[2][3], 1.0f );
		}

		bool VRhandler::GetDigitalActionRisingEdge(vr::VRActionHandle_t action, vr::VRInputValueHandle_t* pDevicePath)
		{
			vr::InputDigitalActionData_t actionData;
			vr::VRInput()->GetDigitalActionData(action, &actionData, sizeof(actionData), vr::k_ulInvalidInputValueHandle);
			if (pDevicePath)
			{
				*pDevicePath = vr::k_ulInvalidInputValueHandle;
				if (actionData.bActive)
				{
					vr::InputOriginInfo_t originInfo;
					if (vr::VRInputError_None == vr::VRInput()->GetOriginTrackedDeviceInfo(actionData.activeOrigin, &originInfo, sizeof(originInfo)))
					{
						*pDevicePath = originInfo.devicePath;
					}
				}
			}
			return actionData.bActive && actionData.bChanged && actionData.bState;
		}

		bool VRhandler::GetDigitalActionState(vr::VRActionHandle_t action, vr::VRInputValueHandle_t* pDevicePath)
		{
			vr::InputDigitalActionData_t actionData;
			vr::VRInput()->GetDigitalActionData(action, &actionData, sizeof(actionData), vr::k_ulInvalidInputValueHandle);
			if (pDevicePath)
			{
				*pDevicePath = vr::k_ulInvalidInputValueHandle;
				if (actionData.bActive)
				{
					vr::InputOriginInfo_t originInfo;
					if (vr::VRInputError_None == vr::VRInput()->GetOriginTrackedDeviceInfo(actionData.activeOrigin, &originInfo, sizeof(originInfo)))
					{
						*pDevicePath = originInfo.devicePath;
					}
				}
			}
			return actionData.bActive && actionData.bState;
		}

		MDL::Model* VRhandler::LoadVRModel(const char* ModelName)
		{
			for (std::vector< MDL::Model* >::iterator i = m_ControllerModels.begin(); i != m_ControllerModels.end(); i++)
			{
			
				if ((*i)->m_FilePath == ModelName)
				{
					std::cout << "Here" << '\n';
					return (*i);
				}
			}

			std::vector<Math::Vertex> vertices;
			std::vector<unsigned int> indices;

			vr::RenderModel_t* model;
			vr::EVRRenderModelError err;

			while (1)
			{
				err = vr::VRRenderModels()->LoadRenderModel_Async(ModelName, &model);
				if (err != vr::VRRenderModelError_Loading)
					break;
				#ifdef _WIN32
					Sleep(1);
				#else
					usleep(1 * 1000);
				#endif // _WIN32
			}

			if (err != vr::VRRenderModelError_None)
			{
				std::cout << "Unable To load Controller Model. ERROR: " << vr::VRRenderModels()->GetRenderModelErrorNameFromEnum(err) << '\n';
				return nullptr;
			}

			for (int i = 0; i < (int)model->unVertexCount; i++)
			{
				//X,Y,Z of vertex info
				Math::Vertex vertex;
				glm::vec3 vector;
				glm::vec2 uv;
				vector.x = model->rVertexData[i].vPosition.v[0];
				vector.y = model->rVertexData[i].vPosition.v[1];
				vector.z = model->rVertexData[i].vPosition.v[2];
				vertex.m_Pos = glm::vec4(vector, 1.0f);

				vector.x = model->rVertexData[i].vNormal.v[0];
				vector.y = model->rVertexData[i].vNormal.v[1];
				vector.z = model->rVertexData[i].vNormal.v[2];
				vertex.m_Normal = vector;

				uv.x = model->rVertexData[i].rfTextureCoord[0];
				uv.y = model->rVertexData[i].rfTextureCoord[1];
				vertex.m_UVs = uv;

				vertices.push_back(vertex);
			};

			for (int i = 0; i < (int)model->unTriangleCount * 3; i++)
			{
				indices.push_back(model->rIndexData[i]);
			}

			vr::RenderModel_TextureMap_t* pTexture;
			while (1)
			{
				err = vr::VRRenderModels()->LoadTexture_Async(model->diffuseTextureId, &pTexture);
				if (err != vr::VRRenderModelError_Loading)
					break;

				#ifdef _WIN32
					Sleep(1);
				#else
					usleep(1 * 1000);
				#endif // _WIN32
			}

			if (err != vr::VRRenderModelError_None)
			{
				std::cout << "Unable to load render texture id: " << model->diffuseTextureId << " for render model: " << ModelName << '\n';
				vr::VRRenderModels()->FreeRenderModel(model);
				return NULL; // move on to the next tracked device
			}
			m_ControllerModels.push_back(new MDL::Model(ModelName, vertices, indices, {}, glm::mat4(1.0f), LoadVRTexture(*pTexture)));
			vr::VRRenderModels()->FreeRenderModel(model);
			vr::VRRenderModels()->FreeTexture(pTexture);
			
			return m_ControllerModels.back();
		}

		unsigned int VRhandler::LoadVRTexture(vr::RenderModel_TextureMap_t& pTexture)
		{
			unsigned int textureid;
			glGenTextures(1, &textureid);
			glBindTexture(GL_TEXTURE_2D, textureid);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pTexture.unWidth, pTexture.unHeight,
				0, GL_RGBA, GL_UNSIGNED_BYTE, pTexture.rubTextureMapData);

			// If this renders black ask McJohn what's wrong.
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

			GLfloat fLargest;
			glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &fLargest);
			glTexParameterf(GL_TEXTURE_2D, GL_MAX_TEXTURE_MAX_ANISOTROPY, fLargest);

			glBindTexture(GL_TEXTURE_2D, 0);

			return textureid;
		}

	}
}