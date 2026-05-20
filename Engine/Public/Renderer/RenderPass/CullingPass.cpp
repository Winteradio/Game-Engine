#include <Renderer/RenderPass/CullingPass.h>

#include <Renderer/Proxy/MaterialProxy.h>
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

	void CullingPass::Draw(const MeshDrawCommands& drawCommands, const LightProxies& lightProxies, Memory::RefPtr<RHICommandList> cmdList)
	{
		if (drawCommands.Empty() || !cmdList)
		{
			return;
		}

		m_commands.Clear();

		for (const auto& drawCommand : drawCommands)
		{
			if (!drawCommand)
			{
				continue;
			}

			auto pipeline = GetPipeLine(cmdList, drawCommand->material);
			if (!pipeline || pipeline->GetState() != eResourceState::eReady)
			{
				continue;
			}

			m_commands[pipeline].PushBack(drawCommand);
		}

		for (const auto& [pipeline, commands] : m_commands)
		{
			if (!pipeline || pipeline->GetState() != eResourceState::eReady)
			{
				continue;
			}

			cmdList->SetPipeLine(pipeline);

			for (const auto& command : commands)
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

				UnsetCommand(cmdList, command);
			}

			cmdList->UnsetPipeLine(pipeline);
		}
	}

	bool CullingPass::SetCommand(Memory::RefPtr<RHICommandList> cmdList, Memory::RefPtr<const RHIPipeLine> pipeline, Memory::RefPtr<const MeshDrawCommand> drawCommand)
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

	void CullingPass::UnsetCommand(Memory::RefPtr<RHICommandList> cmdList, Memory::RefPtr<const MeshDrawCommand> drawCommand)
	{
		if (!cmdList || !drawCommand)
		{
			return;
		}
	}

	const RHIDispatchDesc CullingPass::GetDispatchCommand()
	{
		RHIDispatchDesc desc;
		desc.groupX = m_groupX;
		desc.groupY = m_groupY;
		desc.groupZ = m_groupZ;

		return desc;
	}

	Memory::RefPtr<const RHIPipeLine> CullingPass::GetPipeLine(Memory::RefPtr<RHICommandList> cmdList, Memory::RefPtr<const ShaderProxy> shaderProxy)
	{
		if (!cmdList || !shaderProxy)
		{
			return {};
		}

		auto csAsset = shaderProxy->GetShader<ComputeCSState>();
		if (!csAsset || csAsset->GetResourceState() != eResourceState::eReady)
		{
			return {};
		}

		RHIPipeLineCreateDesc desc;
		desc.computeShader = csAsset ? csAsset->GetShader() : nullptr;

		Memory::RefPtr<const RHIPipeLine> pipeline = GlobalResource::GetPipeLine(cmdList, desc);
		if (!pipeline || pipeline->GetState() != eResourceState::eReady)
		{
			return {};
		}
		else
		{
			return pipeline;
		}
	}
}