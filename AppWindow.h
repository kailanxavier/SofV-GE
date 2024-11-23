#pragma once
#include "Window.h"
#include "GraphicsEngine.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"

class AppWindow : public Window
{
public:
	AppWindow();

	void updateQuadPosition();

	~AppWindow();

	// Inherited via Window
	virtual void onCreate() override;
	virtual void onUpdate() override;
	virtual void onDestroy() override;
private:
	SwapChain* m_swap_chain;
	VertexBuffer* m_vb; // Vertex buffer.
	VertexShader* m_vs; // Vertex shader.
	PixelShader* m_ps; // Pixel shader.
	ConstantBuffer* m_cb; // Constant buffer.
	IndexBuffer* m_ib; // Index buffer.
private:
	float m_old_delta;
	float m_new_delta;
	float m_delta_time;

	float m_delta_pos;
	float m_delta_scale;
	float m_delta_rot;
};