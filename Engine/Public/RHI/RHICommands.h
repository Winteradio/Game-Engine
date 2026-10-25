#ifndef __WTR_RHICOMMANDS_H__
#define __WTR_RHICOMMANDS_H__

#include <Reflection/include/Utils.h>
#include <Memory/include/Pointer/RefPtr.h>

#include <Framework/Name.h>
#include <RHI/RHIDescriptions.h>

namespace wtr
{
	class RHIBuffer;
	class RHIVertexLayout;
	class RHITexture;
	class RHISampler;
	class RHIShader;
	class RHIPipeLine;
	class RHIRenderPass;
};

namespace wtr
{
	class RHICommandBase
	{
	public :
		using ExecuteFunc = void(*)(RHICommandBase*);

		RHICommandBase(const ExecuteFunc func);
		virtual ~RHICommandBase() {}

	public :
		void Execute();

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
		static void ExecuteAndDestruct(RHICommandBase* commandBase)
		{
			static_assert(Reflection::Utils::IsBase<RHICommandBase, T>::value && "The RHI Command's type is invalid");

			if (nullptr != commandBase)
			{
				T* command = static_cast<T*>(commandBase);
				command->Execute();
				command->~T();
			}
		}
	};

	class RHICommandResize : public RHICommand<RHICommandResize>
	{
	public :
		RHICommandResize(const uint32_t width, const uint32_t height);
		RHICommandResize(const uint32_t posX, const uint32_t posY, const uint32_t width, const uint32_t height);
		~RHICommandResize() = default;

	public :
		void Execute();

	private :
		const uint32_t m_width;
		const uint32_t m_height;
		const uint32_t m_posX;
		const uint32_t m_posY;
	};

	class RHICommandInitializeBuffer : public RHICommand<RHICommandInitializeBuffer>
	{
	public :
		RHICommandInitializeBuffer(Memory::RefPtr<RHIBuffer> buffer, const RHIBufferCreateDesc info);
		~RHICommandInitializeBuffer() = default;

	public :
		void Execute();

	private :
		const RHIBufferCreateDesc m_info;
		Memory::RefPtr<RHIBuffer> m_buffer;
	};

	class RHICommandInitializeVertexLayout : public RHICommand<RHICommandInitializeVertexLayout>
	{
	public :
		RHICommandInitializeVertexLayout(Memory::RefPtr<RHIVertexLayout> layout, const RHIVertexLayoutCreateDesc info);
		~RHICommandInitializeVertexLayout() = default;

	public:
		void Execute();

	private:
		const RHIVertexLayoutCreateDesc m_info;
		Memory::RefPtr<RHIVertexLayout> m_layout;
	};

	class RHICommandInitializeTexture : public RHICommand<RHICommandInitializeTexture>
	{
	public :
		RHICommandInitializeTexture(Memory::RefPtr<RHITexture> texture, const RHITextureCreateDesc info);
		~RHICommandInitializeTexture() = default;

	public :
		void Execute();

	private :
		const RHITextureCreateDesc m_info;
		Memory::RefPtr<RHITexture> m_texture;
	};

	class RHICommandInitializeSampler : public RHICommand<RHICommandInitializeSampler>
	{
	public :
		RHICommandInitializeSampler(Memory::RefPtr<RHISampler> sampler, const RHISamplerCreateDesc info);
		~RHICommandInitializeSampler() = default;

	public :
		void Execute();

	private :
		const RHISamplerCreateDesc m_info;
		Memory::RefPtr<RHISampler> m_sampler;
	};

	class RHICommandInitializeVertexShader : public RHICommand<RHICommandInitializeVertexShader>
	{
	public :
		RHICommandInitializeVertexShader(Memory::RefPtr<RHIShader> shader, const RHIShaderCreateDesc info);
		~RHICommandInitializeVertexShader() = default;

	public :
		void Execute();

	private :
		const RHIShaderCreateDesc m_info;
		Memory::RefPtr<RHIShader> m_shader;
	};

	class RHICommandInitializeGeometryShader : public RHICommand<RHICommandInitializeGeometryShader>
	{
	public :
		RHICommandInitializeGeometryShader(Memory::RefPtr<RHIShader> shader, const RHIShaderCreateDesc info);
		~RHICommandInitializeGeometryShader() = default;

	public :
		void Execute();

	private :
		const RHIShaderCreateDesc m_info;
		Memory::RefPtr<RHIShader> m_shader;
	};

	class RHICommandInitializeHullShader : public RHICommand<RHICommandInitializeHullShader>
	{
	public:
		RHICommandInitializeHullShader(Memory::RefPtr<RHIShader> shader, const RHIShaderCreateDesc info);
		~RHICommandInitializeHullShader() = default;

	public:
		void Execute();

	private:
		const RHIShaderCreateDesc m_info;
		Memory::RefPtr<RHIShader> m_shader;
	};

	class RHICommandInitializeDomainShader : public RHICommand<RHICommandInitializeDomainShader>
	{
	public :
		RHICommandInitializeDomainShader(Memory::RefPtr<RHIShader> shader, const RHIShaderCreateDesc info);
		~RHICommandInitializeDomainShader() = default;

	public :
		void Execute();

	private :
		const RHIShaderCreateDesc m_info;
		Memory::RefPtr<RHIShader> m_shader;
	};

	class RHICommandInitializePixelShader : public RHICommand<RHICommandInitializePixelShader>
	{
	public :
		RHICommandInitializePixelShader(Memory::RefPtr<RHIShader> shader, const RHIShaderCreateDesc info);
		~RHICommandInitializePixelShader() = default;

	public :
		void Execute();

	private :
		const RHIShaderCreateDesc m_info;
		Memory::RefPtr<RHIShader> m_shader;
	};

	class RHICommandInitializeComputeShader : public RHICommand<RHICommandInitializeComputeShader>
	{
	public :
		RHICommandInitializeComputeShader(Memory::RefPtr<RHIShader> shader, const RHIShaderCreateDesc info);
		~RHICommandInitializeComputeShader() = default;

	public :
		void Execute();

	private :
		const RHIShaderCreateDesc m_info;
		Memory::RefPtr<RHIShader> m_shader;
	};

	class RHICommandInitializePipeLine : public RHICommand<RHICommandInitializePipeLine>
	{
	public :
		RHICommandInitializePipeLine(Memory::RefPtr<RHIPipeLine> pipeline, const RHIPipeLineCreateDesc info);
		~RHICommandInitializePipeLine() = default;

	public :
		void Execute();

	private :
		const RHIPipeLineCreateDesc m_info;
		Memory::RefPtr<RHIPipeLine> m_pipeline;
	};

	class RHICommandInitializeRenderPass : public RHICommand<RHICommandInitializeRenderPass>
	{
	public :
		RHICommandInitializeRenderPass(Memory::RefPtr<RHIRenderPass> pass, const RHIRenderPassCreateDesc info);
		~RHICommandInitializeRenderPass() = default;

	public :
		void Execute();

	private :
		const RHIRenderPassCreateDesc m_info;
		Memory::RefPtr<RHIRenderPass> m_pass;
	};

		class RHICommandRemoveVertexLayout : public RHICommand<RHICommandRemoveVertexLayout>
	{
	public:
		RHICommandRemoveVertexLayout(Memory::RefPtr<RHIVertexLayout> layout);
		~RHICommandRemoveVertexLayout() = default;

	public:
		void Execute();

	private:
		Memory::RefPtr<RHIVertexLayout> m_layout;
	};

	class RHICommandRemoveBuffer : public RHICommand<RHICommandRemoveBuffer>
	{
	public:
		RHICommandRemoveBuffer(Memory::RefPtr<RHIBuffer> buffer);
		~RHICommandRemoveBuffer() = default;

	public:
		void Execute();

	private:
		Memory::RefPtr<RHIBuffer> m_buffer;
	};

	class RHICommandRemoveTexture : public RHICommand<RHICommandRemoveTexture>
	{
	public:
		RHICommandRemoveTexture(Memory::RefPtr<RHITexture> texture);
		~RHICommandRemoveTexture() = default;

	public:
		void Execute();

	private:
		Memory::RefPtr<RHITexture> m_texture;
	};

	class RHICommandRemoveSampler : public RHICommand<RHICommandRemoveSampler>
	{
	public:
		RHICommandRemoveSampler(Memory::RefPtr<RHISampler> sampler);
		~RHICommandRemoveSampler() = default;

	public:
		void Execute();

	private:
		Memory::RefPtr<RHISampler> m_sampler;
	};

	class RHICommandRemoveShader : public RHICommand<RHICommandRemoveShader>
	{
	public:
		RHICommandRemoveShader(Memory::RefPtr<RHIShader> shader);
		~RHICommandRemoveShader() = default;

	public:
		void Execute();

	private:
		Memory::RefPtr<RHIShader> m_shader;
	};

	class RHICommandRemovePipeLine : public RHICommand<RHICommandRemovePipeLine>
	{
	public:
		RHICommandRemovePipeLine(Memory::RefPtr<RHIPipeLine> pipeline);
		~RHICommandRemovePipeLine() = default;

	public:
		void Execute();

	private:
		Memory::RefPtr<RHIPipeLine> m_pipeline;
	};

	class RHICommandRemoveRenderPass : public RHICommand<RHICommandRemoveRenderPass>
	{
	public:
		RHICommandRemoveRenderPass(Memory::RefPtr<RHIRenderPass> pass);
		~RHICommandRemoveRenderPass() = default;

	public:
		void Execute();

	private:
		Memory::RefPtr<RHIRenderPass> m_pass;
	};

	class RHICommandUpdateBuffer : public RHICommand<RHICommandUpdateBuffer>
	{
	public :
		RHICommandUpdateBuffer(Memory::RefPtr<RHIBuffer> buffer, const RHIBufferUpdateDesc info);
		~RHICommandUpdateBuffer() = default;

	public :
		void Execute();

	private :
		const RHIBufferUpdateDesc m_info;
		Memory::RefPtr<RHIBuffer> m_buffer;
	};

	class RHICommandUpdateTexture : public RHICommand<RHICommandUpdateTexture>
	{
	public :
		RHICommandUpdateTexture(Memory::RefPtr<RHITexture> texture, const RHITextureUpdateDesc info);
		~RHICommandUpdateTexture() = default;

	public :
		void Execute();

	private :
		const RHITextureUpdateDesc m_info;
		Memory::RefPtr<RHITexture> m_texture;
	};

	class RHICommandSetConstant : public RHICommand<RHICommandSetConstant>
	{
	public :
		RHICommandSetConstant(const RHIConstDesc info, const Name& binding);
		~RHICommandSetConstant() = default;

	public :
		void Execute();

	private:
		const RHIConstDesc m_info;
		const Name m_binding;
	};

	class RHICommandSetVertexLayout : public RHICommand<RHICommandSetVertexLayout>
	{
	public:
		RHICommandSetVertexLayout(Memory::RefPtr<const RHIVertexLayout> layout);
		~RHICommandSetVertexLayout() = default;

	public:
		void Execute();

	private:
		Memory::RefPtr<const RHIVertexLayout> m_layout;
	};

	class RHICommandSetVertexBuffer : public RHICommand<RHICommandSetVertexBuffer>
	{
	public:
		RHICommandSetVertexBuffer(Memory::RefPtr<const RHIBuffer> buffer, const VertexKey& key, const uint32_t offset);
		~RHICommandSetVertexBuffer() = default;

	public:
		void Execute();

	private:
		Memory::RefPtr<const RHIBuffer> m_buffer;
		const VertexKey m_key;
		const uint32_t m_offset;
	};

	class RHICommandSetIndexBuffer : public RHICommand<RHICommandSetIndexBuffer>
	{
	public :
		RHICommandSetIndexBuffer(Memory::RefPtr<const RHIBuffer> buffer, const uint32_t offset);
		~RHICommandSetIndexBuffer() = default;

	public :
		void Execute();
	private :
		Memory::RefPtr<const RHIBuffer> m_buffer;
		const uint32_t m_offset;
	};

	class RHICommandSetBuffer : public RHICommand<RHICommandSetBuffer>
	{
	public :
		RHICommandSetBuffer(Memory::RefPtr<const RHIBuffer> buffer, const Name& binding, const uint32_t offset, const uint32_t range);
		~RHICommandSetBuffer() = default;

	public :
		void Execute();

	private:
		Memory::RefPtr<const RHIBuffer> m_buffer;
		const Name m_binding;
		const uint32_t m_offset;
		const uint32_t m_range;
	};

	class RHICommandSetTexture : public RHICommand<RHICommandSetTexture>
	{
	public :
		RHICommandSetTexture(Memory::RefPtr<const RHITexture> texture, const Name& binding);
		~RHICommandSetTexture() = default;

	public:
		void Execute();

	private:
		Memory::RefPtr<const RHITexture> m_texture;
		const Name m_binding;
	};

	class RHICommandSetSampler : public RHICommand<RHICommandSetSampler>
	{
	public :
		RHICommandSetSampler(Memory::RefPtr<const RHISampler> sampler, const Name& binding);
		~RHICommandSetSampler() = default;

	public:
		void Execute();

	private:
		Memory::RefPtr<const RHISampler> m_sampler;
		const Name m_binding;
	};

	class RHICommandSetPipeLine : public RHICommand<RHICommandSetPipeLine>
	{
	public :
		RHICommandSetPipeLine(Memory::RefPtr<const RHIPipeLine> pipeline);
		~RHICommandSetPipeLine() = default;

	public:
		void Execute();

	private :
		Memory::RefPtr<const RHIPipeLine> m_pipeline;
	};

	class RHICommandSetRenderPass : public RHICommand<RHICommandSetRenderPass>
	{
	public :
		RHICommandSetRenderPass(Memory::RefPtr<const RHIRenderPass> pass);
		~RHICommandSetRenderPass() = default;

	public :
		void Execute();

	private :
		Memory::RefPtr<const RHIRenderPass> m_pass;
	};

	class RHICommandUnsetVertexLayout : public RHICommand<RHICommandUnsetVertexLayout>
	{
	public:
		RHICommandUnsetVertexLayout();
		~RHICommandUnsetVertexLayout() = default;

	public:
		void Execute();
	};

	class RHICommandUnsetVertexBuffer : public RHICommand<RHICommandUnsetVertexBuffer>
	{
	public :
		RHICommandUnsetVertexBuffer(const Name& binding);
		~RHICommandUnsetVertexBuffer() = default;

	public :
		void Execute();

	private :
		const Name m_binding;
	};

	class RHICommandUnsetIndexBuffer : public RHICommand<RHICommandUnsetIndexBuffer>
	{
	public :
		RHICommandUnsetIndexBuffer() = default;
		~RHICommandUnsetIndexBuffer() = default;

	public :
		void Execute();
	};

	class RHICommandUnsetBuffer : public RHICommand<RHICommandUnsetBuffer>
	{
	public:
		RHICommandUnsetBuffer(Memory::RefPtr<const RHIBuffer> buffer, const Name& binding);
		~RHICommandUnsetBuffer() = default;

	public:
		void Execute();

	private:
		Memory::RefPtr<const RHIBuffer> m_buffer;
		const Name m_binding;
	};

	class RHICommandUnsetTexture : public RHICommand<RHICommandUnsetTexture>
	{
	public:
		RHICommandUnsetTexture(Memory::RefPtr<const RHITexture> texture, const Name& binding);
		~RHICommandUnsetTexture() = default;

	public:
		void Execute();

	private:
		Memory::RefPtr<const RHITexture> m_texture;
		const Name m_binding;
	};

	class RHICommandUnsetSampler : public RHICommand<RHICommandUnsetSampler>
	{
	public:
		RHICommandUnsetSampler(Memory::RefPtr<const RHISampler> sampler, const Name& binding);
		~RHICommandUnsetSampler() = default;

	public:
		void Execute();

	private:
		Memory::RefPtr<const RHISampler> m_sampler;
		const Name m_binding;
	};

	class RHICommandUnsetPipeLine : public RHICommand<RHICommandUnsetPipeLine>
	{
	public :
		RHICommandUnsetPipeLine();
		~RHICommandUnsetPipeLine() = default;

	public:
		void Execute();
	};

	class RHICommandUnsetRenderPass : public RHICommand<RHICommandUnsetRenderPass>
	{
	public :
		RHICommandUnsetRenderPass();
		~RHICommandUnsetRenderPass() = default;

	public :
		void Execute();
	};

	class RHICommandTransitionBuffer : public RHICommand<RHICommandTransitionBuffer>
	{
	public :
		RHICommandTransitionBuffer(Memory::RefPtr<const RHIBuffer> buffer, const RHIBufferState& next);
		~RHICommandTransitionBuffer() = default;

	public:
		void Execute();

	private:
		Memory::RefPtr<const RHIBuffer> m_buffer;
		const RHIBufferState m_next;
	};

	class RHICommandTransitionTexture : public RHICommand<RHICommandTransitionTexture>
	{
	public :
		RHICommandTransitionTexture(Memory::RefPtr<const RHITexture> texture, const RHITextureState& next);
		~RHICommandTransitionTexture() = default;

	public :
		void Execute();

	private :
		Memory::RefPtr<const RHITexture> m_texture;
		const RHITextureState m_next;
	};

	class RHICommandDispatchCompute : public RHICommand<RHICommandDispatchCompute>
	{
	public :
		RHICommandDispatchCompute(const RHIDispatchDesc info);
		~RHICommandDispatchCompute() = default;

	public:
		void Execute();

	private:
		const RHIDispatchDesc m_info;
	};

	class RHICommandDrawDirect : public RHICommand<RHICommandDrawDirect>
	{
	public :
		RHICommandDrawDirect(const RHIDrawDirectDesc info);
		~RHICommandDrawDirect() = default;

	public :
		void Execute();

	private :
		const RHIDrawDirectDesc m_info;
	};

	class RHICommandDrawIndirect : public RHICommand<RHICommandDrawIndirect>
	{
	public:
		RHICommandDrawIndirect(const RHIDrawIndirectDesc info);
		~RHICommandDrawIndirect() = default;

	public :
		void Execute();

	private :
		const RHIDrawIndirectDesc m_info;
	};
};

#endif // __WTR_RHICOMMANDS_H__