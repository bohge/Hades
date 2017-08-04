#pragma once

//不适用自动全局static组要原因是static初始化顺序不能控制会引起两个问题
#define FACTOR_VECTOR2F			0
#define FACTOR_VECTOR3F			1
#define FACTOR_VECTOR4F			2
#define FACTOR_MATRIX22F		3
#define FACTOR_MATRIX33F		4
#define FACTOR_MATRIX44F		5
#define FACTOR_QUATERNIONF		6
#define FACTOR_RAYF				7
#define FACTOR_LINEF			8
#define FACTOR_PLANEF			9
#define FACTOR_FRUSTUMF			10
#define FACTOR_AABBOX3DF		11
#define FACTOR_AABRECTF			12
#define FACTOR_AABROUNDF		13
#define FACTOR_AABSPHEREF		14

#define FACTOR_NAVMESH			15
#define FACTOR_BEHAVIOR			16

#define FACTOR_AGENT			17
#define FACTOR_MESSAGE			18

#define FACTOR_RESUMER			19

#define FACTOR_SERVER			20
#define FACTOR_SOCKET			21