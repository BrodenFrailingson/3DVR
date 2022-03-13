#include "Application.h"

using namespace TDVR;

int main(int argc , char* argv[]) 
{
	bool ShouldQuit = false;
	
	while (!ShouldQuit) 
	{
		ShouldQuit = Application::Instance()->Input();
		Application::Instance()->Draw();
	};
	Application::Instance()->Clear();
	MDL::ModelManager::Instance()->Clear();
	return 0;
}
