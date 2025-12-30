#ifndef __WTR_RHIRESOURCES_H__
#define __WTR_RHIRESOURCES_H__

#include <Memory/include/Pointer/RefPtr.h>

namespace wtr
{
	class RHIResource : public Memory::RefCounted
	{

	};

	class RHIBuffer : public RHIResource 
	{
	};

	class RHITexture : public RHIResource
	{
	};

	class RHISampler : public RHIResource
	{

	};

	class RHIShader : public RHIResource
	{
	};

	class RHIState
	{
	};

	class RHIDepthStencilState : public RHIState
	{
	};

	class RHIBlendState : public RHIState
	{
	};

	class RHIRasterizerState : public RHIState
	{
	};

	class RHIPipeLine : public RHIResource
	{
	};

	class RHIGraphicPipeLineState : public RHIPipeLine
	{
	};

	class RHIComputePipeLineState : public RHIPipeLine
	{
	};
};

#endif // __WTR_RHIRESOURCES_H__