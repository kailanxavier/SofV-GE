#pragma once
#include <d3d11.h>
#include "Prerequisites.h"
#include "RenderSystem.h"

class GraphicsEngine
{
private:
	// Initialise the GraphicsEngine and DX11 device
	GraphicsEngine();
	// Release all loaded resources
	~GraphicsEngine();
public:
	RenderSystem* getRenderSystem();
public:
	static GraphicsEngine* get();
	static void create();
	static void release();
private:
	RenderSystem* m_render_system = nullptr;
	static GraphicsEngine* m_engine;
};