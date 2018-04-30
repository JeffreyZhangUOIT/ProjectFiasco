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
along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
*/
#pragma once

#include "Renderer.h"

class BoxFactory {
public:
	struct color {
		float r, g, b, a;
	};

	BoxFactory(Renderer& ren);
	~BoxFactory();
	void update(Renderer& ren, float* boxDimen, color setColor);
	void draw(Renderer& ren);
private:
	void createMesh(Renderer& ren);
	ID3D11Buffer* m_pVertexBuffer = nullptr;
	ID3D11Buffer* mIB = nullptr;
	int flag[];
};