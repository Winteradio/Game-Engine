#ifndef __WTR_CULLINGPASS_H__
#define __WTR_CULLINGPASS_H__

#include <Renderer/RenderPass/RenderPass.h>

namespace wtr
{
	class RHIRenderTarget;
};

namespace wtr
{
	class CullingPass : public ComputePass
	{
		GENERATE(CullingPass);
	public:
		CullingPass();
		virtual ~CullingPass() = default;

	public:
		eResourceState GetResourceState() const override;

		const RHIDispatchDesc GetDispatchCommand(Memory::RefPtr<const MeshDrawCommand> drawCommand) override;

		void Upload(Memory::RefPtr<RHICommandList> cmdList) override;
		bool Draw(const MeshDrawCommands& drawCommands, const LightProxies& lightProxies, Memory::RefPtr<RHICommandList> cmdList) override;

		bool SetCommand(Memory::RefPtr<RHICommandList> cmdList, Memory::RefPtr<const RHIPipeLine> pipeline, Memory::RefPtr<const MeshDrawCommand> drawCommand) override;
		void UnsetCommand(Memory::RefPtr<RHICommandList> cmdList) override;
	};
};

#endif // __WTR_CULLINGPASS_H__