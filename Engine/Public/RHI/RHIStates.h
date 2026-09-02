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

		bool operator==(const RHIColorState& other) const noexcept
		{
			return red == other.red &&
				green == other.green &&
				blue == other.blue &&
				alpha == other.alpha;
		}

		bool operator!=(const RHIColorState& other) const noexcept
		{
			return !(*this == other);
		}
	};

	struct RHIDepthState
	{
		bool enable				= false;
		bool write				= false;
		eCompareFunc func		= eCompareFunc::eAlways;

		bool operator==(const RHIDepthState& other) const noexcept
		{
			return enable == other.enable &&
				write == other.write &&
				func == other.func;
		}

		bool operator!=(const RHIDepthState& other) const noexcept
		{
			return !(*this == other);
		}
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

		bool operator==(const RHIStencilState& other) const noexcept
		{
			return enable == other.enable &&
				refMask == other.refMask &&
				readMask == other.readMask &&
				writeMask == other.writeMask &&
				func == other.func &&
				failOp == other.failOp &&
				depthFailOp == other.depthFailOp &&
				passOp == other.passOp;
		}

		bool operator!=(const RHIStencilState& other) const noexcept
		{
			return !(*this == other);
		}
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

		bool operator==(const RHIBlendState& other) const noexcept
		{
			return enable == other.enable &&
				srcColor == other.srcColor &&
				srcAlpha == other.srcAlpha &&
				destColor == other.destColor &&
				destAlpha == other.destAlpha &&
				colorOp == other.colorOp &&
				alphaOp == other.alphaOp &&
				writeMask == other.writeMask;
		}

		bool operator!=(const RHIBlendState& other) const noexcept
		{
			return !(*this == other);
		}
	};

	struct RHIRasterizerState
	{
		bool			cullEnable	= true;
		eCullFace		cullFace	= eCullFace::eBack;
		eFrontFace		frontFace 	= eFrontFace::eCCW;
		ePrimitiveMode	fillMode	= ePrimitiveMode::eFill;

		bool operator==(const RHIRasterizerState& other) const noexcept
		{
			return cullEnable == other.cullEnable &&
				cullFace == other.cullFace &&
				frontFace == other.frontFace &&
				fillMode == other.fillMode;
		}

		bool operator!=(const RHIRasterizerState& other) const noexcept
		{
			return !(*this == other);
		}
	};

	struct RHIClearState
	{
		eClearBuffer	clearBuffer	= eClearBuffer::eNone;

		fvec4 color = {0.f, 0.f, 0.f, 1.f};
		float depth = 1.0f;
		uint32_t stencil = 0;

		bool operator==(const RHIClearState& other) const noexcept
		{
			return clearBuffer == other.clearBuffer &&
				color == other.color &&
				depth == other.depth &&
				stencil == other.stencil;
		}

		bool operator!=(const RHIClearState& other) const noexcept
		{
			return !(*this == other);
		}
	};
};

#endif // __WTR_RHISTATES_H__