#pragma warning ( disable : 4838 )

#ifndef __DXCOMMON_H__
#define __DXCOMMON_H__

#include <dinput.h>
#include <dxgi.h>
#include <d3dcommon.h>
#include <d3dcompiler.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include <d3dx10math.h>
#include <xnamath.h>

#include "SUBLog.h"

typedef enum RASTERZIERSTATE
{
	CCWCUL, CWCUL, NOCUL, WIREFRAME
}RASTERIZER;

typedef enum BLENERSTATE
{
	NONE, ACTIVEALPHA, DEACTIVEALPHA
}BLEND;

typedef enum DEPTHSTENCILSTATE
{
	DEPTHACTIVE, DEPTHDEACTIVE
}DEPTHSTENCIL;

struct RENDERSTATE
{
	DEPTHSTENCIL DepthStencilState;
	RASTERIZER RasterizerState;
	BLEND BlendState;
};

#endif