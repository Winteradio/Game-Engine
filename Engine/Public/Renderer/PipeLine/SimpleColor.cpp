#include <Renderer/PipeLine/SimpleColor.h>

#include <Framework/SceneView.h>
#include <RHI/RHICommandList.h>
#include <RHI/RHIDescriptions.h>
#include <RHI/RHIResources.h>
#include <RHI/RHIStates.h>

namespace wtr
{
	SimpleColor::SimpleColor()
		: PipeLine()
		, m_vertexShader(nullptr)
		, m_pixelShader(nullptr)
	{}

	SimpleColor::~SimpleColor()
	{}

	void SimpleColor::Draw(const SceneView& scene, Memory::RefPtr<RHICommandList> commandList)
	{
		if (!commandList || !m_pipeLine) // TODO
		{
			return;
		}

		static float tick = 0.0f;
		tick += 0.01f;
		
		RHIClearState clearState = m_pipeLine->GetClearState();
		clearState.color.r = std::sin(tick);
		clearState.color.g = std::cos(tick);
		clearState.color.b = std::tan(tick);
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

		RHIPipeLineDesc desc;
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
};