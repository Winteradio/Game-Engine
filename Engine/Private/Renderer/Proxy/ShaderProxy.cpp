#include <Renderer/Proxy/ShaderProxy.h>

#include <RHI/RHIResources.h>

namespace wtr
{
	ShaderProxy::ShaderProxy()
		: m_shaders()
	{}

	eResourceState ShaderProxy::GetResourceState() const
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

	void ShaderProxy::AddShader(Memory::RefPtr<const ShaderState> shaderState)
	{
		const Reflection::TypeInfo* typeInfo = shaderState->GetTypeInfo();
		const size_t typeHash = typeInfo->GetTypeHash();
		m_shaders[typeHash] = shaderState;
	}
}