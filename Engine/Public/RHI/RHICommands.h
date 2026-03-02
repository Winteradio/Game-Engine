#ifndef __WTR_RHICOMMANDS_H__
#define __WTR_RHICOMMANDS_H__

#include <Reflection/include/Utils.h>
#include <Memory/include/Pointer/RefPtr.h>

#include <RHI/RHIDescriptions.h>

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

	struct RHIColorState;
	struct RHIDepthState;
	struct RHIStencilState;
	struct RHIBlendState;
	struct RHIRasterizerState;
	struct RHIClearState;
};

namespace wtr
{
	class RHICommandBase
	{
	public :
		using ExecuteFunc = void(*)(Memory::RefPtr<RHISystem> system, RHICommandBase*);

		RHICommandBase(const ExecuteFunc func);
		virtual ~RHICommandBase() {}

	public :
		void Execute(Memory::RefPtr<RHISystem> system);

	private :
		ExecuteFunc m_func = nullptr;
	};

	template<typename T>
	class RHICommand : public RHICommandBase
	{
	public :
		RHICommand()
			: RHICommandBase(&ExecuteAndDestruct)
		{}

		virtual ~RHICommand() = default;

	public :
		static void ExecuteAndDestruct(Memory::RefPtr<RHISystem> system, RHICommandBase* commandBase)
		{
			static_assert(Reflection::Utils::IsBase<RHICommandBase, T>::value && "The RHI Command's type is invalid");

			if (nullptr != commandBase)
			{
				T* command = static_cast<T*>(commandBase);
				command->Execute(system);
				command->~T();
			}
		}
	};

	class RHICommandClear : public RHICommand<RHICommandClear>
	{
	public :
		RHICommandClear(const RHIClearState& state);
		~RHICommandClear() = default;

	public :
		void Execute(Memory::RefPtr<RHISystem> system);

	private :
		const RHIClearState m_state;
	};

	class RHICommandFlush : public RHICommand<RHICommandFlush>
	{
	public :
		RHICommandFlush() = default;
		~RHICommandFlush() = default;

	public :
		void Execute(Memory::RefPtr<RHISystem> system);
	};

	class RHICommandPresent : public RHICommand<RHICommandPresent>
	{
	public :
		RHICommandPresent() = default;
		~RHICommandPresent() = default;

	public :
		void Execute(Memory::RefPtr<RHISystem> system);
	};

	class RHICommandResize : public RHICommand<RHICommandResize>
	{
	public :
		RHICommandResize(const uint32_t width, const uint32_t height);
		RHICommandResize(const uint32_t posX, const uint32_t posY, const uint32_t width, const uint32_t height);
		~RHICommandResize() = default;

	public :
		void Execute(Memory::RefPtr<RHISystem> system);

	private :
		const uint32_t m_width;
		const uint32_t m_height;
		const uint32_t m_posX;
		const uint32_t m_posY;
	};

	class RHICommandColorState : public RHICommand<RHICommandColorState>
	{
	public :
		RHICommandColorState(const RHIColorState& state);
		~RHICommandColorState() = default;

	public :
		void Execute(Memory::RefPtr<RHISystem> system);

	private :
		const RHIColorState m_state;
	};

	class RHICommandDepthState : public RHICommand<RHICommandDepthState>
	{
	public :
		RHICommandDepthState(const RHIDepthState& state);
		~RHICommandDepthState() = default;

	public :
		void Execute(Memory::RefPtr<RHISystem> system);

	private :
		const RHIDepthState m_state;
	};

	class RHICommandStencilState : public RHICommand<RHICommandStencilState>
	{
	public :
		RHICommandStencilState(const RHIStencilState& state);
		~RHICommandStencilState() = default;

	public :
		void Execute(Memory::RefPtr<RHISystem> system);

	private :
		const RHIStencilState m_state;
	};

	class RHICommandBlendState : public RHICommand<RHICommandBlendState>
	{
	public :
		RHICommandBlendState(const RHIBlendState& state);
		~RHICommandBlendState() = default;

	public :
		void Execute(Memory::RefPtr<RHISystem> system);

	private :
		const RHIBlendState m_state;
	};

	class RHICommandRasterizerState : public RHICommand<RHICommandRasterizerState>
	{
	public :
		RHICommandRasterizerState(const RHIRasterizerState& state);
		~RHICommandRasterizerState() = default;

	public :
		void Execute(Memory::RefPtr<RHISystem> system);

	private :
		const RHIRasterizerState m_state;
	};

	class RHICommandInitializeBuffer : public RHICommand<RHICommandInitializeBuffer>
	{
	public :
		RHICommandInitializeBuffer(const RHIBufferCreateInfo info, const Memory::RefPtr<RHIBuffer> buffer);
		~RHICommandInitializeBuffer() = default;

	public :
		void Execute(Memory::RefPtr<RHISystem> system);

	private :
		const RHIBufferCreateInfo m_info;
		const Memory::RefPtr<RHIBuffer> m_buffer;
	};

	class RHICommandInitializeTexture : public RHICommand<RHICommandInitializeTexture>
	{
	public :
		RHICommandInitializeTexture(const RHITextureCreateInfo info, const Memory::RefPtr<RHITexture> texture);
		~RHICommandInitializeTexture() = default;

	public :
		void Execute(Memory::RefPtr<RHISystem> system);

	private :
		const RHITextureCreateInfo m_info;
		const Memory::RefPtr<RHITexture> m_texture;
	};

	class RHICommandInitializeSampler : public RHICommand<RHICommandInitializeSampler>
	{
	public :
		RHICommandInitializeSampler(const RHISamplerCreateInfo info, const Memory::RefPtr<RHISampler> sampler);
		~RHICommandInitializeSampler() = default;

	public :
		void Execute(Memory::RefPtr<RHISystem> system);

	private :
		const RHISamplerCreateInfo m_info;
		const Memory::RefPtr<RHISampler> m_sampler;
	};

	class RHICommandInitializeVertexShader : public RHICommand<RHICommandInitializeVertexShader>
	{
	public :
		RHICommandInitializeVertexShader(const RHIVertexShaderCreateInfo info, const Memory::RefPtr<RHIVertexShader> shader);
		~RHICommandInitializeVertexShader() = default;

	public :
		void Execute(Memory::RefPtr<RHISystem> system);

	private :
		const RHIVertexShaderCreateInfo m_info;
		const Memory::RefPtr<RHIVertexShader> m_shader;
	};

	class RHICommandInitializeGeometryShader : public RHICommand<RHICommandInitializeGeometryShader>
	{
	public :
		RHICommandInitializeGeometryShader(const RHIGeometryShaderCreateInfo info, const Memory::RefPtr<RHIGeometryShader> shader);
		~RHICommandInitializeGeometryShader() = default;

	public :
		void Execute(Memory::RefPtr<RHISystem> system);

	private :
		const RHIGeometryShaderCreateInfo m_info;
		const Memory::RefPtr<RHIGeometryShader> m_shader;
	};

	class RHICommandInitializePixelShader : public RHICommand<RHICommandInitializePixelShader>
	{
	public :
		RHICommandInitializePixelShader(const RHIPixelShaderCreateInfo info, const Memory::RefPtr<RHIPixelShader> shader);
		~RHICommandInitializePixelShader() = default;

	public :
		void Execute(Memory::RefPtr<RHISystem> system);

	private :
		const RHIPixelShaderCreateInfo m_info;
		const Memory::RefPtr<RHIPixelShader> m_shader;
	};

	class RHICommandInitializeComputeShader : public RHICommand<RHICommandInitializeComputeShader>
	{
	public :
		RHICommandInitializeComputeShader(const RHIComputeShaderCreateInfo info, const Memory::RefPtr<RHIComputeShader> shader);
		~RHICommandInitializeComputeShader() = default;

	public :
		void Execute(Memory::RefPtr<RHISystem> system);

	private :
		const RHIComputeShaderCreateInfo m_info;
		const Memory::RefPtr<RHIComputeShader> m_shader;
	};

	class RHICommandInitializePipeLine : public RHICommand<RHICommandInitializePipeLine>
	{
	public :
		RHICommandInitializePipeLine(const RHIPipeLineCreateInfo info, const Memory::RefPtr<RHIPipeLine> shader);
		~RHICommandInitializePipeLine() = default;

	public :
		void Execute(Memory::RefPtr<RHISystem> system);

	private :
		const RHIPipeLineCreateInfo m_info;
		const Memory::RefPtr<RHIPipeLine> m_shader;
	};

	class RHICommandUpdateBuffer : public RHICommand<RHICommandUpdateBuffer>
	{
	public :
		RHICommandUpdateBuffer(const RHIBufferCreateInfo info, const Memory::RefPtr<RHIBuffer> buffer);
		~RHICommandUpdateBuffer() = default;

	public :
		void Execute(Memory::RefPtr<RHISystem> system);

	private :
		const RHIBufferCreateInfo m_info;
		const Memory::RefPtr<RHIBuffer> m_buffer;
	};

	class RHICommandUpdateTexture : public RHICommand<RHICommandUpdateTexture>
	{
	public :
		RHICommandUpdateTexture(const RHITextureCreateInfo info, const Memory::RefPtr<RHITexture> texture);
		~RHICommandUpdateTexture() = default;

	public :
		void Execute(Memory::RefPtr<RHISystem> system);

	private :
		const RHITextureCreateInfo m_info;
		const Memory::RefPtr<RHITexture> m_texture;
	};

	class RHICommandRemoveBuffer : public RHICommand<RHICommandRemoveBuffer>
	{
	public :
		RHICommandRemoveBuffer(const Memory::RefPtr<RHIBuffer> buffer);
		~RHICommandRemoveBuffer() = default;

	public :
		void Execute(Memory::RefPtr<RHISystem> system);

	private :
		const Memory::RefPtr<RHIBuffer> m_buffer;
	};

	class RHICommandRemoveTexture : public RHICommand<RHICommandRemoveTexture>
	{
	public :
		RHICommandRemoveTexture(const Memory::RefPtr<RHITexture> texture);
		~RHICommandRemoveTexture() = default;

	public :
		void Execute(Memory::RefPtr<RHISystem> system);

	private :
		const Memory::RefPtr<RHITexture> m_texture;
	};

	class RHICommandRemoveSampler : public RHICommand<RHICommandRemoveSampler>
	{
	public :
		RHICommandRemoveSampler(const Memory::RefPtr<RHISampler> sampler);
		~RHICommandRemoveSampler() = default;

	public :
		void Execute(Memory::RefPtr<RHISystem> system);

	private :
		const Memory::RefPtr<RHISampler> m_sampler;
	};

	class RHICommandRemoveShader : public RHICommand<RHICommandRemoveShader>
	{
	public :
		RHICommandRemoveShader(const Memory::RefPtr<RHIShader> shader);
		~RHICommandRemoveShader() = default;

	public :
		void Execute(Memory::RefPtr<RHISystem> system);

	private :
		const Memory::RefPtr<RHIShader> m_shader;
	};

	class RHICommandRemovePipeLine : public RHICommand<RHICommandRemovePipeLine>
	{
	public :
		RHICommandRemovePipeLine(const Memory::RefPtr<RHIPipeLine> pipeline);
		~RHICommandRemovePipeLine() = default;

	public :
		void Execute(Memory::RefPtr<RHISystem> system);

	private :
		const Memory::RefPtr<RHIPipeLine> m_pipeline;
	};

	class RHICommandDrawIndexPrimitive : public RHICommand<RHICommandDrawIndexPrimitive>
	{
	public :
		RHICommandDrawIndexPrimitive() = default;
		~RHICommandDrawIndexPrimitive() = default;

	public :
		void Execute(Memory::RefPtr<RHISystem> system);
	};
};

#endif // __WTR_RHICOMMANDS_H__