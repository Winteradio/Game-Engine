#include <Renderer/PipeLine/SimpleColor.h>

#include <Memory/include/Core.h>
#include <Framework/SceneView.h>
#include <RHI/RHIDescriptions.h>
#include <RHI/RHIStates.h>

namespace wtr
{
	namespace PipeLine
	{
		SimpleColor::SimpleColor(Memory::RefPtr<RHISystem> system)
			: Base(system)
		{}

		SimpleColor::~SimpleColor()
		{}

		void SimpleColor::Draw(const SceneView& scene)
		{

		}

		void SimpleColor::Init()
		{
			RHIPipeLineDesc desc;
			desc.depth.enable = false;
			desc.stencil.enable = false;
			desc.blend.enable = false;
			desc.rasterizer.cullEnable = false;
			desc.rasterizer.cullFace = eCullFace::eBack;
			desc.rasterizer.frontFace = eFrontFace::eCCW;
			desc.rasterizer.fillMode = ePrimitiveMode::eFill;

			m_pipeLine = Memory::MakeRef<RHIPipeLine>(desc);
		}
	};
};