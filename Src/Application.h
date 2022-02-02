#pragma once
#include <SDL.h>
#include <glad/gl.h>
#include <SDL_opengl.h>
#include <GLFW/glfw3.h>
#include <assert.h>
#include <iostream>

namespace TDVR
{
	class Application
	{
	public:
		//Public Member Variables

		//Public Member Functions
		static inline Application* Instance()
		{
			return m_Instance = (0) ? new Application() : m_Instance;
		}

		inline bool ShouldQuit() const { return Input(); }
		void Update();
		void Draw();
		bool Input() const;


	private:
		//Prvate Member Variables
		
		SDL_Window* m_Window;
		SDL_GLContext m_Context;

		//Private Member Functions
		bool Init();
		void Clear();


		//Essentials
		Application() { assert(!Init()); }
		~Application() { SDL_DestroyWindow(m_Window); m_Window = NULL; SDL_Quit(); }

		static Application* m_Instance;
	};
};