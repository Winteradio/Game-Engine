#include <Renderer/RenderPass/GeometryPass.h>

#include <Renderer/GlobalRenderer.h>
#include <Renderer/MeshDrawCommand.h>
#include <Renderer/Proxy/MaterialProxy.h>

#include <RHI/RHICommandList.h>
#include <RHI/RHIDescriptions.h>
#include <RHI/RHIResources.h>
#include <RHI/RHISystem.h>

#include <Memory/include/Core.h>

namespace wtr
{
	GeometryPass::GeometryPass()
		: m_target(nullptr)
	{
	}

	eResourceState GeometryPass::GetResourceState() const
	{
		return m_target ? m_target->GetState() : eResourceState::eNone;
	}

	void GeometryPass::Upload(Memory::RefPtr<RHICommandList> cmdList)
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

	void GeometryPass::Unload(Memory::RefPtr<RHICommandList> cmdList)
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

	void GeometryPass::Draw(const MeshDrawCommands& drawCommands, const LightProxies& lightProxies, Memory::RefPtr<RHICommandList> cmdList)
	{
		if (drawCommands.Empty() || !cmdList)
		{
			return;
		}

		if (!m_target || m_target->GetState() != eResourceState::eReady)
		{
			return;
		}

		m_commands.Clear();

		for (const auto& drawCommand : drawCommands)
		{
			if (!drawCommand)
			{
				continue;
			}

			auto pipeline = GetPipeLine(cmdList, drawCommand->material);
			if (!pipeline || pipeline->GetState() != eResourceState::eReady)
			{
				continue;
			}

			m_commands[pipeline].PushBack(drawCommand);
		}

		cmdList->SetRenderTarget(m_target);

		for (const auto& [pipeline, commands] : m_commands)
		{
			if (!pipeline || pipeline->GetState() != eResourceState::eReady)
			{
				continue;
			}

			cmdList->SetPipeLine(pipeline);

			for (const auto& command : commands)
			{
				if (!command)
				{
					continue;
				}


				if (SetCommand(cmdList, pipeline, command))
				{
					const auto drawDesc = GetDrawCommand(command);
					cmdList->DrawIndexPrimitive(drawDesc);
				}

				UnsetCommand(cmdList, command);
			}

			cmdList->UnsetPipeLine(pipeline);
		}

		cmdList->UnsetRenderTarget(m_target);
	}

	bool GeometryPass::SetCommand(Memory::RefPtr<RHICommandList> cmdList, Memory::RefPtr<const RHIPipeLine> pipeline, Memory::RefPtr<const MeshDrawCommand> drawCommand)
	{
		if (!cmdList || !pipeline || !drawCommand)
		{
			return false;
		}

		if (drawCommand->indirectDraw)
		{
			const RHIResourceBinding indirect = pipeline->GetBindingSlot("uIndirect");
			const RHIResourceBinding visible = pipeline->GetBindingSlot("uVisible");

			const auto indirectBuffer = drawCommand->indirect;
			const auto visibleBuffer = drawCommand->visible;
			if (!indirectBuffer || indirectBuffer->GetState() != eResourceState::eReady || 
				!visibleBuffer || visibleBuffer->GetState() != eResourceState::eReady)
			{
				return false;
			}

			cmdList->SetBuffer(indirectBuffer, indirect.location);
			cmdList->SetBuffer(visibleBuffer, visible.location);
		}

		const RHIResourceBinding camera = pipeline->GetBindingSlot("uCamera");
		const RHIResourceBinding transform = pipeline->GetBindingSlot("uTransform");

		if (camera.location == -1 || transform.location == -1)
		{
			return false;
		}

		const auto cameraBuffer = GlobalResource::GetCamera();
		const auto transformBuffer = drawCommand->transform;
		if (!cameraBuffer || cameraBuffer->GetState() != eResourceState::eReady ||
			!transformBuffer || transformBuffer->GetState() != eResourceState::eReady)
		{
			return false;
		}

		cmdList->SetBuffer(cameraBuffer, camera.location);
		cmdList->SetBuffer(transformBuffer, transform.location);

		cmdList->SetVertexLayout(drawCommand->vertexLayout);

		return true;
	}

	void GeometryPass::UnsetCommand(Memory::RefPtr<RHICommandList> cmdList, Memory::RefPtr<const MeshDrawCommand> drawCommand)
	{
		if (!cmdList || !drawCommand)
		{
			return;
		}

		cmdList->UnsetVertexLayout(drawCommand->vertexLayout);
	}

	const RHIDrawIndexDesc GeometryPass::GetDrawCommand(Memory::RefPtr<const MeshDrawCommand> drawCommand)
	{
		if (!drawCommand)
		{
			return {};
		}

		RHIDrawIndexDesc desc;
		desc.drawMode = drawCommand->drawMode;
		desc.indexType = drawCommand->indexType;
		desc.indexCount = drawCommand->indexCount;
		desc.indexOffset = drawCommand->indexOffset;
		desc.baseVertex = drawCommand->minVertexIndex;
		desc.instanceCount = drawCommand->instanceCount;
		desc.indirectDraw = drawCommand->indirectDraw;

		return desc;
	}

	Memory::RefPtr<const RHIPipeLine> GeometryPass::GetPipeLine(Memory::RefPtr<RHICommandList> cmdList, Memory::RefPtr<const ShaderProxy> shaderProxy)
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
		
		auto vsAsset = shaderProxy->GetShader<GeometryVSState>();
		auto gsAsset = shaderProxy->GetShader<GeometryGSState>();
		auto hsAsset = shaderProxy->GetShader<GeometryHSState>();
		auto dsAsset = shaderProxy->GetShader<GeometryDSState>();
		auto psAsset = shaderProxy->GetShader<GeometryPSState>();

		desc.vertexShader = vsAsset ? vsAsset->GetShader() : nullptr;
		desc.geometryShader = gsAsset ? gsAsset->GetShader() : nullptr;
		desc.hullShader = hsAsset ? hsAsset->GetShader() : nullptr;
		desc.domainShader = dsAsset ? dsAsset->GetShader() : nullptr;
		desc.pixelShader = psAsset ? psAsset->GetShader() : nullptr;

		Memory::RefPtr<const RHIPipeLine> pipeline = GlobalResource::GetPipeLine(cmdList, desc);
		if (!pipeline || pipeline->GetState() != eResourceState::eReady)
		{
			return {};
		}
		else
		{
			return pipeline;
		}
	}
}