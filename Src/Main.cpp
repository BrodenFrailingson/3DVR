#include "Application.h"

using namespace TDVR;
int main(int argc , char* argv[]) 
{
	Application* inst = Application::Instance();
	while (Application::Instance()->ShouldQuit()) 
	{
		inst->Draw();
	};
	return 0;
}
