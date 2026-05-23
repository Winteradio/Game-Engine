#ifndef __WTR_RENDERPASS_H__
#define __WTR_RENDERPASS_H__

#include <Renderer/RenderResource.h>

#include <Memory/include/Pointer/RefPtr.h>
#include <Container/include/DynamicArray.h>
#include <Container/include/HashMap.h>
#include <ECS/include/Object/Object.h>
#include <Reflection/include/Type/TypeMacro.h>

namespace wtr
{
	struct RenderView;
	struct MeshDrawCommand;
	class LightProxy;
	class ShaderProxy;
	class ShaderState;

	struct RHIClearState;
	struct RHIColorState;
	struct RHIDepthState;
	struct RHIStencilState;
	struct RHIBlendState;
	struct RHIRasterizerState;
	struct RHIDrawIndexDesc;
	struct RHIDispatchDesc;
	class RHIPipeLine;
	class RHICommandList;
};

namespace wtr
{
	using MeshDrawCommands = wtr::DynamicArray<Memory::RefPtr<const MeshDrawCommand>>;
	using LightProxies = wtr::DynamicArray<Memory::RefPtr<const LightProxy>>;

	class RenderPass : public ECS::Object, public RenderResource
	{
		GENERATE(RenderPass);
	public :
		RenderPass() = default;
		virtual ~RenderPass() = default;

	public :
		virtual bool Draw(const MeshDrawCommands& drawCommands, const LightProxies& lightProxies, Memory::RefPtr<RHICommandList> cmdList) = 0;
		virtual bool SetCommand(Memory::RefPtr<RHICommandList> cmdList, Memory::RefPtr<const RHIPipeLine> pipeline, Memory::RefPtr<const MeshDrawCommand> drawCommand) = 0;
		virtual void UnsetCommand(Memory::RefPtr<RHICommandList> cmdList) = 0;
	};

	class ComputePass : public RenderPass
	{
		GENERATE(ComputePass);
	public :
		ComputePass(const uint32_t groupX, const uint32_t groupY, const uint32_t groupZ);
		virtual ~ComputePass() = default;

	public :
		const RHIDispatchDesc GetDispatchCommand();
		Memory::RefPtr<const RHIPipeLine> GetPipeLine(Memory::RefPtr<RHICommandList> cmdList);

	protected :
		uint32_t m_groupX;
		uint32_t m_groupY;
		uint32_t m_groupZ;

		Memory::RefPtr<ShaderState> m_computeShader;
	};

	class GraphicPass : public RenderPass
	{
		GENERATE(GraphicPass);
	public :
		GraphicPass() = default;
		virtual ~GraphicPass() = default;

	public :
		virtual void InitState() = 0;
		virtual const RHIDrawIndexDesc GetDrawCommand(Memory::RefPtr<const MeshDrawCommand> drawCommand) = 0;
		virtual Memory::RefPtr<const RHIPipeLine> GetPipeLine(Memory::RefPtr<RHICommandList> cmdList, Memory::RefPtr<const ShaderProxy> shaderProxy) = 0;

	public :
		void SetState(Memory::RefPtr<RHICommandList> cmdList);

	protected :
		Memory::RefPtr<RHIClearState> m_clear;
		Memory::RefPtr<RHIColorState> m_color;
		Memory::RefPtr<RHIDepthState> m_depth;
		Memory::RefPtr<RHIStencilState> m_stencil;
		Memory::RefPtr<RHIBlendState> m_blend;
		Memory::RefPtr<RHIRasterizerState> m_rasterizer;
	};

	struct RenderPassHasher
	{
		size_t operator()(const Memory::RefPtr<RenderPass>& refPass)
		{
			if (!refPass)
			{
				return 0;
			}

			return std::hash<const void*>()(refPass.Get());
		}
	};
};

#endif // __WTR_RENDERPASS_H__