#pragma once
#include <d3d11.h>

class DeviceContext;

class ConstantBuffer
{
public:
	ConstantBuffer();
	bool load(void* buffer, UINT size_buffer);
	void update(DeviceContext* context, void* buffer);
	bool release();
	~ConstantBuffer();
private:
	UINT m_size_vertex;
	UINT m_size_list;
private:
	ID3D11Buffer* m_buffer;
	ID3D11InputLayout* m_layout;
private:
	friend class DeviceContext;
};