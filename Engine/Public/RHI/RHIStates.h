#ifndef __WTR_RHISTATES_H__
#define __WTR_RHISTATES_H__

#include <Renderer/RenderType.h>
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

	struct RHIResourceState
	{
		eDataState state	= eDataState::eNone;
		eDataStage stage	= eDataStage::eNone;

		RHIResourceState() = default;
		RHIResourceState(const eDataState dataState, const eDataStage dataStage)
			: state(dataState)
			, stage(dataStage)
		{}

		bool IsEmpty() const noexcept
		{
			return state == eDataState::eNone;
		}

		bool HasWrite() const noexcept
		{
			return eDataState::eNone != (state & eDataState::eWrite);
		}

		// next가 요구하는 state / stage를 모두 포함하고 있는지
		bool Contains(const RHIResourceState& next) const noexcept
		{
			return (state & next.state) == next.state &&
				(stage & next.stage) == next.stage;
		}

		void Merge(const RHIResourceState& next) noexcept
		{
			state = state | next.state;
			stage = stage | next.stage;
		}

		bool operator==(const RHIResourceState& other) const noexcept
		{
			return state == other.state &&
				stage == other.stage;
		}

		bool operator!=(const RHIResourceState& other) const noexcept
		{
			return !(*this == other);
		}
	};

	struct RHIBufferState : RHIResourceState
	{
		eBufferUsage usage = eBufferUsage::eNone;

		RHIBufferState() = default;
		RHIBufferState(const eDataState dataState, const eDataStage dataStage, const eBufferUsage bufferUsage)
			: RHIResourceState(dataState, dataStage)
			, usage(bufferUsage)
		{}

		RHIBufferState(const RHIResourceState& state)
			: RHIResourceState(state)
		{}

		RHIBufferState& operator=(const RHIResourceState& state)
		{
			if (this != &state)
			{
				RHIResourceState::operator=(state);
			}

			return *this;
		}

		// 전환 없이 next로 바로 쓸 수 있는지
		// 쓰기가 한쪽이라도 끼면 무조건 전환, 읽기끼리는 현재가 next의 stage / usage를 다 포함할 때만 생략
		bool Covers(const RHIBufferState& next) const noexcept
		{
			if (HasWrite() || next.HasWrite())
			{
				return false;
			}

			return Contains(next) && (usage & next.usage) == next.usage;
		}

		// 같은 구간 안에서 한 버퍼에 요구가 겹치면 합친다. 버퍼는 항상 합칠 수 있음
		bool Merge(const RHIBufferState& next) noexcept
		{
			RHIResourceState::Merge(next);
			usage = usage | next.usage;

			return true;
		}

		// 업로드(Initialize / Update) 직후 상태 : 모든 읽기의 합 (UE RHIGetDefaultResourceState의 SRVMask와 같은 방식)
		static RHIBufferState Readable()
		{
			const eDataStage readStage = eDataStage::eVertexInput | eDataStage::eVertex | eDataStage::eHull
				| eDataStage::eDomain | eDataStage::eGeometry | eDataStage::ePixel | eDataStage::eCompute;
			const eBufferUsage readUsage = eBufferUsage::eVertex | eBufferUsage::eIndex
				| eBufferUsage::eUniform | eBufferUsage::eStorage | eBufferUsage::eIndirect;

			return RHIBufferState(eDataState::eRead, readStage, readUsage);
		}

		bool operator==(const RHIBufferState& other) const noexcept
		{
			return RHIResourceState::operator==(other) &&
				usage == other.usage;
		}

		bool operator!=(const RHIBufferState& other) const noexcept
		{
			return !(*this == other);
		}
	};

	struct RHITextureState : RHIResourceState
	{
		eTextureUsage usage = eTextureUsage::eNone;

		RHITextureState() = default;
		RHITextureState(const eDataState dataState, const eDataStage dataStage, const eTextureUsage textureUsage)
			: RHIResourceState(dataState, dataStage)
			, usage(textureUsage)
		{}

		RHITextureState(const RHIResourceState& state)
			: RHIResourceState(state)
		{
		}

		RHITextureState& operator=(const RHIResourceState& state)
		{
			if (this != &state)
			{
				RHIResourceState::operator=(state);
			}

			return *this;
		}

		// 텍스처는 usage가 곧 레이아웃이라, usage가 같아야만 전환을 생략할 수 있음
		bool Covers(const RHITextureState& next) const noexcept
		{
			if (HasWrite() || next.HasWrite())
			{
				return false;
			}

			return Contains(next) && usage == next.usage;
		}

		// usage가 다르면 한 구간에 레이아웃이 두 개 필요하다는 뜻이라 합칠 수 없음 (피드백 루프)
		bool Merge(const RHITextureState& next) noexcept
		{
			if (usage != eTextureUsage::eNone && usage != next.usage)
			{
				return false;
			}

			RHIResourceState::Merge(next);
			usage = next.usage;

			return true;
		}

		// 업로드(Initialize / Update) 직후 상태 : 모든 셰이더 단계에서 샘플링 가능
		static RHITextureState Readable()
		{
			const eDataStage readStage = eDataStage::eVertex | eDataStage::eHull | eDataStage::eDomain
				| eDataStage::eGeometry | eDataStage::ePixel | eDataStage::eCompute;

			return RHITextureState(eDataState::eRead, readStage, eTextureUsage::eSampler);
		}

		bool operator==(const RHITextureState& other) const noexcept
		{
			return RHIResourceState::operator==(other) &&
				usage == other.usage;
		}

		bool operator!=(const RHITextureState& other) const noexcept
		{
			return !(*this == other);
		}
	};
};

#endif // __WTR_RHISTATES_H__