#pragma once
#include <d3d11.h>

class GraphicsEngine
{
public:
	GraphicsEngine();

	// Initialise GraphicsEngine and DX11 device.
	bool init();

	// Release loaded resources.
	bool release();
	~GraphicsEngine();
};

