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

		m_Window = SDL_CreateWindow("3DVR", 540, 960, 1280, 800, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
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

		if (!gladLoadGL(glfwGetProcAddress)) 
		{
			std::cerr << "ERROR: GLAD Init Failed. Cause: " << glGetError() << '\n';
			return false;
		}

		if (SDL_GL_SetSwapInterval(0) != 0) 
		{
			std::cerr << "ERROR: SDL VSync Init Failed. Cause: " << SDL_GetError() << '\n';
			return false;
		}
	}

	void Application::Draw() 
	{
		SDL_GL_SwapWindow(m_Window);
		int R, G, B;
		R= G = B = 0;
		R = (R > 255) ? 0 : R + 1;
		G = (G > 255) ? 0 : G + 2;
		B = (B > 255) ? 0 : B + 3;

		glClearColor(R, G, B, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
}