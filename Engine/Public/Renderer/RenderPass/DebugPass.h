#ifndef __WTR_DEBUGPASS_H__
#define __WTR_DEBUGPASS_H__

#include <Renderer/RenderPass/RenderPass.h>

namespace wtr
{
	class ShaderProxy;
	class LightProxy;
	class RHIVertexLayout;
}

namespace wtr
{
	class DebugPass : public GraphicPass
	{
		GENERATE(DebugPass);
	public:
		DebugPass();
		virtual ~DebugPass() = default;

	public:
		eResourceState GetResourceState() const override;
		void Upload(Memory::RefPtr<RHICommandList> cmdList) override;
		void Unload(Memory::RefPtr<RHICommandList> cmdList) override;

		void InitState();
		bool Draw(const MeshDrawCommands& drawCommands, const LightProxies& lightProxies, Memory::RefPtr<RHICommandList> cmdList) override;

		bool SetCommand(Memory::RefPtr<RHICommandList> cmdList, Memory::RefPtr<const RHIPipeLine> pipeline, Memory::RefPtr<const MeshDrawCommand> drawCommand) override;
		void UnsetCommand(Memory::RefPtr<RHICommandList> cmdList) override;

		RHIDrawIndexDesc GetDrawCommand(Memory::RefPtr<const MeshDrawCommand> drawCommand) override;
		Memory::RefPtr<const RHIPipeLine> GetPipeLine(Memory::RefPtr<RHICommandList> cmdList, Memory::RefPtr<const ShaderProxy> shaderProxy) override;

	private :
		bool SetLight(Memory::RefPtr<RHICommandList> cmdList, Memory::RefPtr<const RHIPipeLine> pipeline, Memory::RefPtr<const LightProxy> light);

	private :
		Memory::RefPtr<ShaderProxy> m_frustumShader;
	};
};

#endif