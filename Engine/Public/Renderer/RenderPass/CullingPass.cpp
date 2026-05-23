#include <Renderer/RenderPass/CullingPass.h>

#include <Renderer/GlobalRenderer.h>
#include <Renderer/MeshDrawCommand.h>

#include <RHI/RHIDescriptions.h>
#include <RHI/RHIResources.h>
#include <RHI/RHICommandList.h>

namespace wtr
{
	CullingPass::CullingPass()
		: ComputePass(64,64,64)
	{
	}

	eResourceState CullingPass::GetResourceState() const
	{
		return eResourceState::eReady;
	}

	const RHIDispatchDesc CullingPass::GetDispatchCommand(Memory::RefPtr<const MeshDrawCommand> drawCommand)
	{
		return {};
	}

	void CullingPass::Upload(Memory::RefPtr<RHICommandList> cmdList)
	{
		return;
	}

	bool CullingPass::Draw(const MeshDrawCommands& drawCommands, const LightProxies& lightProxies, Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList)
		{
			return false;
		}

		if (drawCommands.Empty())
		{
			return true;
		}

		return true;
	}

	bool CullingPass::SetCommand(Memory::RefPtr<RHICommandList> cmdList, Memory::RefPtr<const RHIPipeLine> pipeline, Memory::RefPtr<const MeshDrawCommand> drawCommand)
	{
		if (!cmdList || !pipeline || !drawCommand)
		{
			return false;
		}

		const auto cameraBuffer = GlobalResource::GetCamera();
		const auto indirectBuffer = drawCommand->indirect;
		const auto visibleBuffer = drawCommand->visible;
		const auto transformBuffer = drawCommand->transform;
		const auto boudingBoxBuffer = drawCommand->localBounding;
		if (!cameraBuffer || cameraBuffer->GetState() != eResourceState::eReady ||
			!indirectBuffer || indirectBuffer->GetState() != eResourceState::eReady ||
			!visibleBuffer || visibleBuffer->GetState() != eResourceState::eReady ||
			!transformBuffer || transformBuffer->GetState() != eResourceState::eReady ||
			!boudingBoxBuffer || boudingBoxBuffer->GetState() != eResourceState::eReady)
		{
			return false;
		}

		cmdList->SetBuffer(pipeline, cameraBuffer, eResourceSlot::eCamera);
		cmdList->SetBuffer(pipeline, indirectBuffer, eResourceSlot::eIndirect);
		cmdList->SetBuffer(pipeline, visibleBuffer, eResourceSlot::eVisible);
		cmdList->SetBuffer(pipeline, transformBuffer, eResourceSlot::eTransform);
		cmdList->SetBuffer(pipeline, boudingBoxBuffer, eResourceSlot::eLocalBounding);

		return true;
	}

	void CullingPass::UnsetCommand(Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList)
		{
			return;
		}
	}
}