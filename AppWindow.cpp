#include "AppWindow.h"
#include <Windows.h>
#include "Vector3D.h"
#include "Matrix4x4.h"
#include "InputSystem.h"

struct vertex
{
	Vector3D position;
	Vector3D color;
	Vector3D color1;
};


__declspec(align(16)) // This controls the alignment of the struct.
struct constant
{
	Matrix4x4 m_world; // World
	Matrix4x4 m_view; // View
	Matrix4x4 m_proj; // Projection metrics
	unsigned int m_time;
};

AppWindow::AppWindow()
{
}

void AppWindow::update()
{
	constant cc;
	cc.m_time = ::GetTickCount();

	m_delta_pos += m_delta_time / 10.0f;
	if (m_delta_pos > 1.0f)
		m_delta_pos = 0;


	Matrix4x4 temp;

	m_delta_scale += m_delta_time / 0.55f;

	// cc.m_world.setScale(Vector3D::lerp(Vector3D(0.5, 0.5, 0), Vector3D(1.0f, 1.0f, 0), (sin(m_delta_scale) + 1.0f) / 2.0f));

	// temp.setTranslation(Vector3D::lerp(Vector3D(-1.5f, -1.5f, 0), Vector3D(1.5f, 1.5f, 0), m_delta_pos));

	// cc.m_world *= temp;

	/* cc.m_world.setScale(Vector3D(m_scale_cube, m_scale_cube, m_scale_cube));

	temp.setIdentity();
	temp.setRotationZ(0.0f);
	cc.m_world *= temp;

	temp.setIdentity();
	temp.setRotationY(m_rot_y);
	cc.m_world *= temp;

	temp.setIdentity();
	temp.setRotationX(m_rot_x);
	cc.m_world *= temp; */

	cc.m_world.setIdentity();

	Matrix4x4 world_cam;
	world_cam.setIdentity();

	temp.setIdentity();
	temp.setRotationX(m_rot_x);
	world_cam *= temp;

	temp.setIdentity();
	temp.setRotationY(m_rot_y);
	world_cam *= temp;
	

	Vector3D new_pos = m_world_cam.getTranslation() + world_cam.getZDirection() * (m_forward * 0.3f);

	new_pos = new_pos + world_cam.getXDirection() * (m_rightward * 0.3f);


	world_cam.setTranslation(new_pos);


	m_world_cam = world_cam;
	world_cam.inverse();



	cc.m_view = world_cam;

	/*cc.m_proj.setOrthoLH
	(
		(this->getClientWindowRect().right - this->getClientWindowRect().left) / 400.0f,
		(this->getClientWindowRect().bottom - this->getClientWindowRect().top) / 400.0f,
		-4.0f,
		4.0f
	); */

	int width = (this->getClientWindowRect().right - this->getClientWindowRect().left);
	int height = (this->getClientWindowRect().bottom - this->getClientWindowRect().top);

	cc.m_proj.setPerspectiveFovLH(1.57f, ((float) width /(float) height), 0.1f, 100.0f);

	m_cb->update(GraphicsEngine::get()->getImmediateDeviceContext(), &cc);
}

AppWindow::~AppWindow()
{
}

void AppWindow::onCreate()
{
	Window::onCreate();

	InputSystem::get()->addListener(this);
	InputSystem::get()->showCursor(false);

	GraphicsEngine::get()->init();
	m_swap_chain = GraphicsEngine::get()->createSwapChain();

	RECT rc = this->getClientWindowRect();
	m_swap_chain->init(this->m_hwnd, rc.right - rc.left, rc.bottom - rc.top);

	m_world_cam.setTranslation(Vector3D(0, 0, -2));

	vertex vertex_list[]=
	{
		// XYZ
		
		// Front face: 
		{Vector3D(-0.5f, -0.5f, -0.5f), Vector3D(1, 0, 0), Vector3D(0.2f, 0, 0)}, 
		{Vector3D(-0.5f, 0.5f, -0.5f), Vector3D(1, 1, 0), Vector3D(0.2f, 0.2f, 0)},
		{Vector3D(0.5f, 0.5f, -0.5f), Vector3D(1, 1, 0), Vector3D(0.2f, 0.2f, 0)},
		{Vector3D(0.5f, -0.5f, -0.5f), Vector3D(1, 0, 0), Vector3D(0.2f, 0, 0)},
		
		// Back face: 
		{Vector3D(0.5f, -0.5f, 0.5f), Vector3D(0, 1, 0), Vector3D(0, 0.2f, 0)},
		{Vector3D(00.5f, 0.5f, 0.5f), Vector3D(0, 1, 1), Vector3D(0, 0.2f, 0.2f)},
		{Vector3D(-0.5f, 0.5f, 0.5f), Vector3D(0, 1, 1), Vector3D(0, 0.2f, 0.2f)},
		{Vector3D(-0.5f, -0.5f, 0.5f), Vector3D(0, 1, 0), Vector3D(0, 0.2f, 0)}
	};

	m_vb = GraphicsEngine::get()->createVertexBuffer();
	UINT size_list = ARRAYSIZE(vertex_list);


	unsigned int index_list[] =
	{
		// Front side:
		0, 1, 2, // Triangle 1
		2, 3, 0, // Triangle 2

		// Back side:
		4, 5, 6, // Triangle 1
		6, 7, 4, // Triangle 2

		// Top side:
		1, 6, 5, // Triangle 1
		5, 2, 1, // Triangle 2

		// Bottom side:
		7, 0, 3, // Triangle 1
		3, 4, 7, // Triangle 2

		// Right side:
		3, 2, 5, // Triangle 1
		5, 4, 3, // Triangle 2

		// Left side:
		7, 6, 1, // Triangle 1
		1, 0, 7  // Triangle 2
	};


	m_ib = GraphicsEngine::get()->createIndexBuffer();
	UINT size_index_list = ARRAYSIZE(index_list); // This loads index list to the index buffer.


	m_ib->load(index_list, size_index_list);

	void* shader_byte_code = nullptr;
	size_t size_shader = 0;
	GraphicsEngine::get()->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);

	m_vs = GraphicsEngine::get()->createVertexShader(shader_byte_code, size_shader);
	m_vb->load(vertex_list, sizeof(vertex), size_list, shader_byte_code, size_shader);

	GraphicsEngine::get()->releaseCompiledShader();


	GraphicsEngine::get()->compilePixelShader(L"PixelShader.hlsl", "psmain", &shader_byte_code, &size_shader);
	m_ps = GraphicsEngine::get()->createPixelShader(shader_byte_code, size_shader);
	GraphicsEngine::get()->releaseCompiledShader();

	constant cc;
	cc.m_time = 0;

	
	m_cb = GraphicsEngine::get()->createConstantBuffer();
	m_cb->load(&cc, sizeof(constant));
}

void AppWindow::onUpdate()
{
	Window::onUpdate();

	InputSystem::get()->update();

	// This clear the target render:
	GraphicsEngine::get()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swap_chain, 0, 0.4f, 0.2f, 1);


	// This sets the viewport of the render target:
	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->getImmediateDeviceContext()->setViewportSize(rc.right - rc.left, rc.bottom - rc.top);

	update();

	GraphicsEngine::get()->getImmediateDeviceContext()->setConstantBuffer(m_vs, m_cb);
	GraphicsEngine::get()->getImmediateDeviceContext()->setConstantBuffer(m_ps, m_cb);


	// This sets the default shader in the graphics pipeline:
	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexShader(m_vs);
	GraphicsEngine::get()->getImmediateDeviceContext()->setPixelShader(m_ps);


	// This sets the VertexBuffer:
	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexBuffer(m_vb);


	// This sets the IndexBuffer:
	GraphicsEngine::get()->getImmediateDeviceContext()->setIndexBuffer(m_ib);


	// This draws the triangle:
	GraphicsEngine::get()->getImmediateDeviceContext()->drawIndexedTriangleList(m_ib->getSizeIndexList(), 0, 0);
	m_swap_chain->present(true);

	m_old_delta = m_new_delta;
	m_new_delta = ::GetTickCount();

	m_delta_time = (m_old_delta) ? ((m_new_delta - m_old_delta) / 1000.0f) : 0;
}

void AppWindow::onDestroy()
{
	Window::onDestroy();
	m_vb->release();
	m_ib->release();
	m_cb->release();
	m_swap_chain->release();
	m_vs->release();
	m_ps->release();
	GraphicsEngine::get()->release();
}

void AppWindow::onFocus()
{
	InputSystem::get()->addListener(this);
}

void AppWindow::onKillFocus()
{
	InputSystem::get()->removeListener(this);
}

void AppWindow::onKeyDown(int key)
{
	if (key == 'W')
	{
		// m_rot_x -= 3.14f * m_delta_time;
		m_forward = 1.0f; // moving skibidi forward
	}
	else if (key == 'S')
	{
		// m_rot_x += 3.14f * m_delta_time;
		m_forward = -1.0f; // moving skibidi not forward
	}
	else if (key == 'A')
	{
		// m_rot_y -= 3.14f * m_delta_time;
		m_rightward = -1.0f; // moving sigma not right
	}
	else if (key == 'D')
	{
		// m_rot_y += 3.14f * m_delta_time;
		m_rightward = 1.0f; // moving sigma right
	}
}

void AppWindow::onKeyUp(int key)
{
	m_forward = 0.0f;
	m_rightward = 0.0f;
}

void AppWindow::onMouseMove(const Point & mouse_pos)
{
	int width = (this->getClientWindowRect().right - this->getClientWindowRect().left);
	int height = (this->getClientWindowRect().bottom - this->getClientWindowRect().top);

	m_rot_x += (mouse_pos.m_y - (height / 2.0f)) * m_delta_time * 0.1f;
	m_rot_y += (mouse_pos.m_x - (width / 2.0f)) * m_delta_time * 0.1f;

	InputSystem::get()->setCursorPosition(Point(width / 2.0f, height / 2.0f));
}

void AppWindow::onLeftMouseDown(const Point& mouse_pos)
{
	m_scale_cube = 0.5f;
}

void AppWindow::onLeftMouseUp(const Point& mouse_pos)
{
	m_scale_cube = 1.0f;
}

void AppWindow::onRightMouseDown(const Point& mouse_pos)
{
	m_scale_cube = 2.0f;
}

void AppWindow::onRightMouseUp(const Point& mouse_pos)
{
	m_scale_cube = 1.0f;
}
