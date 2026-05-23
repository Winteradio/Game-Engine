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
	{}

	eResourceState LightingPass::GetResourceState() const
	{
		// TODO
		return eResourceState::eReady;
	}

	void LightingPass::Upload(Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList)
		{
			return;
		}
	}

	void LightingPass::Unload(Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList)
		{
			return;
		}
	}

	void LightingPass::InitState()
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
			m_depth->enable = false;
		}
	}

	bool LightingPass::Draw(const MeshDrawCommands& drawCommands, const LightProxies& lightProxies, Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList)
		{
			return false;
		}

		m_lights.Clear();

		if (lightProxies.Empty())
		{
			const auto lightProxy = GlobalResource::GetDefaultLight();
			auto pipeline = GetPipeLine(cmdList, lightProxy);
			if (pipeline && pipeline->GetState() == eResourceState::eReady)
			{
				m_lights[pipeline].PushBack(lightProxy);
			}
		}
		else
		{
			for (const auto& lightProxy : lightProxies)
			{
				if (!lightProxy)
				{
					continue;
				}

				auto pipeline = GetPipeLine(cmdList, lightProxy);
				if (!pipeline || pipeline->GetState() != eResourceState::eReady)
				{
					continue;
				}

				m_lights[pipeline].PushBack(lightProxy);
			}
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
				if (!lightProxy)
				{
					continue;
				}

				if (SetLight(cmdList, pipeline, lightProxy) && SetCommand(cmdList, pipeline, nullptr))
				{
					const auto drawDesc = GetDrawCommand(nullptr);
					cmdList->DrawIndexPrimitive(drawDesc);
				}

				UnsetCommand(cmdList);
			}

			cmdList->UnsetPipeLine();
		}

		return true;
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

	bool LightingPass::SetCommand(Memory::RefPtr<RHICommandList> cmdList, Memory::RefPtr<const RHIPipeLine> pipeline, Memory::RefPtr<const MeshDrawCommand> drawCommand)
	{
		if (!cmdList)
		{
			return false;
		}

		const auto layout = GlobalResource::GetQuad();
		if (!layout || layout->GetState() != eResourceState::eReady)
		{
			return false;
		}

		cmdList->SetVertexLayout(layout);

		return true;
	}

	void LightingPass::UnsetCommand(Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList)
		{
			return;
		}

		cmdList->UnsetVertexLayout();
	}

	bool LightingPass::SetLight(Memory::RefPtr<RHICommandList> cmdList, Memory::RefPtr<const RHIPipeLine> pipeline, Memory::RefPtr<const LightProxy> light)
	{
		if (!cmdList || !pipeline || !light || light->GetResourceState() != eResourceState::eReady)
		{
			return false;
		}

		const auto positionTexture = GlobalResource::GetGBuffer(eGBufferSlot::ePosition);
		const auto normalTexture = GlobalResource::GetGBuffer(eGBufferSlot::eNormal);
		const auto albedoTexture = GlobalResource::GetGBuffer(eGBufferSlot::eAlbedo);
		if (!positionTexture || positionTexture->GetState() != eResourceState::eReady ||
			!normalTexture || normalTexture->GetState() != eResourceState::eReady ||
			!albedoTexture || albedoTexture->GetState() != eResourceState::eReady)
		{
			return false;
		}

		const auto positionSampler = GlobalResource::GetSampler(cmdList, eResourceSlot::eGPosition);
		const auto normalSampler = GlobalResource::GetSampler(cmdList, eResourceSlot::eGNormal);
		const auto albedoSampler = GlobalResource::GetSampler(cmdList, eResourceSlot::eGAlbedo);
		if (!positionSampler || positionSampler->GetState() != eResourceState::eReady ||
			!normalSampler || normalSampler->GetState() != eResourceState::eReady ||
			!albedoSampler || albedoSampler->GetState() != eResourceState::eReady)
		{
			return false;
		}

		const auto lightBuffer = light->GetLightBuffer();
		if (!lightBuffer || lightBuffer->GetState() != eResourceState::eReady)
		{
			return false;
		}

		cmdList->SetBuffer(pipeline, lightBuffer, eResourceSlot::eLight);
		cmdList->SetTexture(pipeline, positionTexture, eResourceSlot::eGPosition);
		cmdList->SetSampler(pipeline, positionSampler, eResourceSlot::eGPosition);
		cmdList->SetTexture(pipeline, normalTexture, eResourceSlot::eGNormal);
		cmdList->SetSampler(pipeline, normalSampler, eResourceSlot::eGNormal);
		cmdList->SetTexture(pipeline, albedoTexture, eResourceSlot::eGAlbedo);
		cmdList->SetSampler(pipeline, albedoSampler, eResourceSlot::eGAlbedo);

		return true;
	}
}