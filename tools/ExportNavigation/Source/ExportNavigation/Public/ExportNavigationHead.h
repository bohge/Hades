// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once
static const int NAVMESHSET_MAGIC = 'M' << 24 | 'S' << 16 | 'E' << 8 | 'T'; //'MSET';
static const int NAVMESHSET_VERSION = 1;
#include "DetourNavMesh.h"

struct NavMeshSetHeader
{
	int magic;
	int version;
	int numTiles;
	original::dtNavMeshParams params;
};

struct NavMeshTileHeader
{
	//dtTileRef tileRef;
	unsigned int tileRef;
	int dataSize;
};

enum SamplePolyAreas
{
	HADES_POLYAREA_GROUND,
	HADES_POLYAREA_WATER,
	HADES_POLYAREA_ROAD,
	HADES_POLYAREA_DOOR,
	HADES_POLYAREA_GRASS,
	HADES_POLYAREA_JUMP,
};
enum SamplePolyFlags
{
	HADES_POLYFLAGS_WALK = 0x01,		// Ability to walk (ground, grass, road)
	HADES_POLYFLAGS_SWIM = 0x02,		// Ability to swim (water).
	HADES_POLYFLAGS_DOOR = 0x04,		// Ability to move through doors.
	HADES_POLYFLAGS_JUMP = 0x08,		// Ability to jump.
	HADES_POLYFLAGS_DISABLED = 0x10,		// Disabled polygon
	HADES_POLYFLAGS_ALL = 0xffff	// All abilities.
};