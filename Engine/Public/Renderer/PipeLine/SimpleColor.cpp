#include <Renderer/PipeLine/SimpleColor.h>

#include <Asset/AssetTypes.h>
#include <Asset/AssetSystem.h>
#include <Renderer/MeshBatch.h>
#include <Renderer/GlobalResource.h>
#include <RHI/RHICommandList.h>
#include <RHI/RHIDescriptions.h>
#include <RHI/RHIResources.h>
#include <RHI/RHIStates.h>

#include <Log/include/Log.h>
#include <Memory/include/Core.h>

namespace wtr
{
	SimpleColor::SimpleColor()
		: PipeLine()
		, m_vertexShader(nullptr)
		, m_pixelShader(nullptr)
	{}

	SimpleColor::~SimpleColor()
	{}

	void SimpleColor::Init()
	{
		const std::string vertexShader = "asset/shader/SimpleColor.vs.glsl";
		const std::string pixelShader = "asset/shader/SimpleColor.ps.glsl";

		m_vertexShader = Memory::Cast<const ShaderAsset>(AssetSystem::Load(vertexShader));
		if (!m_vertexShader)
		{
			LOGERROR() << "[SimpleColor] Failed to initialize the simple color pipeline, cause failed to load the vertex shader, path : " << vertexShader;
			return;
		}

		m_pixelShader = Memory::Cast<const ShaderAsset>(AssetSystem::Load(pixelShader));
		if (!m_pixelShader)
		{
			LOGERROR() << "[SimpleColor] Failed to initialize the simple color pipeline, cause failed to load the pixel shader, path : " << pixelShader;
			return;
		}
	}

	void SimpleColor::Prepare()
	{
		if (!m_pipeLine)
		{
			return;
		}

		if (!m_pipeLine->HasSlot("CameraData") || !m_pipeLine->HasSlot("InstanceData"))
		{
			LOGERROR() << "[SimpleColor] Failed to prepare the simple color pipeline, cause the pipeline is missing required resource slots";
			return;
		}

		const RHIResourceBinding& cameraSlot = m_pipeLine->GetBindingSlot("CameraData");
		const RHIResourceBinding& instanceSlot = m_pipeLine->GetBindingSlot("InstanceData");

		m_cameraSlot = cameraSlot.location;
		m_instanceSlot = instanceSlot.location;

		m_prepared = true;
	}

	void SimpleColor::Draw(const MeshDrawCommands& meshDrawCommands, Memory::RefPtr<GlobalResource> globalResource, Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList || !globalResource|| !m_pipeLine)
		{
			return;
		}

		static float tick = 0.0f;
		const float diff = 0.01f;
		tick += (tick >= 6.283185307f) ? -6.283185307f + diff : diff;

		RHIClearState clearState = m_pipeLine->GetClearState();

		clearState.color.r = std::sin(tick) * 0.5f + 0.5f;
		clearState.color.g = std::sin(tick + 2.094f) * 0.5f + 0.5f;
		clearState.color.b = std::sin(tick + 4.188f) * 0.5f + 0.5f;
		clearState.color.a = 1.0f;

		cmdList->Clear(clearState);
		cmdList->SetPipeLine(m_pipeLine);
		cmdList->SetBuffer(globalResource->GetCameraBuffer(), m_cameraSlot);

		RHIDrawIndexDesc drawDesc;

		for (const auto drawCommand : meshDrawCommands)
		{
			if (!drawCommand)
			{
				continue;
			}

			drawDesc.drawMode = eDrawMode::eTriangles;
			drawDesc.indexType = eDataType::eUInt;
			drawDesc.indexCount = drawCommand->indexCount;
			drawDesc.indexOffset = drawCommand->indexOffset * GetDataTypeSize(drawDesc.indexType);
			drawDesc.baseVertex = drawCommand->minVertexIndex;
			drawDesc.instanceCount = drawCommand->instanceCount;

			cmdList->SetBuffer(drawCommand->instanceBuffer, m_instanceSlot);
			cmdList->SetVertexLayout(drawCommand->vertexLayout);
			cmdList->DrawIndexPrimitive(drawDesc);
		}
	}

	void SimpleColor::Upload(Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList)
		{
			return;
		}

		if (!m_vertexShader || !m_vertexShader->shader || !m_pixelShader || !m_pixelShader->shader)
		{
			return;
		}

		RHIPipeLineCreateDesc desc;
		desc.clear.clearBuffer = eClearBuffer::eColor;
		desc.clear.color = { 0.0f, 0.0f, 0.0f, 1.0f };
		desc.depth.enable = false;
		desc.stencil.enable = false;
		desc.blend.enable = false;
		desc.rasterizer.cullEnable = false;
		desc.rasterizer.cullFace = eCullFace::eBack;
		desc.rasterizer.frontFace = eFrontFace::eCCW;
		desc.rasterizer.fillMode = ePrimitiveMode::eFill;
		desc.vertexShader = m_vertexShader->shader;
		desc.pixelShader = m_pixelShader->shader;

		m_pipeLine = cmdList->CreatePipeLine(desc);
		if (!m_pipeLine)
		{
			LOGERROR() << "[SimpleColor] Failed to upload the simple color pipeline, cause failed to create the pipeline";
			return;
		}
	}

	void SimpleColor::Unload(Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList)
		{
			return;
		}

		if (m_pipeLine)
		{
			cmdList->RemovePipeLine(m_pipeLine);
			m_pipeLine = nullptr;
		}

		m_vertexShader = nullptr;
		m_pixelShader = nullptr;
	}

	eResourceState SimpleColor::GetResourceState() const
	{
		if (!m_pipeLine || !m_vertexShader || !m_pixelShader)
		{
			return eResourceState::eNone;
		}

		const eResourceState allState = m_pipeLine->GetState() & m_vertexShader->GetResourceState() & m_pixelShader->GetResourceState();

		return allState;
	}

	eResourceState SimpleColor::GetShaderState() const
	{
		if (!m_vertexShader || !m_pixelShader)
		{
			return eResourceState::eNone;
		}

		const eResourceState allState = m_vertexShader->GetResourceState() & m_pixelShader->GetResourceState();
		return allState;
	}
};