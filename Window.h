#pragma once
#include <Windows.h>

class Window
{
public:
	// Initialise window.
	Window();

	bool broadcast();
	bool isRun();

	RECT getClientWindowRect();

	// Events:
	virtual void onCreate();
	virtual void onUpdate();
	virtual void onDestroy();
	virtual void onFocus();
	virtual void onKillFocus();

	// Release window.
	~Window();
protected:
	HWND m_hwnd;
	bool m_is_run;
	bool m_is_init = false;
};
