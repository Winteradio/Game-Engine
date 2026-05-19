#include <Renderer/RenderPass/LightingPass.h>

#include <Renderer/GlobalRenderer.h>
#include <Renderer/ShaderState.h>
#include <Renderer/Proxy/ShaderProxy.h>
#include <Renderer/Proxy/LightProxy.h>

#include <RHI/RHICommandList.h>
#include <RHI/RHIDescriptions.h>
#include <RHI/RHIResources.h>

#include <Memory/include/Core.h>

namespace wtr
{
	LightingPass::LightingPass()
		: m_target(nullptr)
	{}

	eResourceState LightingPass::GetResourceState() const
	{
		return m_target ? m_target->GetState() : eResourceState::eNone;
	}

	void LightingPass::Upload(Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList)
		{
			return;
		}

		if (!m_clear)
		{
			m_clear = Memory::MakeRef<RHIClearState>();
		}

		if (!m_color)
		{
			m_color = Memory::MakeRef<RHIColorState>();
		}

		if (!m_depth)
		{
			m_depth = Memory::MakeRef<RHIDepthState>();
		}

		if (!m_stencil)
		{
			m_stencil = Memory::MakeRef<RHIStencilState>();
		}

		if (!m_blend)
		{
			m_blend = Memory::MakeRef<RHIBlendState>();
		}

		if (!m_rasterizer)
		{
			m_rasterizer = Memory::MakeRef<RHIRasterizerState>();
		}

		if (!m_target)
		{
			RHIRenderTargetCreateDesc desc;

			RHIColorAttachment position;
			position.slot = 0;
			position.type = eAttachment::eColor;
			position.texture = GlobalResource::GetGBuffer(eGBufferSlot::ePosition);
			if (!position.texture || position.texture->GetState() != eResourceState::eReady)
			{
				return;
			}

			RHIColorAttachment normal;
			normal.slot = 1;
			normal.type = eAttachment::eColor;
			normal.texture = GlobalResource::GetGBuffer(eGBufferSlot::eNormal);
			if (!normal.texture || normal.texture->GetState() != eResourceState::eReady)
			{
				return;
			}

			RHIColorAttachment albedo;
			albedo.slot = 2;
			albedo.type = eAttachment::eColor;
			albedo.texture = GlobalResource::GetGBuffer(eGBufferSlot::eAlbedo);
			if (!albedo.texture || albedo.texture->GetState() != eResourceState::eReady)
			{
				return;
			}

			RHIDepthStencilAttachment depthStencil;
			depthStencil.type = eAttachment::eDepthStencil;
			depthStencil.texture = GlobalResource::GetGBuffer(eGBufferSlot::eDepth);
			if (!depthStencil.texture || depthStencil.texture->GetState() != eResourceState::eReady)
			{
				return;
			}

			desc.colors = { position, normal, albedo };
			desc.depthStencil = depthStencil;

			Memory::RefPtr<RHIRenderTarget> target = cmdList->CreateRenderTarget(desc);
			if (target)
			{
				m_target = target;
			}
		}
	}

	void LightingPass::Unload(Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList)
		{
			return;
		}

		if (m_target)
		{
			cmdList->RemoveRenderTarget(m_target);
			m_target.Reset();
		}
	}

	void LightingPass::Draw(const MeshDrawCommands& drawCommands, const LightProxies& lightProxies, Memory::RefPtr<RHICommandList> cmdList)
	{
		if (lightProxies.Empty() || !cmdList)
		{
			return;
		}

		if (!m_target || m_target->GetState() != eResourceState::eReady)
		{
			return;
		}

		// TODO
	}

	bool LightingPass::SetCommand(Memory::RefPtr<RHICommandList> cmdList, Memory::RefPtr<const RHIPipeLine> pipeline, Memory::RefPtr<const MeshDrawCommand> drawCommand)
	{
		// TODO

		return true;
	}

	void LightingPass::UnsetCommand(Memory::RefPtr<RHICommandList> cmdList, Memory::RefPtr<const MeshDrawCommand> drawCommand)
	{
		if (!cmdList || !drawCommand)
		{
			return;
		}

		// TODO
	}

	const RHIDrawIndexDesc LightingPass::GetDrawCommand(Memory::RefPtr<const MeshDrawCommand> drawCommand)
	{
		Memory::RefPtr<const RHIVertexLayout> layout = GlobalResource::GetQuad();
		if (!layout)
		{
			return {};
		}

		const RHIVertexLayoutDesc& layoutDesc = layout->GetDesc();
		if (!layoutDesc.indexBuffer)
		{
			return {};
		}

		RHIDrawIndexDesc desc;
		desc.drawMode = eDrawMode::eTriangles;
		desc.indexType = layoutDesc.indexBuffer->GetComponentType();
		desc.indexCount = layoutDesc.indexBuffer->GetCount();
		desc.indexOffset = 0;
		desc.baseVertex = 0;
		desc.instanceCount = 1;
		desc.indirectDraw = false;

		return desc;
	}

	Memory::RefPtr<const RHIPipeLine> LightingPass::GetPipeLine(Memory::RefPtr<RHICommandList> cmdList, Memory::RefPtr<const ShaderProxy> shaderProxy)
	{
		if (!cmdList || !shaderProxy)
		{
			return {};
		}

		RHIPipeLineCreateDesc desc;
		desc.clear = m_clear;
		desc.color = m_color;
		desc.depth = m_depth;
		desc.stencil = m_stencil;
		desc.blend = m_blend;
		desc.rasterizer = m_rasterizer;

		auto vsAsset = shaderProxy->GetShader<LightVSState>();
		auto psAsset = shaderProxy->GetShader<LightPSState>();

		desc.vertexShader = vsAsset ? vsAsset->GetShader() : nullptr;
		desc.pixelShader = psAsset ? psAsset->GetShader() : nullptr;

		Memory::RefPtr<const RHIPipeLine> pipeline = GlobalResource::GetPipeLine(cmdList, desc);
		if (pipeline && pipeline->GetState() == eResourceState::eReady)
		{
			return pipeline;
		}
		else
		{
			return {};
		}
	}
}