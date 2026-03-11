#ifndef __WTR_RHISTATES_H__
#define __WTR_RHISTATES_H__

#include <Renderer/RenderTypes.h>
#include <Framework/Math/MathTypes.h>

namespace wtr
{
	struct RHIColorState
	{
		bool red 				= true;
		bool green				= true;
		bool blue				= true;
		bool alpha				= true;
	};

	struct RHIDepthState
	{
		bool enable				= false;
		bool write				= false;
		eCompareFunc func		= eCompareFunc::eAlways;
	};

	struct RHIStencilState
	{
		bool enable				= false;
		uint8_t refMask			= 0x00;
		uint8_t readMask		= 0xFF;
		uint8_t writeMask		= 0xFF;
		eCompareFunc func		= eCompareFunc::eAlways;
		eStencilOp failOp		= eStencilOp::eKeep;
		eStencilOp depthFailOp	= eStencilOp::eKeep;
		eStencilOp passOp		= eStencilOp::eKeep;
	};

	struct RHIBlendState
	{
		bool enable				= false;
		eBlendFunc srcColor		= eBlendFunc::eOne;
		eBlendFunc srcAlpha		= eBlendFunc::eOne;
		eBlendFunc destColor	= eBlendFunc::eZero;
		eBlendFunc destAlpha	= eBlendFunc::eZero;
		eBlendOp colorOp		= eBlendOp::eAdd;
		eBlendOp alphaOp		= eBlendOp::eAdd;
		uint8_t	writeMask		= 0x0F;
	};

	struct RHIRasterizerState
	{
		bool			cullEnable	= true;
		eCullFace		cullFace	= eCullFace::eBack;
		eFrontFace		frontFace 	= eFrontFace::eCCW;
		ePrimitiveMode	fillMode	= ePrimitiveMode::eFill;
	};

	struct RHIClearState
	{
		eClearBuffer	clearBuffer	= eClearBuffer::eNone;

		fvec4 color = {0.f, 0.f, 0.f, 1.f};
		float depth = 1.0f;
		uint32_t stencil = 0;
	};
};

#endif // __WTR_RHISTATES_H__