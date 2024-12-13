#include "GraphicsEngine.h"
#include "RenderSystem.h"
#include <exception>

GraphicsEngine* GraphicsEngine::m_engine = nullptr;

GraphicsEngine::GraphicsEngine()
{
	try
	{
		m_render_system = new RenderSystem();
	}
	catch (...)
	{
		throw std::exception("GraphicsEngine could not be created successfully.");
	}
}

RenderSystem* GraphicsEngine::getRenderSystem()
{
	return m_render_system;
}

GraphicsEngine::~GraphicsEngine()
{
	GraphicsEngine::m_engine = nullptr;
	delete m_render_system;
}

void GraphicsEngine::create()
{
	if (GraphicsEngine::m_engine)
		throw std::exception("GraphicsEngine already created.");
	GraphicsEngine::m_engine = new GraphicsEngine();
}

void GraphicsEngine::release()
{
	if (!GraphicsEngine::m_engine)
		return;
	delete GraphicsEngine::m_engine;
}

GraphicsEngine* GraphicsEngine::get()
{
	return m_engine;
}


