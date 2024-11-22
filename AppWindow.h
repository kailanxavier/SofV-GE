#pragma once
#include "Window.h"



class AppWindow: public Window 
{
public:
	AppWindow();
	~AppWindow();



	// Inherited via Window
	void onCreate() override;

	void onUpdate() override;

	void onDestroy() override;

};

