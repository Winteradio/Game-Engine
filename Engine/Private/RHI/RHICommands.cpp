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

	RHICommandInitializeVertexShader::RHICommandInitializeVertexShader(const RHIVertexShaderCreateDesc info, const Memory::RefPtr<RHIVertexShader> shader)
		: m_info(info)
		, m_shader(shader)
	{}

	void RHICommandInitializeVertexShader::Execute(Memory::RefPtr<RHISystem> system)
	{
		if (system && m_shader)
		{
			system->InitializeVertexShader(m_info, m_shader);
		}
	}

	RHICommandInitializeGeometryShader::RHICommandInitializeGeometryShader(const RHIGeometryShaderCreateDesc info, const Memory::RefPtr<RHIGeometryShader> shader)
		: m_info(info)
		, m_shader(shader)
	{}

	void RHICommandInitializeGeometryShader::Execute(Memory::RefPtr<RHISystem> system)
	{
		if (system && m_shader)
		{
			system->InitializeGeometryShader(m_info, m_shader);
		}
	}

	RHICommandInitializeHullShader::RHICommandInitializeHullShader(const RHIHullShaderCreateDesc info, const Memory::RefPtr<RHIHullShader> shader)
		: m_info(info)
		, m_shader(shader)
	{
	}

	void RHICommandInitializeHullShader::Execute(Memory::RefPtr<RHISystem> system)
	{
		if (system && m_shader)
		{
			system->InitializeHullShader(m_info, m_shader);
		}
	}

	RHICommandInitializePixelShader::RHICommandInitializePixelShader(const RHIPixelShaderCreateDesc info, const Memory::RefPtr<RHIPixelShader> shader)
		: m_info(info)
		, m_shader(shader)
	{}

	void RHICommandInitializePixelShader::Execute(Memory::RefPtr<RHISystem> system)
	{
		if (system && m_shader)
		{
			system->InitializePixelShader(m_info, m_shader);
		}
	}

	RHICommandInitializeComputeShader::RHICommandInitializeComputeShader(const RHIComputeShaderCreateDesc info, const Memory::RefPtr<RHIComputeShader> shader)
		: m_info(info)
		, m_shader(shader)
	{}

	void RHICommandInitializeComputeShader::Execute(Memory::RefPtr<RHISystem> system)
	{
		if (system && m_shader)
		{
			system->InitializeComputeShader(m_info, m_shader);
		}
	}

	RHICommandInitializePipeLine::RHICommandInitializePipeLine(const RHIPipeLineCreateDesc info, const Memory::RefPtr<RHIPipeLine> shader)
		: m_info(info)
		, m_shader(shader)
	{}

	void RHICommandInitializePipeLine::Execute(Memory::RefPtr<RHISystem> system)
	{
		if (system && m_shader)
		{
			system->InitializePipeLine(m_info, m_shader);
		}
	}

	RHICommandUpdateBuffer::RHICommandUpdateBuffer(const RHIBufferCreateDesc info, const Memory::RefPtr<RHIBuffer> buffer)
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

	RHICommandUpdateTexture::RHICommandUpdateTexture(const RHITextureCreateDesc info, const Memory::RefPtr<RHITexture> texture)
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