#include <RHI/RHICommands.h>

#include <RHI/RHIResources.h>
#include <RHI/RHIStates.h>
#include <RHI/RHISystem.h>

namespace wtr
{
	RHICommandBase::RHICommandBase(const RHICommandBase::ExecuteFunc func)
		: m_func(func)
	{}

	void RHICommandBase::Execute(Memory::RefPtr<RHISystem> system)
	{
		if (m_func)
		{
			m_func(system, this);
		}
	}

	RHICommandClear::RHICommandClear(const RHIClearState& state)
		: m_state(state)
	{}

	void RHICommandClear::Execute(Memory::RefPtr<RHISystem> system)
	{
		if (system)
		{
			system->Clear(m_state);
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

	void RHICommandResize::Execute(Memory::RefPtr<RHISystem> system)
	{
		if (system)
		{
			system->Resize(m_posX, m_posY, m_width, m_height);
		}
	}

	RHICommandInitializeBuffer::RHICommandInitializeBuffer(const RHIBufferCreateDesc info, const Memory::RefPtr<RHIBuffer> buffer)
		: m_info(info)
		, m_buffer(buffer)
	{}

	void RHICommandInitializeBuffer::Execute(Memory::RefPtr<RHISystem> system)
	{
		if (system && m_buffer)
		{
			system->InitializeBuffer(m_info, m_buffer);
		}
	}

	RHICommandInitializeVertexLayout::RHICommandInitializeVertexLayout(const RHIVertexLayoutCreateDesc info, const Memory::RefPtr<RHIVertexLayout> layout)
		: m_info(info)
		, m_layout(layout)
	{}

	void RHICommandInitializeVertexLayout::Execute(Memory::RefPtr<RHISystem> system)
	{
		if (system && m_layout)
		{
			system->InitializeVertexLayout(m_info, m_layout);
		}
	}

	RHICommandInitializeTexture::RHICommandInitializeTexture(const RHITextureCreateDesc info, const Memory::RefPtr<RHITexture> texture)
		: m_info(info)
		, m_texture(texture)
	{}

	void RHICommandInitializeTexture::Execute(Memory::RefPtr<RHISystem> system)
	{
		if (system && m_texture)
		{
			system->InitializeTexture(m_info, m_texture);
		}
	}

	RHICommandInitializeSampler::RHICommandInitializeSampler(const RHISamplerCreateDesc info, const Memory::RefPtr<RHISampler> sampler)
		: m_info(info)
		, m_sampler(sampler)
	{}

	void RHICommandInitializeSampler::Execute(Memory::RefPtr<RHISystem> system)
	{
		if (system && m_sampler)
		{
			system->InitializeSampler(m_info, m_sampler);		
		}
	}

	RHICommandInitializeVertexShader::RHICommandInitializeVertexShader(const RHIShaderCreateDesc info, const Memory::RefPtr<RHIShader> shader)
		: m_info(info)
		, m_shader(shader)
	{}

	void RHICommandInitializeVertexShader::Execute(Memory::RefPtr<RHISystem> system)
	{
		if (system && m_shader)
		{
			system->InitializeShader(m_info, m_shader);
		}
	}

	RHICommandInitializeGeometryShader::RHICommandInitializeGeometryShader(const RHIShaderCreateDesc info, const Memory::RefPtr<RHIShader> shader)
		: m_info(info)
		, m_shader(shader)
	{}

	void RHICommandInitializeGeometryShader::Execute(Memory::RefPtr<RHISystem> system)
	{
		if (system && m_shader)
		{
			system->InitializeShader(m_info, m_shader);
		}
	}

	RHICommandInitializeHullShader::RHICommandInitializeHullShader(const RHIShaderCreateDesc info, const Memory::RefPtr<RHIShader> shader)
		: m_info(info)
		, m_shader(shader)
	{
	}

	void RHICommandInitializeHullShader::Execute(Memory::RefPtr<RHISystem> system)
	{
		if (system && m_shader)
		{
			system->InitializeShader(m_info, m_shader);
		}
	}

	RHICommandInitializeDomainShader::RHICommandInitializeDomainShader(const RHIShaderCreateDesc info, const Memory::RefPtr<RHIShader> shader)
		: m_info(info)
		, m_shader(shader)
	{}

	void RHICommandInitializeDomainShader::Execute(Memory::RefPtr<RHISystem> system)
	{
		if (system && m_shader)
		{
			system->InitializeShader(m_info, m_shader);
		}
	}

	RHICommandInitializePixelShader::RHICommandInitializePixelShader(const RHIShaderCreateDesc info, const Memory::RefPtr<RHIShader> shader)
		: m_info(info)
		, m_shader(shader)
	{}

	void RHICommandInitializePixelShader::Execute(Memory::RefPtr<RHISystem> system)
	{
		if (system && m_shader)
		{
			system->InitializeShader(m_info, m_shader);
		}
	}

	RHICommandInitializeComputeShader::RHICommandInitializeComputeShader(const RHIShaderCreateDesc info, const Memory::RefPtr<RHIShader> shader)
		: m_info(info)
		, m_shader(shader)
	{}

	void RHICommandInitializeComputeShader::Execute(Memory::RefPtr<RHISystem> system)
	{
		if (system && m_shader)
		{
			system->InitializeShader(m_info, m_shader);
		}
	}

	RHICommandInitializePipeLine::RHICommandInitializePipeLine(const RHIPipeLineCreateDesc info, const Memory::RefPtr<RHIPipeLine> pipeline)
		: m_info(info)
		, m_pipeline(pipeline)
	{}

	void RHICommandInitializePipeLine::Execute(Memory::RefPtr<RHISystem> system)
	{
		if (system && m_pipeline)
		{
			system->InitializePipeLine(m_info, m_pipeline);
		}
	}

	RHICommandInitializeRenderTarget::RHICommandInitializeRenderTarget(const RHIRenderTargetCreateDesc info, const Memory::RefPtr<RHIRenderTarget> target)
		: m_info(info)
		, m_target(target)
	{}

	void RHICommandInitializeRenderTarget::Execute(Memory::RefPtr<RHISystem> system)
	{
		if (system && m_target)
		{
			system->InitializeRenderTarget(m_info, m_target);
		}
	}

	RHICommandUpdateBuffer::RHICommandUpdateBuffer(const RHIBufferUpdateDesc info, const Memory::RefPtr<RHIBuffer> buffer)
		: m_info(info)
		, m_buffer(buffer)
	{}

	void RHICommandUpdateBuffer::Execute(Memory::RefPtr<RHISystem> system)
	{
		if (system && m_buffer)
		{
			system->UpdateBuffer(m_info, m_buffer);
		}
	}

	RHICommandUpdateTexture::RHICommandUpdateTexture(const RHITextureUpdateDesc info, const Memory::RefPtr<RHITexture> texture)
		: m_info(info)
		, m_texture(texture)
	{}

	void RHICommandUpdateTexture::Execute(Memory::RefPtr<RHISystem> system)
	{
		if (system && m_texture)
		{
			system->UpdateTexture(m_info, m_texture);
		}
	}

	RHICommandUpdateVertexLayout::RHICommandUpdateVertexLayout(const RHIVertexLayoutUpdateDesc info, const Memory::RefPtr<RHIVertexLayout> layout)
		: m_info(info)
		, m_layout(layout)
	{
	}

	void RHICommandUpdateVertexLayout::Execute(Memory::RefPtr<RHISystem> system)
	{
		if (system && m_layout)
		{
			system->UpdateVertexLayout(m_info, m_layout);
		}
	}

	RHICommandResizeBuffer::RHICommandResizeBuffer(const RHIBufferCreateDesc info, const Memory::RefPtr<RHIBuffer> buffer)
		: m_info(info)
		, m_buffer(buffer)
	{
	}

	void RHICommandResizeBuffer::Execute(Memory::RefPtr<RHISystem> system)
	{
		if (system && m_buffer)
		{
			system->ResizeBuffer(m_info, m_buffer);
		}
	}

	RHICommandResizeTexture::RHICommandResizeTexture(const RHITextureCreateDesc info, const Memory::RefPtr<RHITexture> texture)
		: m_info(info)
		, m_texture(texture)
	{
	}

	void RHICommandResizeTexture::Execute(Memory::RefPtr<RHISystem> system)
	{
		if (system && m_texture)
		{
			system->ResizeTexture(m_info, m_texture);
		}
	}

	RHICommandRemoveBuffer::RHICommandRemoveBuffer(const Memory::RefPtr<RHIBuffer> buffer)
		: m_buffer(buffer)
	{}

	void RHICommandRemoveBuffer::Execute(Memory::RefPtr<RHISystem> system)
	{
		if (system && m_buffer)
		{
			system->RemoveBuffer(m_buffer);
		}
	}

	RHICommandRemoveVertexLayout::RHICommandRemoveVertexLayout(const Memory::RefPtr<RHIVertexLayout> layout)
		: m_layout(layout)
	{}

	void RHICommandRemoveVertexLayout::Execute(Memory::RefPtr<RHISystem> system)
	{
		if (system && m_layout)
		{
			system->RemoveVertexLayout(m_layout);
		}
	}

	RHICommandRemoveTexture::RHICommandRemoveTexture(const Memory::RefPtr<RHITexture> texture)
		: m_texture(texture)
	{}

	void RHICommandRemoveTexture::Execute(Memory::RefPtr<RHISystem> system)
	{
		if (system && m_texture)
		{
			system->RemoveTexture(m_texture);
		}
	}

	RHICommandRemoveSampler::RHICommandRemoveSampler(const Memory::RefPtr<RHISampler> sampler)
		: m_sampler(sampler)
	{}

	void RHICommandRemoveSampler::Execute(Memory::RefPtr<RHISystem> system)
	{
		if (system && m_sampler)
		{
			system->RemoveSampler(m_sampler);
		}
	}

	RHICommandRemoveShader::RHICommandRemoveShader(const Memory::RefPtr<RHIShader> shader)
		: m_shader(shader)
	{}

	void RHICommandRemoveShader::Execute(Memory::RefPtr<RHISystem> system)
	{
		if (system && m_shader)
		{
			system->RemoveShader(m_shader);
		}
	}

	RHICommandRemovePipeLine::RHICommandRemovePipeLine(const Memory::RefPtr<RHIPipeLine> pipeline)
		: m_pipeline(pipeline)
	{}

	void RHICommandRemovePipeLine::Execute(Memory::RefPtr<RHISystem> system)
	{
		if (system && m_pipeline)
		{
			system->RemovePipeLine(m_pipeline);
		}
	}

	RHICommandRemoveRenderTarget::RHICommandRemoveRenderTarget(const Memory::RefPtr<RHIRenderTarget> target)
		: m_target(target)
	{
	}

	void RHICommandRemoveRenderTarget::Execute(Memory::RefPtr<RHISystem> system)
	{
		if (system && m_target)
		{
			system->RemoveRenderTarget(m_target);
		}
	}

	RHICommandSetConstant::RHICommandSetConstant(const RHIConstDesc info, const int32_t binding)
		: m_info(info)
		, m_binding(binding)
	{
	}

	void RHICommandSetConstant::Execute(Memory::RefPtr<RHISystem> system)
	{
		if (system)
		{
			system->SetConstant(m_info, m_binding);
		}
	}

	RHICommandSetVertexBuffer::RHICommandSetVertexBuffer(const Memory::RefPtr<const RHIBuffer> buffer, const uint32_t bindingIndex, const uint32_t offset)
		: m_buffer(buffer)
		, m_bindingIndex(bindingIndex)
		, m_offset(offset)
	{
	}

	void RHICommandSetVertexBuffer::Execute(Memory::RefPtr<RHISystem> system)
	{
		if (system)
		{
			system->SetVertexBuffer(m_buffer, m_bindingIndex, m_offset);
		}
	}

	RHICommandSetIndexBuffer::RHICommandSetIndexBuffer(const Memory::RefPtr<const RHIBuffer> buffer, const uint32_t offset)
		: m_buffer(buffer)
		, m_offset(offset)
	{
	}

	void RHICommandSetIndexBuffer::Execute(Memory::RefPtr<RHISystem> system)
	{
		if (system)
		{
			system->SetIndexBuffer(m_buffer, m_offset);
		}
	}

	RHICommandSetBuffer::RHICommandSetBuffer(const Memory::RefPtr<const RHIBuffer> buffer, const int32_t binding)
		: m_buffer(buffer)
		, m_binding(binding)
	{
	}

	void RHICommandSetBuffer::Execute(Memory::RefPtr<RHISystem> system)
	{
		if (system)
		{
			system->SetBuffer(m_buffer, m_binding);
		}
	}

	RHICommandSetTexture::RHICommandSetTexture(const Memory::RefPtr<const RHITexture> texture, const int32_t binding)
		: m_texture(texture)
		, m_binding(binding)
	{
	}

	void RHICommandSetTexture::Execute(Memory::RefPtr<RHISystem> system)
	{
		if (system)
		{
			system->SetTexture(m_texture, m_binding);
		}
	}

	RHICommandSetSampler::RHICommandSetSampler(const Memory::RefPtr<const RHISampler> sampler, const int32_t binding)
		: m_sampler(sampler)
		, m_binding(binding)
	{
	}

	void RHICommandSetSampler::Execute(Memory::RefPtr<RHISystem> system)
	{
		if (system)
		{
			system->SetSampler(m_sampler, m_binding);
		}
	}

	RHICommandSetVertexLayout::RHICommandSetVertexLayout(const Memory::RefPtr<const RHIVertexLayout> layout)
		: m_layout(layout)
	{
	}

	void RHICommandSetVertexLayout::Execute(Memory::RefPtr<RHISystem> system)
	{
		if (system && m_layout)
		{
			system->SetVertexLayout(m_layout);
		}
	}

	RHICommandSetPipeLine::RHICommandSetPipeLine(const Memory::RefPtr<const RHIPipeLine> pipeline)
		: m_pipeline(pipeline)
	{
	}

	void RHICommandSetPipeLine::Execute(Memory::RefPtr<RHISystem> system)
	{
		if (system && m_pipeline)
		{
			system->SetPipeLine(m_pipeline);
		}
	}

	RHICommandBeginRenderTarget::RHICommandBeginRenderTarget(const Memory::RefPtr<const RHIRenderTarget> target)
		: m_target(target)
	{
	}

	void RHICommandBeginRenderTarget::Execute(Memory::RefPtr<RHISystem> system)
	{
		if (system && m_target)
		{
			system->BeginRenderTarget(m_target);
		}
	}

	RHICommandClearRenderTarget::RHICommandClearRenderTarget(const Memory::RefPtr<const RHIRenderTarget> target, const RHIClearState& state)
		: m_target(target)
		, m_state(state)
	{
	}

	void RHICommandClearRenderTarget::Execute(Memory::RefPtr<RHISystem> system)
	{
		if (system && m_target)
		{
			system->ClearRenderTarget(m_target, m_state);
		}
	}

	RHICommandUnsetBuffer::RHICommandUnsetBuffer(const Memory::RefPtr<const RHIBuffer> buffer, const int32_t binding)
		: m_buffer(buffer)
		, m_binding(binding)
	{
	}

	void RHICommandUnsetBuffer::Execute(Memory::RefPtr<RHISystem> system)
	{
		if (system)
		{
			system->UnsetBuffer(m_buffer, m_binding);
		}
	}

	RHICommandUnsetTexture::RHICommandUnsetTexture(const Memory::RefPtr<const RHITexture> texture, const int32_t binding)
		: m_texture(texture)
		, m_binding(binding)
	{
	}

	void RHICommandUnsetTexture::Execute(Memory::RefPtr<RHISystem> system)
	{
		if (system)
		{
			system->UnsetTexture(m_texture, m_binding);
		}
	}

	RHICommandUnsetSampler::RHICommandUnsetSampler(const Memory::RefPtr<const RHISampler> sampler, const int32_t binding)
		: m_sampler(sampler)
		, m_binding(binding)
	{
	}

	void RHICommandUnsetSampler::Execute(Memory::RefPtr<RHISystem> system)
	{
		if (system)
		{
			system->UnsetSampler(m_sampler, m_binding);
		}
	}

	RHICommandUnsetVertexBuffer::RHICommandUnsetVertexBuffer(const uint32_t bindingIndex)
		: m_bindingIndex(bindingIndex)
	{
	}

	void RHICommandUnsetVertexBuffer::Execute(Memory::RefPtr<RHISystem> system)
	{
		if (system)
		{
			system->UnsetVertexBuffer(m_bindingIndex);
		}
	}

	void RHICommandUnsetIndexBuffer::Execute(Memory::RefPtr<RHISystem> system)
	{
		if (system)
		{
			system->UnsetIndexBuffer();
		}
	}

	RHICommandUnsetVertexLayout::RHICommandUnsetVertexLayout()
	{
	}

	void RHICommandUnsetVertexLayout::Execute(Memory::RefPtr<RHISystem> system)
	{
		if (system)
		{
			system->UnsetVertexLayout();
		}
	}

	RHICommandUnsetPipeLine::RHICommandUnsetPipeLine()
	{
	}

	void RHICommandUnsetPipeLine::Execute(Memory::RefPtr<RHISystem> system)
	{
		if (system)
		{
			system->UnsetPipeLine();
		}
	}

	RHICommandEndRenderTarget::RHICommandEndRenderTarget()
	{
	}

	void RHICommandEndRenderTarget::Execute(Memory::RefPtr<RHISystem> system)
	{
		if (system)
		{
			system->EndRenderTarget();
		}
	}

	RHICommandDispatchCompute::RHICommandDispatchCompute(const RHIDispatchDesc info)
		: m_info(info)
	{
	}

	void RHICommandDispatchCompute::Execute(Memory::RefPtr<RHISystem> system)
	{
		if (system)
		{
			system->DispatchCompute(m_info);
		}
	}

	RHICommandDrawPrimitive::RHICommandDrawPrimitive(const RHIDrawDesc info)
		: m_info(info)
	{}

	void RHICommandDrawPrimitive::Execute(Memory::RefPtr<RHISystem> system)
	{
		if (system)
		{
			system->DrawPrimitive(m_info);
		}
	}

	RHICommandDrawIndirectPrimitive::RHICommandDrawIndirectPrimitive(const RHIDrawIndirectDesc info)
		: m_info(info)
	{}

	void RHICommandDrawIndirectPrimitive::Execute(Memory::RefPtr<RHISystem> system)
	{
		if (system)
		{
			system->DrawIndirectPrimitive(m_info);
		}
	}

}