#include <Renderer/PipeLine/PipeLine.h>

#include <RHI/RHIResources.h>

namespace wtr
{
	PipeLine::PipeLine()
		: m_pipeLine(nullptr)
		, m_prepared(false)
	{}

	PipeLine::~PipeLine()
	{}

	void PipeLine::Execute(const MeshDrawCommands& meshDrawCommands, Memory::RefPtr<GlobalResource> globalResource, Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!m_prepared)
		{
			Prepare();
		}
		else
		{
			Draw(meshDrawCommands, globalResource, cmdList);
		}
	}

	std::string PipeLineString::operator()(const Memory::RefPtr<PipeLine>& pipeLine) const
	{
		if (!pipeLine)
		{
			return {};
		}

		const Reflection::TypeInfo* typeInfo = pipeLine->GetTypeInfo();
		return typeInfo->GetTypeName();
	}

	size_t PipeLineHasher::operator()(const Memory::RefPtr<PipeLine>& pipeLine) const
	{
		if (pipeLine)
		{
			const Reflection::TypeInfo* typeInfo = pipeLine->GetTypeInfo();
			return typeInfo->GetTypeHash();
		}
		else
		{
			return 0;
		}
	}
}