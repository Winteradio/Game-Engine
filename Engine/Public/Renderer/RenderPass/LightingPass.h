#ifndef __WTR_LIGHTINGPASS_H__
#define __WTR_LIGHTINGPASS_H__

#include <Renderer/RenderPass/RenderPass.h>

namespace wtr
{
	class RHIRenderTarget;
};

namespace wtr
{
	class LightingPass : public GraphicPass
	{
		GENERATE(LightingPass);
	public:
		LightingPass();
		virtual ~LightingPass() = default;

	public:
		eResourceState GetResourceState() const override;
		void Upload(Memory::RefPtr<RHICommandList> cmdList) override;
		void Unload(Memory::RefPtr<RHICommandList> cmdList) override;

		void Draw(const MeshDrawCommands& drawCommands, const LightProxies& lightProxies, Memory::RefPtr<RHICommandList> cmdList) override;

		bool SetCommand(Memory::RefPtr<RHICommandList> cmdList, Memory::RefPtr<const RHIPipeLine> pipeline, Memory::RefPtr<const MeshDrawCommand> drawCommand) override;
		void UnsetCommand(Memory::RefPtr<RHICommandList> cmdList, Memory::RefPtr<const MeshDrawCommand> drawCommand) override;

		const RHIDrawIndexDesc GetDrawCommand(Memory::RefPtr<const MeshDrawCommand> drawCommand) override;
		Memory::RefPtr<const RHIPipeLine> GetPipeLine(Memory::RefPtr<RHICommandList> cmdList, Memory::RefPtr<const ShaderProxy> shaderProxy) override;

	private:
		Memory::RefPtr<RHIRenderTarget> m_target;
	};
}

#endif // __WTR_LIGHTINGPASS_H__