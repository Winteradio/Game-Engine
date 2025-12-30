#ifndef __WTR_RHIDEVICE_H__
#define __WTR_RHIDEVICE_H__

#include <Memory/include/Core.h>

namespace wtr
{
	enum class eRHIType : uint8_t;

	struct RHIBufferDesc;
	struct RHITextureDesc;
	struct RHIShaderDesc;
	struct RHIStateDesc;
	struct RHIPipeLineDesc;

	class RHIContext;
	class RHIBuffer;
	class RHITexture;
	class RHIShader;
	class RHIState;
	class RHIPipeLine;
};

namespace wtr
{
	class RHIDevice
	{
		public :
			RHIDevice() = default;
			virtual ~RHIDevice() = default;

		public :
			virtual void Init() = 0;
			virtual void Shutdown() = 0;

			virtual Memory::RefPtr<RHIContext> CreateContext(const eRHIType eType) = 0;
			virtual Memory::RefPtr<RHIBuffer> CreateBuffer(const RHIBufferDesc& bufferDesc) = 0;
			virtual Memory::RefPtr<RHITexture> CreateTexture(const RHITextureDesc& textureDesc) = 0;
			virtual Memory::RefPtr<RHIShader> CreateShader(const RHIShaderDesc& shaderDesc) = 0;
			virtual Memory::RefPtr<RHIState> CreateState(const RHIStateDesc& stateDesc) = 0;
			virtual Memory::RefPtr<RHIPipeLine> CreatePipeLine(const RHIPipeLineDesc& pipeLineDesc) = 0;

			virtual void DestroyBuffer(Memory::RefPtr<RHIBuffer>& refBuffer) = 0;
			virtual void DestroyTexture(Memory::RefPtr<RHITexture>& refTexture) = 0;
			virtual void DestroyShader(Memory::RefPtr<RHIShader>& refShader) = 0;
			virtual void DestroyState(Memory::RefPtr<RHIState>& refState) = 0;
			virtual void DestroyPipeLine(Memory::RefPtr<RHIPipeLine>& refPipeLine) = 0;
	};
};

#endif // __WTR_RHIDEVICE_H__