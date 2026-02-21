#ifndef __WTR_RHI_COMMANDLIST_H__
#define __WTR_RHI_COMMANDLIST_H__

#include <Framework/CommandList.h>
#include <RHI/RHICommands.h>

#include <Memory/include/Pointer/RefPtr.h>
#include <Container/include/DynamicArray.h>
#include <Container/include/HashMap.h>

namespace wtr
{
	class RHISystem;
	class RHIBuffer;
	class RHITexture;
	class RHISampler;
	class RHIShader;
	class RHIVertexShader;
	class RHIGeometryShader;
	class RHIPixelShader;
	class RHIComputeShader;
	class RHIPipeLine;

	struct RHIBufferDesc;
	struct RHITextureDesc;
	struct RHISamplerDesc;
	struct RHIVertexShaderDesc;
	struct RHIGeometryShaderDesc;
	struct RHIPixelShaderDesc;
	struct RHIComputeShaderDesc;
	struct RHIPipeLineDesc;

	struct RHIColorState;
	struct RHIDepthState;
	struct RHIStencilState;
	struct RHIBlendState;
	struct RHIRasterizerState;
	struct RHIClearState;
};

namespace wtr
{
	class RHICommandList : public CommandList<RHICommandBase>
	{
	public :
		using Super = CommandList<RHICommandBase>;

		RHICommandList();
		~RHICommandList();

		template<typename T, typename... Args>
		void Enqueue(Args&&... args)
		{
			RHICommandBase* command = Create<T, Args...>(std::forward<Args>(args)...);

			m_commands.PushBack(command);
		}

		void ExecuteAll(Memory::RefPtr<RHISystem> system);
		void Reset();

	public :
		void Clear(const RHIClearState& state);
		void Flush();
		void Present();
		void Resize(const uint32_t posX, const uint32_t posY, const uint32_t width, const uint32_t height);

		void SetColorState(const RHIColorState& state);
		void SetDepthState(const RHIDepthState& state);
		void SetStencilState(const RHIStencilState& state);
		void SetBlendState(const RHIBlendState& state);
		void SetRasterizerState(const RHIRasterizerState& state);

		Memory::RefPtr<RHIBuffer> CreateBuffer(const RHIBufferDesc& desc);
		Memory::RefPtr<RHITexture> CreateTexture(const RHITextureDesc& desc);
		Memory::RefPtr<RHISampler> CreateSampler(const RHISamplerDesc& desc);
		Memory::RefPtr<RHIVertexShader> CreateVertexShader(const RHIVertexShaderDesc& desc);
		Memory::RefPtr<RHIGeometryShader> CreateGeometryShader(const RHIGeometryShaderDesc& desc);
		Memory::RefPtr<RHIPixelShader> CreatePixelShader(const RHIPixelShaderDesc& desc);
		Memory::RefPtr<RHIComputeShader> CreateComputeShader(const RHIComputeShaderDesc& desc);
		Memory::RefPtr<RHIPipeLine> CreatePipeLine(const RHIPipeLineDesc& desc);

		void UpdateBuffer(const RHIBufferDesc& desc, Memory::RefPtr<RHIBuffer> buffer);
		void UpdateTexture(const RHITextureDesc& desc, Memory::RefPtr<RHITexture> texture);

		void RemoveBuffer(Memory::RefPtr<RHIBuffer> buffer);
		void RemoveTexture(Memory::RefPtr<RHITexture> texture);
		void RemoveSampler(Memory::RefPtr<RHISampler> sampler);
		void RemoveShader(Memory::RefPtr<RHIShader> shader);
		void RemovePipeLine(Memory::RefPtr<RHIPipeLine> shader);

		void DrawIndexPrimitive();

	private :
		wtr::DynamicArray<RHICommandBase*> m_commands;
	};
};

#endif // __WTR_RHI_COMMANDLIST_H__