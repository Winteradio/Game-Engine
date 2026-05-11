#include <Renderer/Proxy/ShaderSet.h>

#include <RHI/RHIResources.h>

namespace wtr
{
	ShaderSet::ShaderSet()
		: m_shaders()
	{}

	eResourceState ShaderSet::GetResourceState() const
	{
		if (m_shaders.Empty())
		{
			return eResourceState::eNone;
		}

		eResourceState state = eResourceState::eAll;
		for (const auto& [hash, shaderState] : m_shaders)
		{
			if (!shaderState)
			{
				return eResourceState::eError;
			}

			state = std::min(state, shaderState->GetResourceState());
		}

		return state;
	}

	void ShaderSet::AddShader(Memory::RefPtr<const ShaderState> shaderState)
	{
		const Reflection::TypeInfo* typeInfo = shaderState->GetTypeInfo();
		const size_t typeHash = typeInfo->GetTypeHash();
		m_shaders[typeHash] = shaderState;
	}
}