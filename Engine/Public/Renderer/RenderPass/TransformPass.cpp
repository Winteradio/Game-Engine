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

	const RHIDispatchDesc TransformPass::GetDispatchCommand(Memory::RefPtr<const MeshDrawCommand> drawCommand)
	{
		if (!drawCommand)
		{
			return {};
		}

		constexpr uint32_t threadCount = 64;
		RHIDispatchDesc desc;
		desc.groupX = (drawCommand->instanceCount + threadCount - 1) / threadCount;
		desc.groupY = 1;
		desc.groupZ = 1;

		return desc;
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
				const auto dispatchDesc = GetDispatchCommand(command);
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

		const auto rawTransformBuffer = drawCommand->rawTransform;
		const auto transformBuffer = drawCommand->transform;
		if (!rawTransformBuffer || rawTransformBuffer->GetState() != eResourceState::eReady ||
			!transformBuffer || transformBuffer->GetState() != eResourceState::eReady)
		{
			return false;
		}

		Memory::RefPtr<ScalarData<uint32_t>> instanceCount = Memory::MakeRef<ScalarData<uint32_t>>();
		instanceCount->data = static_cast<uint32_t>(drawCommand->instanceCount);

		RHIConstDesc info;
		info.dataType = eDataType::eUInt;
		info.dimension = 1;
		info.offset = 0;
		info.data = instanceCount;

		cmdList->SetBuffer(pipeline, rawTransformBuffer, eResourceSlot::eRawTransform);
		cmdList->SetBuffer(pipeline, transformBuffer, eResourceSlot::eTransform);
		cmdList->SetConstant(pipeline, info, eResourceSlot::eInstanceCount);

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