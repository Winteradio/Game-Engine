#include <Renderer/RenderPass/DebugPass.h>

#include <Asset/AssetSystem.h>
#include <Asset/AssetTypes.h>

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
	DebugPass::DebugPass()
		: m_frustumShader()
	{
	}

	eResourceState DebugPass::GetResourceState() const
	{
		if (!m_frustumShader)
		{
			return eResourceState::eNone;
		}

		eResourceState allState = eResourceState::eAll;

		allState &= m_frustumShader->GetResourceState();

		return allState;
	}

	void DebugPass::Upload(Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList)
		{
			return;
		}
		
		if (!m_frustumShader)
		{
			Memory::RefPtr<ShaderAsset> frustumVS = Memory::Cast<ShaderAsset>(AssetSystem::Load("asset/shader/debug_frustum.vs.glsl"));
			Memory::RefPtr<ShaderAsset> frustumPS = Memory::Cast<ShaderAsset>(AssetSystem::Load("asset/shader/debug_frustum.ps.glsl"));

			m_frustumShader = Memory::MakeRef<ShaderProxy>();
			if (m_frustumShader)
			{
				m_frustumShader->AddShader(Memory::MakeRef<DebugVSState>(frustumVS));
				m_frustumShader->AddShader(Memory::MakeRef<DebugPSState>(frustumPS));
			}
		}
	}

	void DebugPass::Unload(Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList)
		{
			return;
		}

		m_frustumShader.Reset();
	}

	void DebugPass::InitState()
	{
		if (!m_depth)
		{
			m_depth = Memory::MakeRef<RHIDepthState>();
			m_depth->enable = true;
			m_depth->write = false;
		}

		if (!m_blend)
		{
			m_blend = Memory::MakeRef<RHIBlendState>();
			m_blend->enable = true;
			m_blend->colorOp = eBlendOp::eAdd;
			m_blend->srcColor = eBlendFunc::eOne;
			m_blend->srcAlpha = eBlendFunc::eOne;
			m_blend->destColor = eBlendFunc::eOne_Minus_Src_Alpha;
			m_blend->destAlpha = eBlendFunc::eOne_Minus_Src_Alpha;
		}

		if (!m_rasterizer)
		{
			m_rasterizer = Memory::MakeRef<RHIRasterizerState>();
			m_rasterizer->fillMode = ePrimitiveMode::eLine;
		}
	}

	bool DebugPass::Draw(const MeshDrawCommands& drawCommands, const LightProxies& lightProxies, Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList)
		{
			return false;
		}

		auto pipeline = GetPipeLine(cmdList, m_frustumShader);
		if (!pipeline || pipeline->GetState() != eResourceState::eReady)
		{
			return false;
		}

		const auto defaultLight = GlobalResource::GetDefaultLight();
		if (!defaultLight || defaultLight->GetResourceState() != eResourceState::eReady)
		{
			return false;
		}

		LightProxies lights = lightProxies;
		lights.PushBack(defaultLight);

		cmdList->SetPipeLine(pipeline);
		for (const auto& lightProxy : lights)
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

		return true;
	}

	RHIDrawIndexDesc DebugPass::GetDrawCommand(Memory::RefPtr<const MeshDrawCommand> drawCommand)
	{
		Memory::RefPtr<const RHIVertexLayout> layout = GlobalResource::GetCube();
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

	Memory::RefPtr<const RHIPipeLine> DebugPass::GetPipeLine(Memory::RefPtr<RHICommandList> cmdList, Memory::RefPtr<const ShaderProxy> shaderProxy)
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

		auto vsAsset = shaderProxy->GetShader<DebugVSState>();
		auto psAsset = shaderProxy->GetShader<DebugPSState>();

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

	bool DebugPass::SetCommand(Memory::RefPtr<RHICommandList> cmdList, Memory::RefPtr<const RHIPipeLine> pipeline, Memory::RefPtr<const MeshDrawCommand> drawCommand)
	{
		if (!cmdList)
		{
			return false;
		}

		const auto layout = GlobalResource::GetCube();
		if (!layout || layout->GetState() != eResourceState::eReady)
		{
			return false;
		}

		cmdList->SetVertexLayout(layout);

		return true;
	}

	void DebugPass::UnsetCommand(Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList)
		{
			return;
		}

		cmdList->UnsetVertexLayout();
	}

	bool DebugPass::SetLight(Memory::RefPtr<RHICommandList> cmdList, Memory::RefPtr<const RHIPipeLine> pipeline, Memory::RefPtr<const LightProxy> light)
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

		cmdList->SetBuffer(pipeline, cameraBuffer, eResourceSlot::eCamera);

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