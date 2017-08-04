#pragma once
#include "Engine/EngineTypes.h"

struct BRecastGeometryCache
{
	struct FHeader
	{
		int32 NumVerts;
		int32 NumFaces;
		struct FWalkableSlopeOverride SlopeOverride;
	};

	FHeader Header;

	/** recast coords of vertices (size: NumVerts * 3) */
	float* Verts;

	/** vert indices for triangles (size: NumFaces * 3) */
	int32* Indices;

	BRecastGeometryCache() {}
	BRecastGeometryCache(const uint8* Memory);
};
