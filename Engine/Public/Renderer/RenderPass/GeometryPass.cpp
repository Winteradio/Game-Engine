#include <Renderer/RenderPass/GeometryPass.h>

#include <Asset/AssetTypes.h>

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

		if (!m_target)
		{
			RHIRenderTargetCreateDesc desc;

			RHIColorAttachment normal;
			normal.slot = 0;
			normal.type = eAttachment::eColor;
			normal.texture = GlobalResource::GetGBuffer(eGBufferSlot::eNormal);
			if (!normal.texture || normal.texture->GetState() != eResourceState::eReady)
			{
				return;
			}

			RHIColorAttachment albedo;
			albedo.slot = 1;
			albedo.type = eAttachment::eColor;
			albedo.texture = GlobalResource::GetGBuffer(eGBufferSlot::eAlbedo);
			if (!albedo.texture || albedo.texture->GetState() != eResourceState::eReady)
			{
				return;
			}

			RHIColorAttachment phong;
			phong.slot = 2;
			phong.type = eAttachment::eColor;
			phong.texture = GlobalResource::GetGBuffer(eGBufferSlot::ePhong);
			if (!phong.texture || phong.texture->GetState() != eResourceState::eReady)
			{
				return;
			}

			// TODO - PBR
			//RHIColorAttachment param;
			//param.slot = 2;
			//param.type = eAttachment::eColor;
			//param.texture = GlobalResource::GetGBuffer(eGBufferSlot::eParam);
			//if (!param.texture || param.texture->GetState() != eResourceState::eReady)
			//{
			//	return;
			//}

			RHIDepthStencilAttachment depthStencil;
			depthStencil.type = eAttachment::eDepthStencil;
			depthStencil.texture = GlobalResource::GetGBuffer(eGBufferSlot::eDepth);
			if (!depthStencil.texture || depthStencil.texture->GetState() != eResourceState::eReady)
			{
				return;
			}

			desc.colors = { normal, albedo, phong };
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

	void GeometryPass::InitState()
	{
		if (!m_clear)
		{
			m_clear = Memory::MakeRef<RHIClearState>();
			m_clear->clearBuffer = eClearBuffer::eColor | eClearBuffer::eDepth | eClearBuffer::eStencil;
			m_clear->depth = 1.0f;
			m_clear->stencil = 0.f;
			m_clear->color = fvec4(0.2f, 0.2f, 0.2f, 1.f);
		}

		if (!m_depth)
		{
			m_depth = Memory::MakeRef<RHIDepthState>();
			m_depth->enable = true;
			m_depth->write = true;
			m_depth->func = eCompareFunc::eLessEqual;
		}

		if (!m_rasterizer)
		{
			m_rasterizer = Memory::MakeRef<RHIRasterizerState>();
			m_rasterizer->cullEnable = false;
		}

		if (!m_blend)
		{
			m_blend = Memory::MakeRef<RHIBlendState>();
			m_blend->enable = false;
		}
	}

	bool GeometryPass::Draw(const MeshDrawCommands& drawCommands, const LightProxies& lightProxies, Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList || !m_clear || !m_target || m_target->GetState() != eResourceState::eReady)
		{
			return false;
		}

		if (drawCommands.Empty())
		{
			return true;
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
		SetState(cmdList);
		cmdList->ClearRenderTarget(m_target, *m_clear);

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

				UnsetCommand(cmdList);
			}

			cmdList->UnsetPipeLine();
		}

		cmdList->UnsetRenderTarget();

		return true;
	}

	bool GeometryPass::SetCommand(Memory::RefPtr<RHICommandList> cmdList, Memory::RefPtr<const RHIPipeLine> pipeline, Memory::RefPtr<const MeshDrawCommand> drawCommand)
	{
		if (!cmdList || !pipeline || !drawCommand)
		{
			return false;
		}

		if (!SetMaterial(cmdList, pipeline, drawCommand->material))
		{
			return false;
		}

		if (drawCommand->indirectDraw)
		{
			const auto indirectBuffer = drawCommand->indirect;
			const auto visibleBuffer = drawCommand->visible;
			if (!indirectBuffer || indirectBuffer->GetState() != eResourceState::eReady || 
				!visibleBuffer || visibleBuffer->GetState() != eResourceState::eReady)
			{
				return false;
			}

			cmdList->SetBuffer(pipeline, indirectBuffer, eResourceSlot::eIndirect);
			cmdList->SetBuffer(pipeline, visibleBuffer, eResourceSlot::eVisible);
		}

		const auto cameraBuffer = GlobalResource::GetCamera();
		const auto transformBuffer = drawCommand->transform;
		if (!cameraBuffer || cameraBuffer->GetState() != eResourceState::eReady ||
			!transformBuffer || transformBuffer->GetState() != eResourceState::eReady)
		{
			return false;
		}

		const auto vertexLayout = drawCommand->vertexLayout;
		if (!vertexLayout || vertexLayout->GetState() != eResourceState::eReady)
		{
			return false;
		}

		cmdList->SetBuffer(pipeline, cameraBuffer, eResourceSlot::eCamera);
		cmdList->SetBuffer(pipeline, transformBuffer, eResourceSlot::eTransform);
		cmdList->SetVertexLayout(drawCommand->vertexLayout);

		return true;
	}

	bool GeometryPass::SetMaterial(Memory::RefPtr<RHICommandList> cmdList, Memory::RefPtr<const RHIPipeLine> pipeline, Memory::RefPtr<const MaterialProxy> material)
	{
		if (!cmdList || !pipeline || !material || material->GetResourceState() != eResourceState::eReady)
		{
			return false;
		}

		const auto& materialDesc = material->GetMaterialDesc();
		if (materialDesc.hasTransparent)
		{
			return false;
		}

		const auto materialAsset = material->GetMaterial();
		if (materialAsset)
		{
			for (const auto& [slot, textureAsset] : materialAsset->textures)
			{
				if (!textureAsset || textureAsset->GetResourceState() != eResourceState::eReady)
				{
					continue;
				}

				const auto sampler = GlobalResource::GetSampler(cmdList, slot);
				if (!sampler || sampler->GetState() != eResourceState::eReady)
				{
					continue;
				}

				cmdList->SetTexture(pipeline, textureAsset->texture, slot);
				cmdList->SetSampler(pipeline, sampler, slot);
			}
		}

		const auto materialBuffer = material->GetMaterialBuffer();
		if (materialBuffer)
		{
			cmdList->SetBuffer(pipeline, materialBuffer, eResourceSlot::eMaterial);
		}

		return true;
	}

	void GeometryPass::UnsetCommand(Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList)
		{
			return;
		}

		cmdList->UnsetVertexLayout();
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