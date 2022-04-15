#include "Application.h"
#include "Geometry/ModelManager.h"


int main(int argc , char* argv[]) 
{
	bool ShouldQuit = false;
	
	while (!ShouldQuit) 
	{
		ShouldQuit = TDVR::Application::Instance()->Input();
		TDVR::Application::Instance()->Draw();
	};
	TDVR::Application::Instance()->Clear();
	TDVR::MDL::ModelManager::Instance()->Clear();
	return 0;
}
