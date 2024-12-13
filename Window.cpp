// This script creates the application window

#include "Window.h"
#include <exception>

// Window* window=nullptr;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	//GetWindowLong(hwnd,)
	switch (msg)
	{
	case WM_CREATE:
	{
		break;
	}

	case WM_SETFOCUS:
	{
		// This event is fired when the window is focused.
		Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		if (window)
			window->onFocus();
		break;
	}

	case WM_KILLFOCUS:
	{
		// This event is fired when the window loses focus.
		Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		window->onKillFocus();
		break;
	}

	case WM_DESTROY:
	{
		// This event is fired when the window is destroyed.
		Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		window->onDestroy();
		::PostQuitMessage(0);
		break;
	}


	default:
		return ::DefWindowProc(hwnd, msg, wparam, lparam);
	}

	return NULL;
}

Window::Window()
{
	// WNDCLASSEX set-up.
	WNDCLASSEX wc;
	wc.cbClsExtra = NULL;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.cbWndExtra = NULL;
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = NULL;
	wc.lpszClassName = "MyWindowClass";
	wc.lpszMenuName = "";
	wc.style = NULL;
	wc.lpfnWndProc = &WndProc;

	if (!::RegisterClassEx(&wc)) // If the registration of class fails, the function will return false.
		throw std::exception("The window class could not be registered successfully.");

	/*if (!window)
		window = this; */

		// Creation of the window.
	m_hwnd = ::CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, "MyWindowClass", "SofV Engine", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1024, 768, NULL, NULL, NULL, this);

	// If the creation of the window fails, return false.
	if (!m_hwnd)
		throw std::exception("Window could not be created successfully.");

	// Draw window
	::ShowWindow(m_hwnd, SW_SHOW);
	::UpdateWindow(m_hwnd);


	// Set this flag to true to indicate that the window is running.
	m_is_run = true;
}

bool Window::broadcast()
{
	MSG msg;

	if (!this->m_is_init)
	{
		SetWindowLongPtr(m_hwnd, GWLP_USERDATA, (LONG_PTR)this);
		this->onCreate();
		this->m_is_init = true;
	}

	this->onUpdate();

	while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	Sleep(1);

	return true;
}


bool Window::isRun()
{
	return m_is_run;
}

RECT Window::getClientWindowRect()
{
	RECT rc;
	::GetClientRect(this->m_hwnd, &rc);
	return rc;
}

void Window::onCreate()
{
}

void Window::onUpdate()
{
}

void Window::onDestroy()
{
	m_is_run = false;
}

void Window::onFocus()
{

}

void Window::onKillFocus()
{
}

Window::~Window()
{
}