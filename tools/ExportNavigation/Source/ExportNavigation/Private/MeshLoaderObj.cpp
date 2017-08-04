//
// Copyright (c) 2009-2010 Mikko Mononen memon@inside.org
//
// This software is provided 'as-is', without any express or implied
// warranty.  In no event will the authors be held liable for any damages
// arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.
//
#include "ExportNavigationPrivatePCH.h"
#include "MeshLoaderObj.h"
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#define _USE_MATH_DEFINES
#include <math.h>
namespace original
{
	rcMeshLoaderObj::rcMeshLoaderObj() :
		m_scale(1.0f),
		m_verts(0),
		m_tris(0),
		m_normals(0),
		m_vertCount(0),
		m_triCount(0)
	{
	}

	rcMeshLoaderObj::~rcMeshLoaderObj()
	{
		delete[] m_verts;
		delete[] m_normals;
		delete[] m_tris;
	}

	void rcMeshLoaderObj::addVertex(float x, float y, float z, int& cap)
	{
		if (m_vertCount + 1 > cap)
		{
			cap = !cap ? 8 : cap * 2;
			float* nv = new float[cap * 3];
			if (m_vertCount)
				memcpy(nv, m_verts, m_vertCount * 3 * sizeof(float));
			delete[] m_verts;
			m_verts = nv;
		}
		float* dst = &m_verts[m_vertCount * 3];
		*dst++ = x*m_scale;
		*dst++ = y*m_scale;
		*dst++ = z*m_scale;
		m_vertCount++;
	}

	void rcMeshLoaderObj::addTriangle(int a, int b, int c, int& cap)
	{
		if (m_triCount + 1 > cap)
		{
			cap = !cap ? 8 : cap * 2;
			int* nv = new int[cap * 3];
			if (m_triCount)
				memcpy(nv, m_tris, m_triCount * 3 * sizeof(int));
			delete[] m_tris;
			m_tris = nv;
		}
		int* dst = &m_tris[m_triCount * 3];
		*dst++ = a;
		*dst++ = b;
		*dst++ = c;
		m_triCount++;
	}



	static int parseFace(char* row, int* data, int n, int vcnt)
	{
		int j = 0;
		while (*row != '\0')
		{
			// Skip initial white space
			while (*row != '\0' && (*row == ' ' || *row == '\t'))
				row++;
			char* s = row;
			// Find vertex delimiter and terminated the string there for conversion.
			while (*row != '\0' && *row != ' ' && *row != '\t')
			{
				if (*row == '/') *row = '\0';
				row++;
			}
			if (*s == '\0')
				continue;
			int vi = atoi(s);
			data[j++] = vi < 0 ? vi + vcnt : vi - 1;
			if (j >= n) return j;
		}
		return j;
	}

	bool rcMeshLoaderObj::Init(const TNavStatArray<float>& CoordBuffer, const TNavStatArray<int32>& IndexBuffer)
	{
		int vcap = 0;
		int tcap = 0;
		for (int32 Index = 0; Index < CoordBuffer.Num(); Index += 3)
		{
			addVertex(CoordBuffer[Index + 0], CoordBuffer[Index + 1], CoordBuffer[Index + 2], vcap);
		}

		for (int32 Index = 0; Index < IndexBuffer.Num(); Index += 3)
		{
			const int a = IndexBuffer[Index + 0];
			const int b = IndexBuffer[Index + 1];
			const int c = IndexBuffer[Index + 2];
			if (a < 0 || a >= m_vertCount || b < 0 || b >= m_vertCount || c < 0 || c >= m_vertCount)
				continue;
			addTriangle(a, b, c, tcap);
		}
		return true;
	}
}