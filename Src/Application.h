#pragma once
#ifndef __Application__
#define __Application__

#include <SDL2/SDL.h>

namespace TDVR 
{
	namespace VR { class VRhandler; }
}

namespace TDVR
{
	using VR::VRhandler;
	class Application
	{
	public:
		//Public Member Variables

		//Public Member Functions
		static inline Application* Instance()
		{
			return m_Instance = (m_Instance == 0) ? m_Instance = new Application() : m_Instance;
		}

		void Draw();
		bool Input() const;
		void Clear();

	private:
		//Prvate Member Variables
		VRhandler* m_VRSystem;

		//Private Member Functions
		bool Init();
		


		//Essentials
		Application() { Init(); }
		~Application() { SDL_Quit(); }

		static Application* m_Instance;
	};
}
#endif