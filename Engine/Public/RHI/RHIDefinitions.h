#ifndef __WTR_RHIDEFINITIONS_H__
#define __WTR_RHIDEFINITIONS_H__

#include <Renderer/RenderTypes.h>
#include <cstdint>

namespace wtr
{
	struct RHIBufferDesc
	{
		static constexpr eResourceType type = eResourceType::eBuffer;

		eBufferType bufferType;
		eDataAccess	accessType;
	};

	struct RHITextureDesc
	{
		static constexpr eResourceType type = eResourceType::eTexture;
	};

	struct RHIShaderDesc
	{
		static constexpr eResourceType type = eResourceType::eShader;
	};

	struct RHIStateDesc
	{
		static constexpr eResourceType type = eResourceType::eState;
	};

	struct RHIPipeLineDesc
	{
		static constexpr eResourceType type = eResourceType::ePipeLine;
	};

	struct RHIViewDesc
	{
		static constexpr eResourceType type = eResourceType::eView;
	};
};

#endif // __WTR_RHIDEFINITIONS_H__