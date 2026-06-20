#include <Renderer/RenderPass/ShadowPass.h>

#include <Renderer/GlobalRenderer.h>
#include <Renderer/ShaderState.h>
#include <Renderer/MeshDrawCommand.h>
#include <Renderer/Proxy/ShaderProxy.h>
#include <Renderer/Proxy/LightProxy.h>
#include <Renderer/Proxy/PrimitiveProxy.h>

#include <RHI/RHICommandList.h>
#include <RHI/RHIDescriptions.h>
#include <RHI/RHIResources.h>

#include <Memory/include/Core.h>

namespace wtr
{
	ShadowPass::ShadowPass()
		: m_lights()
	{
	}

	eResourceState ShadowPass::GetResourceState() const
	{
		// TODO
		return eResourceState::eReady;
	}

	void ShadowPass::Upload(Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList)
		{
			return;
		}
	}

	void ShadowPass::Unload(Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList)
		{
			return;
		}
	}

	void ShadowPass::InitState()
	{
		if (!m_clear)
		{
			m_clear = Memory::MakeRef<RHIClearState>();
			m_clear->clearBuffer = eClearBuffer::eDepth;
			m_clear->depth = 1.0f;
			m_clear->stencil = 0.f;
			m_clear->color = fvec4(0.0f, 0.0f, 0.0f, 0.f);
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
			m_rasterizer->cullEnable = true;
			m_rasterizer->cullFace = eCullFace::eBack;
		}
	}

	bool ShadowPass::Draw(const MeshDrawCommands& drawCommands, const LightProxies& lightProxies, Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList || !m_clear)
		{
			return false;
		}

		if (drawCommands.Empty())
		{
			return true;
		}

		const auto defaultLight = GlobalResource::GetDefaultLight();
		auto defaultPipeLine = GetPipeLine(cmdList, defaultLight);
		if (!defaultPipeLine || defaultPipeLine->GetState() != eResourceState::eReady)
		{
			return false;
		}

		m_lights.Clear();
		m_lights[defaultPipeLine].PushBack(defaultLight);

		for (const auto& lightProxy : lightProxies)
		{
			auto pipeline = GetPipeLine(cmdList, lightProxy);
			if (!pipeline || pipeline->GetState() != eResourceState::eReady)
			{
				continue;
			}

			m_lights[pipeline].PushBack(lightProxy);
		}

		SetState(cmdList);

		for (const auto& [pipeline, proxies] : m_lights)
		{
			if (!pipeline || pipeline->GetState() != eResourceState::eReady)
			{
				continue;
			}

			cmdList->SetPipeLine(pipeline);

			for (const auto& lightProxy : proxies)
			{
				if (!SetLight(cmdList, pipeline, lightProxy))
				{
					continue;
				}

				cmdList->Resize(0, 0, 1024, 1024);
				cmdList->SetRenderTarget(lightProxy->GetShadowTarget());
				cmdList->ClearRenderTarget(lightProxy->GetShadowTarget(), *m_clear);

				for (const auto& drawCommand : drawCommands)
				{
					if (!drawCommand)
					{
						continue;
					}

					if (SetCommand(cmdList, pipeline, drawCommand))
					{
						const auto drawDesc = GetDrawCommand(drawCommand);
						cmdList->DrawIndexPrimitive(drawDesc);
					}

					UnsetCommand(cmdList);
				}

				cmdList->UnsetRenderTarget();
			}

			cmdList->UnsetPipeLine();
		}

		return true;
	}

	bool ShadowPass::SetCommand(Memory::RefPtr<RHICommandList> cmdList, Memory::RefPtr<const RHIPipeLine> pipeline, Memory::RefPtr<const MeshDrawCommand> drawCommand)
	{
		if (!cmdList || !pipeline || !drawCommand)
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

	void ShadowPass::UnsetCommand(Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList)
		{
			return;
		}

		cmdList->UnsetVertexLayout();
	}

	bool ShadowPass::SetLight(Memory::RefPtr<RHICommandList> cmdList, Memory::RefPtr<const RHIPipeLine> pipeline, Memory::RefPtr<const LightProxy> light)
	{
		if (!cmdList || !pipeline || !light || light->GetResourceState() != eResourceState::eReady)
		{
			return false;
		}
		
		RHIConstDesc viewDesc;
		viewDesc.dataType = eDataType::eFloat;
		viewDesc.row = 4;
		viewDesc.column = 4;
		viewDesc.offset = 0;
		viewDesc.data = light->GetLightView();

		RHIConstDesc projDesc;
		projDesc.dataType = eDataType::eFloat;
		projDesc.row = 4;
		projDesc.column = 4;
		projDesc.offset = 0;
		projDesc.data = light->GetLightProj();

		cmdList->SetConstant(pipeline, viewDesc, "uLightView");
		cmdList->SetConstant(pipeline, projDesc, "uLightProj");

		return true;
	}

	RHIDrawIndexDesc ShadowPass::GetDrawCommand(Memory::RefPtr<const MeshDrawCommand> drawCommand)
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

	Memory::RefPtr<const RHIPipeLine> ShadowPass::GetPipeLine(Memory::RefPtr<RHICommandList> cmdList, Memory::RefPtr<const ShaderProxy> shaderProxy)
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
		
		auto vsAsset = shaderProxy->GetShader<ShadowVSState>();
		auto gsAsset = shaderProxy->GetShader<ShadowGSState>();
		auto psAsset = shaderProxy->GetShader<ShadowPSState>();
		
		desc.vertexShader = vsAsset ? vsAsset->GetShader() : nullptr;
		desc.geometryShader = gsAsset ? gsAsset->GetShader() : nullptr;
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