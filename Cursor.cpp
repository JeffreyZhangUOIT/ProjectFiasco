#include "Cursor.h"

struct Vertex {
	float x, y, z;
	float r, g, b, a;
};

Mouse::Mouse(Renderer& ren) : GameObject() {
	createMesh(ren);
	x = 0;
	y = 0;
}

Mouse::~Mouse(){
	m_pVertexBuffer->Release();
	mIB->Release();
}

void Mouse::update(Renderer& renderer, float* aim) {
	
	Vertex vertices[] = {
	{ aim[0]-0.01f, aim[1]-0.01f, 0.001f, 0, 0, 1, 1 },
	{ aim[0]+0.01f, aim[1]-0.01f, 0.001f, 0, 0, 1, 1 },
	{ aim[0]-0.01f, aim[1]+0.01f, 0.001f, 0, 0, 1, 1 },
	{ aim[0]+0.01f, aim[1]+0.01f, 0.001f, 0, 0, 1, 1 }
	};

	DWORD indices[6] =
	{
		0, 1, 2,
		1, 2, 3
	};

	D3D11_MAPPED_SUBRESOURCE resource;
	renderer.getDeviceContext()->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, vertices, sizeof(vertices));
	renderer.getDeviceContext()->Unmap(m_pVertexBuffer, 0);

	renderer.getDeviceContext()->Map(mIB, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, indices, sizeof(indices));
	renderer.getDeviceContext()->Unmap(mIB, 0);

}

void Mouse::createMesh(Renderer& ren) {
	// Create our vertext buffer
	Vertex vertices[] = {
	{ -0.01f, -0.01f, 0.001f, 1, 1, 1, 1 },
	{ +0.01f, -0.01f, 0.001f, 1, 1, 1, 1 },
	{ -0.01f, 0.01f, 0.001f, 1, 1, 1, 1 },
	{ 0.01f, 0.01f, 0.001f, 1, 1, 1, 1 }
	};

	auto vertexBufferDesc = CD3D11_BUFFER_DESC(sizeof(Vertex)*4, D3D11_BIND_VERTEX_BUFFER);
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	D3D11_SUBRESOURCE_DATA vertexData = { 0 };
	vertexData.pSysMem = vertices;
	ren.getDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &m_pVertexBuffer);

	// Create our indicies buffer becareful of back-face culling.
	DWORD indices[6] =
	{
		2, 1, 0,
		1, 2, 3
	};

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_DYNAMIC;
	ibd.ByteWidth = sizeof(DWORD) * 6;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	// Specify the data to initialize the index buffer.
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = indices;

	// Create the index buffer.
	ren.getDevice()->CreateBuffer(&ibd, &iinitData, &mIB);
	ren.getDeviceContext()->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);

}

void Mouse::draw(Renderer& ren) {
	auto deviceContext = ren.getDeviceContext();

	// Bind our Player shaders
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Bind our vertex buffer
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	//deviceContext->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);

	// Draw
	deviceContext->DrawIndexed(6, 0, 0);
}