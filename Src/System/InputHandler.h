#pragma once
#ifndef __InputHandler__
#define __InputHandler__

#include <openvr.h>
#include <glm/glm.hpp>
#include <openvr.h>
#include <vector>
#include <iostream>

namespace TDVR {
	namespace VR 
	{
		struct VRControllerInfo;
		enum EHand;
	}
}

namespace TDVR {
	namespace INPT {


		class InputHandler
		{
		public:

			//Public Member Functions
			void InitActionHandles(VR::VRControllerInfo* controllers);
			void HandleInput(VR::VRControllerInfo* controllers);
			void ProcessVREvent(vr::VREvent_t event);

			//Essentials
			InputHandler() : m_AnalogValue(glm::vec3(0.0f)) {}

			~InputHandler();
		private:

			//Device Action Variables
			vr::VRActionHandle_t m_ActionMenuButton = vr::k_ulInvalidActionHandle;
			vr::VRActionHandle_t m_ActionTriggerSelect = vr::k_ulInvalidActionHandle;
			vr::VRActionHandle_t m_ActionGripTransform = vr::k_ulInvalidActionHandle;
			vr::VRActionHandle_t m_ActionTPadModeSelect = vr::k_ulInvalidActionHandle;
			vr::VRActionHandle_t m_ActionTPadAxisValue = vr::k_ulInvalidActionHandle;
			
			vr::VRActionSetHandle_t m_actionsetDemo = vr::k_ulInvalidActionSetHandle;
			
			glm::vec2 m_AnalogValue;

			

			//Private Functions
			void ModelManipulation(VR::VRControllerInfo* controllers);
			bool GetDigitalActionRisingEdge(vr::VRActionHandle_t action, VR::VRControllerInfo controller, vr::VRInputValueHandle_t* pDevicePath = nullptr);
			bool GetDigitalActionState(vr::VRActionHandle_t action, VR::VRControllerInfo controller, vr::VRInputValueHandle_t* pDevicePath = nullptr);
			
		};
	}
}
#endif
