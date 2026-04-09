#include <Renderer/PipeLine/SimpleColor.h>

#include <Asset/AssetTypes.h>
#include <Renderer/RenderView.h>
#include <Renderer/RenderScene.h>
#include <RHI/RHICommandList.h>
#include <RHI/RHIDescriptions.h>
#include <RHI/RHIResources.h>
#include <RHI/RHIStates.h>

#include <Log/include/Log.h>

namespace wtr
{
	SimpleColor::SimpleColor()
		: PipeLine()
		, m_vertexShader(nullptr)
		, m_pixelShader(nullptr)
	{}

	SimpleColor::~SimpleColor()
	{}

	void SimpleColor::Draw(const RenderView& renderView, Memory::RefPtr<RenderScene> renderScene, Memory::RefPtr<RHICommandList> commandList)
	{
		if (!renderScene || !commandList || !m_pipeLine) // TODO
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

		commandList->Clear(clearState);

		// TODO
	}

	void SimpleColor::Init(Memory::RefPtr<RHICommandList> commandList)
	{
		if (!commandList)
		{
			return;
		}

		if (m_pipeLine)
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

		m_pipeLine = commandList->CreatePipeLine(desc);
	}

	eResourceState SimpleColor::GetResourceState() const
	{
		if (!m_pipeLine || !m_vertexShader || !m_pixelShader)
		{
			return eResourceState::eNone;
		}

		eResourceState allState = m_pipeLine->GetState();

		allState |= m_vertexShader->GetResourceState() | m_pixelShader->GetResourceState();

		return allState;
	}
};