#pragma once
#include <d3d11.h>
#include "Prerequisites.h"
#include "RenderSystem.h"

class GraphicsEngine
{
public:
	GraphicsEngine();
	// Initialise the GraphicsEngine and DX11 device
	bool init();
	// Release all loaded resources
	bool release();
	RenderSystem* getRenderSystem();
	~GraphicsEngine();
public:
	static GraphicsEngine* get();
private:
	RenderSystem* m_render_system = nullptr;
};