#include "Application.h"


namespace TDVR {

	Application* Application::m_Instance = 0;
	bool Application::Init()
	{
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) 
		{
			std::cerr << "ERROR: SDL Init Failed. Cause: " << SDL_GetError() << '\n';
			return false;
		}

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 0);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 0);

		m_VRSystem = new VR::VRhandler();
		return true;
	}

	void Application::Draw() 
	{
		m_VRSystem->HandleInput();
		m_VRSystem->RenderFrame();
	}

	bool Application::Input() const 
	{
		SDL_Event KeyEvent;
		bool RVal = false;

		while (SDL_PollEvent(&KeyEvent) != 0) 
		{
			switch (KeyEvent.type) 
			{
			case SDL_QUIT:
				RVal = true;
				break;

			case SDL_KEYDOWN:
				switch (KeyEvent.key.keysym.sym) 
				{
				case SDLK_ESCAPE:
					RVal = true;
					break;

				case SDLK_q:
					RVal = true;
					break;
				}
				break;
			}
		}
		return RVal;
	}

	void Application::Clear() 
	{
		delete m_VRSystem;
	}
}