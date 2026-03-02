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

	struct RHIBufferCreateInfo;
	struct RHITextureCreateInfo;
	struct RHISamplerCreateInfo;
	struct RHIVertexShaderCreateInfo;
	struct RHIGeometryShaderCreateInfo;
	struct RHIPixelShaderCreateInfo;
	struct RHIComputeShadercreateInfo;
	struct RHIPipeLineCreateInfo;

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

		RHICommandList(Memory::RefPtr<RHISystem> system);
		~RHICommandList();

		template<typename T, typename... Args>
		void Enqueue(Args&&... args)
		{
			RHICommandBase* command = Create<T, Args...>(std::forward<Args>(args)...);

			m_commands.PushBack(command);
		}

		void ExecuteAll();
		void Reset();
		void SetFrame(const size_t frame);
		const size_t GetFrame() const;

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

		Memory::RefPtr<RHIBuffer> CreateBuffer(const RHIBufferCreateInfo info);
		Memory::RefPtr<RHITexture> CreateTexture(const RHITextureCreateInfo info);
		Memory::RefPtr<RHISampler> CreateSampler(const RHISamplerCreateInfo info);
		Memory::RefPtr<RHIVertexShader> CreateVertexShader(const RHIVertexShaderCreateInfo info);
		Memory::RefPtr<RHIGeometryShader> CreateGeometryShader(const RHIGeometryShaderCreateInfo info);
		Memory::RefPtr<RHIPixelShader> CreatePixelShader(const RHIPixelShaderCreateInfo info);
		Memory::RefPtr<RHIComputeShader> CreateComputeShader(const RHIComputeShaderCreateInfo info);
		Memory::RefPtr<RHIPipeLine> CreatePipeLine(const RHIPipeLineCreateInfo info);

		void UpdateBuffer(const RHIBufferCreateInfo info, Memory::RefPtr<RHIBuffer> buffer);
		void UpdateTexture(const RHITextureCreateInfo info, Memory::RefPtr<RHITexture> texture);

		void RemoveBuffer(Memory::RefPtr<RHIBuffer> buffer);
		void RemoveTexture(Memory::RefPtr<RHITexture> texture);
		void RemoveSampler(Memory::RefPtr<RHISampler> sampler);
		void RemoveShader(Memory::RefPtr<RHIShader> shader);
		void RemovePipeLine(Memory::RefPtr<RHIPipeLine> shader);

		void DrawIndexPrimitive();

	private :
		Memory::RefPtr<RHISystem> m_system;
		wtr::DynamicArray<RHICommandBase*> m_commands;
		size_t m_frame;
	};
};

#endif // __WTR_RHI_COMMANDLIST_H__