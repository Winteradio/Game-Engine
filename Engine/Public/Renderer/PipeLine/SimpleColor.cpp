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

		// tick 증가량은 그대로 두거나DeltaTime을 곱해 프레임 독립적으로 관리하세요.
		static float tick = 0.0f;
		const float diff = 0.01;
		tick += (tick >= 6.283185307f) ? -6.283185307f + diff : diff;

		RHIClearState clearState = m_pipeLine->GetClearState();

		// 1. sin 함수를 [0, 1] 범위로 변환: (sin(x) * 0.5) + 0.5
		// 2. 각 채널에 120도(2π/3)씩 차이를 주어 색상이 부드럽게 돌아가도록 설정
		clearState.color.r = std::sin(tick) * 0.5f + 0.5f;
		clearState.color.g = std::sin(tick + 2.094f) * 0.5f + 0.5f; // + 120도
		clearState.color.b = std::sin(tick + 4.188f) * 0.5f + 0.5f; // + 240도
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