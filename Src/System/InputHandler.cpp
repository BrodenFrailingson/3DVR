#pragma once
#include "InputHandler.h"
#include "VRhandler.h"
#include "Geometry/ModelManager.h"

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif //_WIN32

namespace TDVR 
{
	namespace INPT 
	{
		void InputHandler::InitActionHandles(VR::VRControllerInfo* controllers) 
		{
			std::cout << "here" << '\n';
			std::string strpath = SRC_DIR + (std::string)"/" + "Bin/" + "example_actions.json";
			const char* filepath = strpath.c_str();
			vr::VRInput()->SetActionManifestPath(filepath);

			vr::VRInput()->GetActionHandle("/actions/demo/in/SelectMode", &m_ActionTPadModeSelect);	
			vr::VRInput()->GetActionHandle("/actions/demo/in/HideController", &m_ActionMenuButton);	
			vr::VRInput()->GetActionHandle("/actions/demo/in/ChangeMode", &m_ActionTPadAxisValue);
			vr::VRInput()->GetActionHandle("/actions/demo/in/Manipulate", &m_ActionGripTransform);	
			vr::VRInput()->GetActionHandle("/actions/demo/in/Interact", &m_ActionTriggerSelect);	


			vr::VRInput()->GetActionSetHandle("/actions/demo", &m_actionsetDemo);
			vr::VRInput()->GetActionHandle("/actions/demo/out/Haptic_Left", &controllers[VR::left].m_ActionHaptic);
			vr::VRInput()->GetInputSourceHandle("/user/hand/left", &controllers[VR::left].m_Source);
			vr::VRInput()->GetActionHandle("/actions/demo/in/Hand_Left", &controllers[VR::left].m_ActionPose);
			vr::VRInput()->GetActionHandle("/actions/demo/out/Haptic_Right", &controllers[VR::right].m_ActionHaptic);	
			vr::VRInput()->GetInputSourceHandle("/user/hand/right", &controllers[VR::right].m_Source);	
			vr::VRInput()->GetActionHandle("/actions/demo/in/Hand_Right", &controllers[VR::right].m_ActionPose);

		}

		void InputHandler::HandleInput(VR::VRControllerInfo* controllers) 
		{
			vr::VRActiveActionSet_t actionSet = { 0 };
			actionSet.ulActionSet = m_actionsetDemo;
			vr::VRInput()->UpdateActionState(&actionSet, sizeof(actionSet), 1);

			vr::VRInputValueHandle_t MenuPress, TriggerPress, GripPress, TPadPress;
			vr::InputAnalogActionData_t TPadTouch;

			controllers[VR::left].m_ShowController = true;
			controllers[VR::right].m_ShowController = true;


			//Manages Menu button actions
			for (VR::EHand hand = VR::left; hand <= VR::right; ((int&)hand)++) {
				if (GetDigitalActionState(m_ActionMenuButton, controllers[hand], &MenuPress))
				{
					if (MenuPress == controllers[VR::left].m_Source)
						controllers[VR::left].m_ShowController = false;
					if (MenuPress == controllers[VR::right].m_Source)
						controllers[VR::right].m_ShowController = false;
				}


				//Manages Trigger Pulls
				if (GetDigitalActionRisingEdge(m_ActionTriggerSelect, controllers[hand] ,&TriggerPress))
				{
					if (TriggerPress == controllers[VR::left].m_Source)
						MDL::ModelManager::Instance()->VertexSelect(controllers[VR::left].m_SelectionPos);
					if (TriggerPress == controllers[VR::right].m_Source)
						MDL::ModelManager::Instance()->VertexSelect(controllers[VR::right].m_SelectionPos);

				}

				//Manages Grip Presses
				if (GetDigitalActionState(m_ActionGripTransform, controllers[hand], &GripPress))
				{
					if (GripPress == controllers[VR::left].m_Source)
						MDL::ModelManager::Instance()->MoveVertex(controllers[VR::left].m_SelectionPos);
					if (GripPress == controllers[VR::right].m_Source)
						MDL::ModelManager::Instance()->MoveVertex(controllers[VR::right].m_SelectionPos);

				}

				//Manages TouchPadPresses
				if (GetDigitalActionRisingEdge(m_ActionTPadModeSelect, controllers[hand], &TPadPress))
				{
					if (TPadPress == controllers[VR::left].m_Source)
						std::cout << "Left Controller TouchPad Pressed!" << '\n';
					if (TPadPress == controllers[VR::right].m_Source)
						std::cout << "Right Controller TouchPad Pressed!" << '\n';

				}

				//Manages TouchPad analog data
				if (vr::VRInput()->GetAnalogActionData(m_ActionTPadAxisValue, &TPadTouch, sizeof(TPadTouch), vr::k_ulInvalidInputValueHandle) == vr::VRInputError_None && TPadTouch.bActive)
				{
					m_AnalogValue.x = TPadTouch.x;
					m_AnalogValue.y = TPadTouch.y;
				}
			}

		}

		void InputHandler::ProcessVREvent(vr::VREvent_t event)
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

		bool InputHandler::GetDigitalActionRisingEdge(vr::VRActionHandle_t action, VR::VRControllerInfo controller, vr::VRInputValueHandle_t* pDevicePath)
		{
			vr::InputDigitalActionData_t actionData;
			vr::VRInput()->GetDigitalActionData(action, &actionData, sizeof(actionData), controller.m_Source);

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

		bool InputHandler::GetDigitalActionState(vr::VRActionHandle_t action,VR::VRControllerInfo controller, vr::VRInputValueHandle_t* pDevicePath)
		{
			vr::InputDigitalActionData_t actionData;
			vr::VRInput()->GetDigitalActionData(action, &actionData, sizeof(actionData), controller.m_Source);
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
	}
}