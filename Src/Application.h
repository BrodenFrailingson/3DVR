#pragma once
#include "System/VRhandler.h"

namespace TDVR
{
	class Application
	{
	public:
		//Public Member Variables

		//Public Member Functions
		static inline Application* Instance()
		{
			return m_Instance = (m_Instance == 0) ? m_Instance = new Application() : m_Instance;
		}

		void Update();
		void Draw();
		bool Input() const;
		void Clear();

	private:
		//Prvate Member Variables
		VR::VRhandler* m_VRSystem;

		//Private Member Functions
		bool Init();
		


		//Essentials
		Application() { Init(); }
		~Application() { SDL_Quit(); }

		static Application* m_Instance;
	};
};