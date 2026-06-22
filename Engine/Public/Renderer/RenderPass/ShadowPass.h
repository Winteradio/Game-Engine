#ifndef __WTR_SHADOWPASS_H__
#define __WTR_SHADOWPASS_H__

#include <Renderer/RenderPass/RenderPass.h>

namespace wtr
{
	class LightProxy;
};

namespace wtr
{
	class ShadowPass : public GraphicPass
	{
		GENERATE(ShadowPass);
	public:
		ShadowPass();
		virtual ~ShadowPass() = default;

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
		wtr::HashMap<Memory::RefPtr<const RHIPipeLine>, LightProxies> m_lights;
	};
};

#endif // __WTR_SHADOWPASS_H__