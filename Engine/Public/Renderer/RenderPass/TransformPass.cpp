#include <Renderer/RenderPass/TransformPass.h>

#include <Asset/AssetSystem.h>
#include <Asset/AssetTypes.h>

#include <Renderer/GlobalRenderer.h>
#include <Renderer/MeshDrawCommand.h>
#include <Renderer/ShaderState.h>

#include <RHI/RHIDescriptions.h>
#include <RHI/RHIResources.h>
#include <RHI/RHICommandList.h>

#include <Memory/include/Core.h>
#include <Log/include/Log.h>

namespace wtr
{
	TransformPass::TransformPass()
		: ComputePass(64, 64, 64)
	{
	}

	eResourceState TransformPass::GetResourceState() const
	{
		return m_computeShader ? m_computeShader->GetResourceState() : eResourceState::eNone;
	}

	void TransformPass::Upload(Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList || m_computeShader)
		{
			return;
		}

		Memory::RefPtr<const ShaderAsset> computeShader = Memory::Cast<const ShaderAsset>(AssetSystem::Load("asset/shader/transform.cs.glsl"));
		if (!computeShader)
		{
			LOGERROR() << "[TransformPass] Failed to load the compute shader";
		}

		m_computeShader = Memory::MakeRef<ShaderState>(computeShader);
	}

	bool TransformPass::Draw(const MeshDrawCommands& drawCommands, const LightProxies& lightProxies, Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList || !m_computeShader || m_computeShader->GetResourceState() != eResourceState::eReady)
		{
			return false;
		}

		if (drawCommands.Empty())
		{
			return true;
		}

		auto pipeline = GetPipeLine(cmdList);
		if (!pipeline || pipeline->GetState() != eResourceState::eReady)
		{
			return false;
		}

		cmdList->SetPipeLine(pipeline);

		for (const auto& command : drawCommands)
		{
			if (!command)
			{
				continue;
			}


			if (SetCommand(cmdList, pipeline, command))
			{
				const auto dispatchDesc = GetDispatchCommand();
				cmdList->DispatchCompute(dispatchDesc);
			}

			UnsetCommand(cmdList);
		}

		cmdList->UnsetPipeLine();

		return true;
	}

	bool TransformPass::SetCommand(Memory::RefPtr<RHICommandList> cmdList, Memory::RefPtr<const RHIPipeLine> pipeline, Memory::RefPtr<const MeshDrawCommand> drawCommand)
	{
		if (!cmdList || !pipeline || !drawCommand)
		{
			return false;
		}

		const RHIResourceBinding camera = pipeline->GetBindingSlot(eResourceSlot::eCamera);
		const RHIResourceBinding indirect = pipeline->GetBindingSlot(eResourceSlot::eIndirect);
		const RHIResourceBinding visible = pipeline->GetBindingSlot(eResourceSlot::eVisible);
		const RHIResourceBinding transform = pipeline->GetBindingSlot(eResourceSlot::eTransform);
		const RHIResourceBinding localBouding = pipeline->GetBindingSlot(eResourceSlot::eLocalBounding);

		if (camera.location == 0 || indirect.location == 0 || visible.location == 0 || transform.location == 0 || localBouding.location == 0)
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

		cmdList->SetBuffer(cameraBuffer, camera.location);
		cmdList->SetBuffer(indirectBuffer, indirect.location);
		cmdList->SetBuffer(visibleBuffer, visible.location);
		cmdList->SetBuffer(transformBuffer, transform.location);
		cmdList->SetBuffer(boudingBoxBuffer, localBouding.location);

		return true;
	}

	void TransformPass::UnsetCommand(Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList)
		{
			return;
		}
	}
}