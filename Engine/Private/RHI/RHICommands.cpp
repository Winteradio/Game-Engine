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

	void RHICommandFlush::Execute(Memory::RefPtr<RHISystem> system)
	{
		if (system)
		{
			system->Flush();
		}
	}

	void RHICommandPresent::Execute(Memory::RefPtr<RHISystem> system)
	{
		if (system)
		{
			system->Present();
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

	RHICommandColorState::RHICommandColorState(const RHIColorState& state)
		: m_state(state)
	{}

	void RHICommandColorState::Execute(Memory::RefPtr<RHISystem> system)
	{
		if (system)
		{
			system->SetColorState(m_state);
		}
	}

	RHICommandDepthState::RHICommandDepthState(const RHIDepthState& state)
		: m_state(state)
	{}

	void RHICommandDepthState::Execute(Memory::RefPtr<RHISystem> system)
	{
		if (system)
		{
			system->SetDepthState(m_state);
		}
	}

	RHICommandStencilState::RHICommandStencilState(const RHIStencilState& state)
		: m_state(state)
	{}

	void RHICommandStencilState::Execute(Memory::RefPtr<RHISystem> system)
	{
		if (system)
		{
			system->SetStencilState(m_state);
		}
	}

	RHICommandBlendState::RHICommandBlendState(const RHIBlendState& state)
		: m_state(state)
	{}

	void RHICommandBlendState::Execute(Memory::RefPtr<RHISystem> system)
	{
		if (system)
		{
			system->SetBlendState(m_state);
		}
	}

	RHICommandRasterizerState::RHICommandRasterizerState(const RHIRasterizerState& state)
		: m_state(state)
	{}

	void RHICommandRasterizerState::Execute(Memory::RefPtr<RHISystem> system)
	{
		if (system)
		{
			system->SetRasterizerState(m_state);
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

	RHICommandSetConstant::RHICommandSetConstant(const Memory::RefPtr<const RHIPipeLine> pipeline, const RHIConstDesc info, const std::string& slotName)
		: m_pipeline(pipeline)
		, m_info(info)
		, m_slotName(slotName)
	{
	}

	void RHICommandSetConstant::Execute(Memory::RefPtr<RHISystem> system)
	{
		if (system)
		{
			system->SetConstant(m_pipeline, m_info, m_slotName);
		}
	}

	RHICommandSetBuffer::RHICommandSetBuffer(const Memory::RefPtr<const RHIPipeLine> pipeline, const Memory::RefPtr<const RHIBuffer> buffer, const std::string& slotName)
		: m_pipeline(pipeline)
		, m_buffer(buffer)
		, m_slotName(slotName)
	{
	}

	void RHICommandSetBuffer::Execute(Memory::RefPtr<RHISystem> system)
	{
		if (system)
		{
			system->SetBuffer(m_pipeline, m_buffer, m_slotName);
		}
	}

	RHICommandSetTexture::RHICommandSetTexture(const Memory::RefPtr<const RHIPipeLine> pipeline, const Memory::RefPtr<const RHITexture> texture, const std::string& slotName)
		: m_pipeline(pipeline)
		, m_texture(texture)
		, m_slotName(slotName)
	{
	}

	void RHICommandSetTexture::Execute(Memory::RefPtr<RHISystem> system)
	{
		if (system)
		{
			system->SetTexture(m_pipeline, m_texture, m_slotName);
		}
	}

	RHICommandSetSampler::RHICommandSetSampler(const Memory::RefPtr<const RHIPipeLine> pipeline, const Memory::RefPtr<const RHISampler> sampler, const std::string& slotName)
		: m_pipeline(pipeline)
		, m_sampler(sampler)
		, m_slotName(slotName)
	{
	}

	void RHICommandSetSampler::Execute(Memory::RefPtr<RHISystem> system)
	{
		if (system)
		{
			system->SetSampler(m_pipeline, m_sampler, m_slotName);
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

	RHICommandSetRenderTarget::RHICommandSetRenderTarget(const Memory::RefPtr<const RHIRenderTarget> target)
		: m_target(target)
	{
	}

	void RHICommandSetRenderTarget::Execute(Memory::RefPtr<RHISystem> system)
	{
		if (system && m_target)
		{
			system->SetRenderTarget(m_target);
		}
	}

	RHICommandUnsetConstant::RHICommandUnsetConstant(const Memory::RefPtr<const RHIPipeLine> pipeline, const std::string& slotName)
		: m_pipeline(pipeline)
		, m_slotName(slotName)
	{
	}

	void RHICommandUnsetConstant::Execute(Memory::RefPtr<RHISystem> system)
	{
		if (system)
		{
			system->UnsetConstant(m_pipeline, m_slotName);
		}
	}

	RHICommandUnsetBuffer::RHICommandUnsetBuffer(const Memory::RefPtr<const RHIPipeLine> pipeline, const Memory::RefPtr<const RHIBuffer> buffer, const std::string& slotName)
		: m_pipeline(pipeline)
		, m_buffer(buffer)
		, m_slotName(slotName)
	{
	}

	void RHICommandUnsetBuffer::Execute(Memory::RefPtr<RHISystem> system)
	{
		if (system)
		{
			system->UnsetBuffer(m_pipeline, m_buffer, m_slotName);
		}
	}

	RHICommandUnsetTexture::RHICommandUnsetTexture(const Memory::RefPtr<const RHIPipeLine> pipeline, const Memory::RefPtr<const RHITexture> texture, const std::string& slotName)
		: m_pipeline(pipeline)
		, m_texture(texture)
		, m_slotName(slotName)
	{
	}

	void RHICommandUnsetTexture::Execute(Memory::RefPtr<RHISystem> system)
	{
		if (system)
		{
			system->UnsetTexture(m_pipeline, m_texture, m_slotName);
		}
	}

	RHICommandUnsetSampler::RHICommandUnsetSampler(const Memory::RefPtr<const RHIPipeLine> pipeline, const Memory::RefPtr<const RHISampler> sampler, const std::string& slotName)
		: m_pipeline(pipeline)
		, m_sampler(sampler)
		, m_slotName(slotName)
	{
	}

	void RHICommandUnsetSampler::Execute(Memory::RefPtr<RHISystem> system)
	{
		if (system)
		{
			system->UnsetSampler(m_pipeline, m_sampler, m_slotName);
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

	RHICommandUnsetRenderTarget::RHICommandUnsetRenderTarget()
	{
	}

	void RHICommandUnsetRenderTarget::Execute(Memory::RefPtr<RHISystem> system)
	{
		if (system)
		{
			system->UnsetRenderTarget();
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

	RHICommandDrawIndexPrimitive::RHICommandDrawIndexPrimitive(const RHIDrawIndexDesc info)
		: m_info(info)
	{}

	void RHICommandDrawIndexPrimitive::Execute(Memory::RefPtr<RHISystem> system)
	{
		if (system)
		{
			system->DrawIndexPrimitive(m_info);
		}
	}

}