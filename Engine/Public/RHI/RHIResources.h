#ifndef __WTR_RHIRESOURCES_H__
#define __WTR_RHIRESOURCES_H__

#include <Memory/include/Pointer/RefPtr.h>

namespace wtr
{
	class RHIResource : public Memory::RefCounted
	{
		public :
			RHIResource() = default;
			virtual ~RHIResource() = default;
	};

	class RHIBuffer : public RHIResource 
	{
		public :
			RHIBuffer() = default;
			virtual ~RHIBuffer() = default;
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
		public :
			RHIState() = default;
			virtual ~RHIState() = default;
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