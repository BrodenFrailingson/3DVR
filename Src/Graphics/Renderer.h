#pragma once

namespace TDVR { namespace Gra {

	class Renderer 
	{
	public:

		//Public Member Variables

		//Public member Functions
		static inline Renderer* Instance() 
		{
			return m_Instance = (0) ? new Renderer() : m_Instance;
		}

	private:

		//Private Member Variables
		bool Init();
		void Render();
		void Clean();
		//Private Member Functions

		//Essentials
		Renderer();
		~Renderer();

		static Renderer* m_Instance;
	};
}
}