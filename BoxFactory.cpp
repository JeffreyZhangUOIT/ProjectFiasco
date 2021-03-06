/*
Copyright 2017, 2018 Jeffrey Zhang

This file is part of ProjectFiasco.

ProjectFiasco is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

ProjectFiasco is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with ProjectFiasco.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "BoxFactory.h"

BoxFactory::BoxFactory(Renderer& ren) {
	createMesh(ren);
}

BoxFactory::~BoxFactory() {
	//delete mIB;
	//delete m_pVertexBuffer;
}

void BoxFactory::update(Renderer& ren, float* boxDimen, int n, float mx, float my) {
	boxColor = n;

	Renderer::Vertex vertices[4] = {
		{ boxDimen[2], boxDimen[3], 0, 0, 0 },
		{ boxDimen[0], boxDimen[3], 0, 1, 0 },
		{ boxDimen[2], boxDimen[1], 0, 0, 1 },
		{ boxDimen[0], boxDimen[1], 0, 1, 1 },
	};

	if (boxColor == 15)
	{
		if ((mx > boxDimen[0]) & (my > boxDimen[1]) & (mx < boxDimen[2]) & (my < boxDimen[3])) 
		{
			vertices[0].Ytex = 0.5f;
			vertices[1].Ytex = 0.5f;
		}
		else
		{
			vertices[2].Ytex = 0.5f;
			vertices[3].Ytex = 0.5f;
		}
	}

	D3D11_MAPPED_SUBRESOURCE resource;
	ren.getDeviceContext()->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);

	memcpy(resource.pData, vertices, sizeof(vertices));
	ren.getDeviceContext()->Unmap(m_pVertexBuffer, 0);

	DWORD indices[6] =
	{
		2, 1, 0,
		1, 2, 3
	};

	ren.getDeviceContext()->Map(mIB, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, indices, sizeof(indices));
	ren.getDeviceContext()->Unmap(mIB, 0);
}

void BoxFactory::draw(Renderer& ren, TextureManager& tex) {
	auto deviceContext = ren.getDeviceContext();

	tex.changeTex(ren, boxColor);
	// Bind our Player shaders
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Bind our vertex buffer
	UINT stride = sizeof(Renderer::Vertex);
	UINT offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);

	// Draw
	deviceContext->DrawIndexed(6, 0, 0);
}


void BoxFactory::createMesh(Renderer& ren) {

	// Create our vertext buffer
	Renderer::Vertex vertices[] = {
	{ -0.90f, -0.90f, 0, 0, 0},
	{ 0.90f, -0.90f, 0, 1, 0 },
	{ 0.90f, -0.50f, 0, 0, 1 },
	{ -0.90f, -0.50f, 0, 1, 1 }
	};
	auto vertexBufferDesc = CD3D11_BUFFER_DESC(sizeof(vertices), D3D11_BIND_VERTEX_BUFFER);
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	D3D11_SUBRESOURCE_DATA vertexData = { 0 };
	vertexData.pSysMem = vertices;

	ren.getDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &m_pVertexBuffer);

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