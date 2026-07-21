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
		: m_lights()
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
			m_clear->color = fvec4(0.0f, 0.0f, 0.0f, 0.f);
		}
	}

	bool LightingPass::Draw(const MeshDrawCommands& drawCommands, const LightProxies& lightProxies, Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList)
		{
			return false;
		}

		m_lights.Clear();

		cmdList->Clear(*m_clear);
		RHIBlendState blendState;
		blendState.enable = false;

		const auto defaultLight = GlobalResource::GetDefaultLight();
		auto directionalPipeline = GetPipeLine(cmdList, defaultLight);
		if (!directionalPipeline || directionalPipeline->GetState() != eResourceState::eReady ||
			!defaultLight || defaultLight->GetResourceState() != eResourceState::eReady)
		{
			return false;
		}

		cmdList->SetPipeLine(directionalPipeline);

		if (SetLight(cmdList, directionalPipeline, defaultLight) && SetCommand(cmdList, directionalPipeline, nullptr))
		{
			const auto drawDesc = GetDrawCommand(nullptr);
			cmdList->DrawIndexPrimitive(drawDesc);
		}

		UnsetCommand(cmdList);

		cmdList->UnsetPipeLine();

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

	RHIDrawIndexDesc LightingPass::GetDrawCommand(Memory::RefPtr<const MeshDrawCommand> drawCommand)
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

		const auto cameraBuffer = GlobalResource::GetCamera();
		if (!cameraBuffer || cameraBuffer->GetState() != eResourceState::eReady)
		{
			return false;
		}

		const auto depthTexture = GlobalResource::GetGBuffer(eGBufferSlot::eDepth);
		const auto normalTexture = GlobalResource::GetGBuffer(eGBufferSlot::eNormal);
		const auto albedoTexture = GlobalResource::GetGBuffer(eGBufferSlot::eAlbedo);
		const auto phongTexture = GlobalResource::GetGBuffer(eGBufferSlot::ePhong);
		const auto shadowTexture = light->GetShadowMap();
		if (!depthTexture || depthTexture->GetState() != eResourceState::eReady ||
			!normalTexture || normalTexture->GetState() != eResourceState::eReady ||
			!albedoTexture || albedoTexture->GetState() != eResourceState::eReady ||
			!phongTexture || phongTexture->GetState() != eResourceState::eReady ||
			!shadowTexture || shadowTexture->GetState() != eResourceState::eReady)
		{
			return false;
		}

		const auto depthSampler = GlobalResource::GetSampler(cmdList, eResourceSlot::eGDepth);
		const auto normalSampler = GlobalResource::GetSampler(cmdList, eResourceSlot::eGNormal);
		const auto albedoSampler = GlobalResource::GetSampler(cmdList, eResourceSlot::eGAlbedo);
		const auto phongSampler = GlobalResource::GetSampler(cmdList, eResourceSlot::eGPhong);
		const auto shadowSampler = GlobalResource::GetSampler(cmdList, eResourceSlot::eGShadow);
		if (!depthSampler || depthSampler->GetState() != eResourceState::eReady ||
			!normalSampler || normalSampler->GetState() != eResourceState::eReady ||
			!albedoSampler || albedoSampler->GetState() != eResourceState::eReady ||
			!phongSampler || phongSampler->GetState() != eResourceState::eReady ||
			!shadowSampler || shadowSampler->GetState() != eResourceState::eReady)
		{
			return false;
		}

		const auto lightBuffer = light->GetLightBuffer();
		if (!lightBuffer || lightBuffer->GetState() != eResourceState::eReady)
		{
			return false;
		}

		cmdList->SetBuffer(pipeline, cameraBuffer, eResourceSlot::eCamera);
		cmdList->SetBuffer(pipeline, lightBuffer, eResourceSlot::eLight);

		cmdList->SetTexture(pipeline, depthTexture, eResourceSlot::eGDepth);
		cmdList->SetSampler(pipeline, depthSampler, eResourceSlot::eGDepth);

		cmdList->SetTexture(pipeline, normalTexture, eResourceSlot::eGNormal);
		cmdList->SetSampler(pipeline, normalSampler, eResourceSlot::eGNormal);

		cmdList->SetTexture(pipeline, albedoTexture, eResourceSlot::eGAlbedo);
		cmdList->SetSampler(pipeline, albedoSampler, eResourceSlot::eGAlbedo);

		cmdList->SetTexture(pipeline, phongTexture, eResourceSlot::eGPhong);
		cmdList->SetSampler(pipeline, phongSampler, eResourceSlot::eGPhong);

		cmdList->SetTexture(pipeline, shadowTexture, eResourceSlot::eGShadow);
		cmdList->SetSampler(pipeline, shadowSampler, eResourceSlot::eGShadow);

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
}