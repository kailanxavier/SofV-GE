#include "AppWindow.h"
#include "InputSystem.h" 


int main()
{
	try
	{
		GraphicsEngine::create();
	}
	catch (...)
	{
		return -1;
	}
	{
		try 
		{
			AppWindow app;
			while (app.isRun())
			{
				app.broadcast();
			}
		}
		catch (...) 
		{
			InputSystem::release();
			GraphicsEngine::release();

			return -1;
		}
	}

	InputSystem::release();
	GraphicsEngine::release();

	return 0;
}