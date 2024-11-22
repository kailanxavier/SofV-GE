#pragma once
#include <Windows.h>

class Window
{
public:

	Window();

	// Initialise window
	bool init();
	bool broadcast();

	// Release window
	bool release();
	bool isRun();

	// EVENTS
	virtual void onCreate() = 0;
	virtual void onUpdate() = 0;
	virtual void onDestroy() = 0;

	~Window();

protected:
	HWND m_hwnd;
	bool m_is_run;
};

