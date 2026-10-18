#include <RHI/RHICommands.h>

#include <RHI/RHIResources.h>
#include <RHI/RHIStates.h>
#include <RHI/RHISystem.h>

namespace wtr
{
	RHICommandBase::RHICommandBase(const RHICommandBase::ExecuteFunc func)
		: m_func(func)
	{}

	void RHICommandBase::Execute()
	{
		if (m_func)
		{
			m_func(this);
		}
	}

	RHICommandResize::RHICommandResize(const uint32_t width, const uint32_t height)
		: m_width(width)
		, m_height(height)
		, m_posX(0)
		, m_posY(0)
	{}

	RHICommandResize::RHICommandResize(const uint32_t posX, const uint32_t posY, const uint32_t width, const uint32_t height)
		: m_width(width)
		, m_height(height)
		, m_posX(posX)
		, m_posY(posY)
	{}

	void RHICommandResize::Execute()
	{
		RHISystem::Resize(m_posX, m_posY, m_width, m_height);
	}

	RHICommandInitializeBuffer::RHICommandInitializeBuffer(const Memory::RefPtr<RHIBuffer> buffer, const RHIBufferCreateDesc info)
		: m_info(info)
		, m_buffer(buffer)
	{}

	void RHICommandInitializeBuffer::Execute()
	{
		if (m_buffer)
		{
			RHISystem::InitializeBuffer(m_buffer, m_info);
		}
	}

	RHICommandInitializeVertexLayout::RHICommandInitializeVertexLayout(const Memory::RefPtr<RHIVertexLayout> layout, const RHIVertexLayoutCreateDesc info)
		: m_info(info)
		, m_layout(layout)
	{}

	void RHICommandInitializeVertexLayout::Execute()
	{
		if (m_layout)
		{
			RHISystem::InitializeVertexLayout(m_layout, m_info);
		}
	}

	RHICommandInitializeTexture::RHICommandInitializeTexture(const Memory::RefPtr<RHITexture> texture, const RHITextureCreateDesc info)
		: m_info(info)
		, m_texture(texture)
	{}

	void RHICommandInitializeTexture::Execute()
	{
		if (m_texture)
		{
			RHISystem::InitializeTexture(m_texture, m_info);
		}
	}

	RHICommandInitializeSampler::RHICommandInitializeSampler(const Memory::RefPtr<RHISampler> sampler, const RHISamplerCreateDesc info)
		: m_info(info)
		, m_sampler(sampler)
	{}

	void RHICommandInitializeSampler::Execute()
	{
		if (m_sampler)
		{
			RHISystem::InitializeSampler(m_sampler, m_info);
		}
	}

	RHICommandInitializeVertexShader::RHICommandInitializeVertexShader(const Memory::RefPtr<RHIShader> shader, const RHIShaderCreateDesc info)
		: m_info(info)
		, m_shader(shader)
	{}

	void RHICommandInitializeVertexShader::Execute()
	{
		if (m_shader)
		{
			RHISystem::InitializeShader(m_shader, m_info);
		}
	}

	RHICommandInitializeGeometryShader::RHICommandInitializeGeometryShader(const Memory::RefPtr<RHIShader> shader, const RHIShaderCreateDesc info)
		: m_info(info)
		, m_shader(shader)
	{}

	void RHICommandInitializeGeometryShader::Execute()
	{
		if (m_shader)
		{
			RHISystem::InitializeShader(m_shader, m_info);
		}
	}

	RHICommandInitializeHullShader::RHICommandInitializeHullShader(const Memory::RefPtr<RHIShader> shader, const RHIShaderCreateDesc info)
		: m_info(info)
		, m_shader(shader)
	{}

	void RHICommandInitializeHullShader::Execute()
	{
		if (m_shader)
		{
			RHISystem::InitializeShader(m_shader, m_info);
		}
	}

	RHICommandInitializeDomainShader::RHICommandInitializeDomainShader(const Memory::RefPtr<RHIShader> shader, const RHIShaderCreateDesc info)
		: m_info(info)
		, m_shader(shader)
	{}

	void RHICommandInitializeDomainShader::Execute()
	{
		if (m_shader)
		{
			RHISystem::InitializeShader(m_shader, m_info);
		}
	}

	RHICommandInitializePixelShader::RHICommandInitializePixelShader(const Memory::RefPtr<RHIShader> shader, const RHIShaderCreateDesc info)
		: m_info(info)
		, m_shader(shader)
	{}

	void RHICommandInitializePixelShader::Execute()
	{
		if (m_shader)
		{
			RHISystem::InitializeShader(m_shader, m_info);
		}
	}

	RHICommandInitializeComputeShader::RHICommandInitializeComputeShader(const Memory::RefPtr<RHIShader> shader, const RHIShaderCreateDesc info)
		: m_info(info)
		, m_shader(shader)
	{}

	void RHICommandInitializeComputeShader::Execute()
	{
		if (m_shader)
		{
			RHISystem::InitializeShader(m_shader, m_info);
		}
	}

	RHICommandInitializePipeLine::RHICommandInitializePipeLine(const Memory::RefPtr<RHIPipeLine> pipeline, const RHIPipeLineCreateDesc info)
		: m_info(info)
		, m_pipeline(pipeline)
	{}

	void RHICommandInitializePipeLine::Execute()
	{
		if (m_pipeline)
		{
			RHISystem::InitializePipeLine(m_pipeline, m_info);
		}
	}

	RHICommandInitializeRenderPass::RHICommandInitializeRenderPass(const Memory::RefPtr<RHIRenderPass> pass, const RHIRenderPassCreateDesc info)
		: m_info(info)
		, m_pass(pass)
	{}

	void RHICommandInitializeRenderPass::Execute()
	{
		if (m_pass)
		{
			RHISystem::InitializeRenderPass(m_pass, m_info);
		}
	}

	RHICommandRemoveVertexLayout::RHICommandRemoveVertexLayout(const Memory::RefPtr<RHIVertexLayout> layout)
		: m_layout(layout)
	{}

	void RHICommandRemoveVertexLayout::Execute()
	{
		if (m_layout)
		{
			RHISystem::RemoveVertexLayout(m_layout);
		}
	}

	RHICommandRemoveBuffer::RHICommandRemoveBuffer(const Memory::RefPtr<RHIBuffer> buffer)
		: m_buffer(buffer)
	{}

	void RHICommandRemoveBuffer::Execute()
	{
		if (m_buffer)
		{
			RHISystem::RemoveBuffer(m_buffer);
		}
	}

	RHICommandRemoveTexture::RHICommandRemoveTexture(const Memory::RefPtr<RHITexture> texture)
		: m_texture(texture)
	{}

	void RHICommandRemoveTexture::Execute()
	{
		if (m_texture)
		{
			RHISystem::RemoveTexture(m_texture);
		}
	}

	RHICommandRemoveSampler::RHICommandRemoveSampler(const Memory::RefPtr<RHISampler> sampler)
		: m_sampler(sampler)
	{}

	void RHICommandRemoveSampler::Execute()
	{
		if (m_sampler)
		{
			RHISystem::RemoveSampler(m_sampler);
		}
	}

	RHICommandRemoveShader::RHICommandRemoveShader(const Memory::RefPtr<RHIShader> shader)
		: m_shader(shader)
	{}

	void RHICommandRemoveShader::Execute()
	{
		if (m_shader)
		{
			RHISystem::RemoveShader(m_shader);
		}
	}

	RHICommandRemovePipeLine::RHICommandRemovePipeLine(const Memory::RefPtr<RHIPipeLine> pipeline)
		: m_pipeline(pipeline)
	{}

	void RHICommandRemovePipeLine::Execute()
	{
		if (m_pipeline)
		{
			RHISystem::RemovePipeLine(m_pipeline);
		}
	}

	RHICommandRemoveRenderPass::RHICommandRemoveRenderPass(const Memory::RefPtr<RHIRenderPass> pass)
		: m_pass(pass)
	{}

	void RHICommandRemoveRenderPass::Execute()
	{
		if (m_pass)
		{
			RHISystem::RemoveRenderPass(m_pass);
		}
	}

	RHICommandUpdateBuffer::RHICommandUpdateBuffer(const Memory::RefPtr<RHIBuffer> buffer, const RHIBufferUpdateDesc info)
		: m_info(info)
		, m_buffer(buffer)
	{}

	void RHICommandUpdateBuffer::Execute()
	{
		if (m_buffer)
		{
			RHISystem::UpdateBuffer(m_buffer, m_info);
		}
	}

	RHICommandUpdateTexture::RHICommandUpdateTexture(const Memory::RefPtr<RHITexture> texture, const RHITextureUpdateDesc info)
		: m_info(info)
		, m_texture(texture)
	{}

	void RHICommandUpdateTexture::Execute()
	{
		if (m_texture)
		{
			RHISystem::UpdateTexture(m_texture, m_info);
		}
	}

	RHICommandUpdateVertexLayout::RHICommandUpdateVertexLayout(const Memory::RefPtr<RHIVertexLayout> layout, const RHIVertexLayoutUpdateDesc info)
		: m_info(info)
		, m_layout(layout)
	{}

	void RHICommandUpdateVertexLayout::Execute()
	{
		if (m_layout)
		{
			RHISystem::UpdateVertexLayout(m_layout, m_info);
		}
	}

	RHICommandSetConstant::RHICommandSetConstant(const RHIConstDesc info, const Name& binding)
		: m_info(info)
		, m_binding(binding)
	{}

	void RHICommandSetConstant::Execute()
	{
		RHISystem::SetConstant(m_info, m_binding);
	}

	RHICommandSetVertexLayout::RHICommandSetVertexLayout(const Memory::RefPtr<const RHIVertexLayout> layout)
		: m_layout(layout)
	{}

	void RHICommandSetVertexLayout::Execute()
	{
		if (m_layout)
		{
			RHISystem::SetVertexLayout(m_layout);
		}
	}

	RHICommandSetVertexBuffer::RHICommandSetVertexBuffer(const Memory::RefPtr<const RHIBuffer> buffer, const VertexKey& key, const uint32_t offset)
		: m_buffer(buffer)
		, m_key(key)
		, m_offset(offset)
	{}

	void RHICommandSetVertexBuffer::Execute()
	{
		RHISystem::SetVertexBuffer(m_buffer, m_key, m_offset);
	}

	RHICommandSetIndexBuffer::RHICommandSetIndexBuffer(const Memory::RefPtr<const RHIBuffer> buffer, const uint32_t offset)
		: m_buffer(buffer)
		, m_offset(offset)
	{}

	void RHICommandSetIndexBuffer::Execute()
	{
		RHISystem::SetIndexBuffer(m_buffer, m_offset);
	}

	RHICommandSetBuffer::RHICommandSetBuffer(const Memory::RefPtr<const RHIBuffer> buffer, const Name& binding, const uint32_t offset, const uint32_t range)
		: m_buffer(buffer)
		, m_binding(binding)
		, m_offset(offset)
		, m_range(range)
	{}

	void RHICommandSetBuffer::Execute()
	{
		RHISystem::SetBuffer(m_buffer, m_binding, m_offset, m_range);
	}

	RHICommandSetTexture::RHICommandSetTexture(const Memory::RefPtr<const RHITexture> texture, const Name& binding)
		: m_texture(texture)
		, m_binding(binding)
	{}

	void RHICommandSetTexture::Execute()
	{
		RHISystem::SetTexture(m_texture, m_binding);
	}

	RHICommandSetSampler::RHICommandSetSampler(const Memory::RefPtr<const RHISampler> sampler, const Name& binding)
		: m_sampler(sampler)
		, m_binding(binding)
	{}

	void RHICommandSetSampler::Execute()
	{
		RHISystem::SetSampler(m_sampler, m_binding);
	}

	RHICommandSetPipeLine::RHICommandSetPipeLine(const Memory::RefPtr<const RHIPipeLine> pipeline)
		: m_pipeline(pipeline)
	{}

	void RHICommandSetPipeLine::Execute()
	{
		if (m_pipeline)
		{
			RHISystem::SetPipeLine(m_pipeline);
		}
	}

	RHICommandSetRenderPass::RHICommandSetRenderPass(const Memory::RefPtr<const RHIRenderPass> pass)
		: m_pass(pass)
	{}

	void RHICommandSetRenderPass::Execute()
	{
		if (m_pass)
		{
			RHISystem::SetRenderPass(m_pass);
		}
	}

	RHICommandUnsetVertexLayout::RHICommandUnsetVertexLayout()
	{}

	void RHICommandUnsetVertexLayout::Execute()
	{
		RHISystem::UnsetVertexLayout();
	}

	RHICommandUnsetVertexBuffer::RHICommandUnsetVertexBuffer(const Name& binding)
		: m_binding(binding)
	{}

	void RHICommandUnsetVertexBuffer::Execute()
	{
		RHISystem::UnsetVertexBuffer(m_binding);
	}

	void RHICommandUnsetIndexBuffer::Execute()
	{
		RHISystem::UnsetIndexBuffer();
	}

	RHICommandUnsetBuffer::RHICommandUnsetBuffer(const Memory::RefPtr<const RHIBuffer> buffer, const Name& binding)
		: m_buffer(buffer)
		, m_binding(binding)
	{}

	void RHICommandUnsetBuffer::Execute()
	{
		RHISystem::UnsetBuffer(m_buffer, m_binding);
	}

	RHICommandUnsetTexture::RHICommandUnsetTexture(const Memory::RefPtr<const RHITexture> texture, const Name& binding)
		: m_texture(texture)
		, m_binding(binding)
	{}

	void RHICommandUnsetTexture::Execute()
	{
		RHISystem::UnsetTexture(m_texture, m_binding);
	}

	RHICommandUnsetSampler::RHICommandUnsetSampler(const Memory::RefPtr<const RHISampler> sampler, const Name& binding)
		: m_sampler(sampler)
		, m_binding(binding)
	{}

	void RHICommandUnsetSampler::Execute()
	{
		RHISystem::UnsetSampler(m_sampler, m_binding);
	}

	RHICommandUnsetPipeLine::RHICommandUnsetPipeLine()
	{}

	void RHICommandUnsetPipeLine::Execute()
	{
		RHISystem::UnsetPipeLine();
	}

	RHICommandUnsetRenderPass::RHICommandUnsetRenderPass()
	{}

	void RHICommandUnsetRenderPass::Execute()
	{
		RHISystem::UnsetRenderPass();
	}

	RHICommandTransitionBuffer::RHICommandTransitionBuffer(const Memory::RefPtr<const RHIBuffer> buffer, const RHIBufferState& next)
		: m_buffer(buffer)
		, m_next(next)
	{}

	void RHICommandTransitionBuffer::Execute()
	{
		if (m_buffer)
		{
			RHISystem::Transition(m_buffer, m_next);
		}
	}

	RHICommandTransitionTexture::RHICommandTransitionTexture(const Memory::RefPtr<const RHITexture> texture, const RHITextureState& next)
		: m_texture(texture)
		, m_next(next)
	{}

	void RHICommandTransitionTexture::Execute()
	{
		if (m_texture)
		{
			RHISystem::Transition(m_texture, m_next);
		}
	}

	RHICommandDispatchCompute::RHICommandDispatchCompute(const RHIDispatchDesc info)
		: m_info(info)
	{}

	void RHICommandDispatchCompute::Execute()
	{
		RHISystem::DispatchCompute(m_info);
	}

	RHICommandDrawDirect::RHICommandDrawDirect(const RHIDrawDirectDesc info)
		: m_info(info)
	{}

	void RHICommandDrawDirect::Execute()
	{
		RHISystem::DrawDirect(m_info);
	}

	RHICommandDrawIndirect::RHICommandDrawIndirect(const RHIDrawIndirectDesc info)
		: m_info(info)
	{}

	void RHICommandDrawIndirect::Execute()
	{
		RHISystem::DrawIndirect(m_info);
	}
}
