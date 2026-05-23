#ifndef __WTR_TRANSFORMPASS_H__
#define __WTR_TRANSFORMPASS_H__

#include <Renderer/RenderPass/RenderPass.h>

namespace wtr
{
	class RHIRenderTarget;
};

namespace wtr
{
	class TransformPass : public ComputePass
	{
		GENERATE(TransformPass);
	public:
		TransformPass();
		virtual ~TransformPass() = default;

	public:
		eResourceState GetResourceState() const override;

		const RHIDispatchDesc GetDispatchCommand(Memory::RefPtr<const MeshDrawCommand> drawCommand);

		void Upload(Memory::RefPtr<RHICommandList> cmdList) override;
		bool Draw(const MeshDrawCommands& drawCommands, const LightProxies& lightProxies, Memory::RefPtr<RHICommandList> cmdList) override;

		bool SetCommand(Memory::RefPtr<RHICommandList> cmdList, Memory::RefPtr<const RHIPipeLine> pipeline, Memory::RefPtr<const MeshDrawCommand> drawCommand) override;
		void UnsetCommand(Memory::RefPtr<RHICommandList> cmdList) override;
	};
};

#endif // __WTR_TRANSFORMPASS_H__