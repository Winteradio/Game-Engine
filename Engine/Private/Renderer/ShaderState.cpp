#include <Renderer/ShaderState.h>

#include <Asset/AssetTypes.h>
#include <RHI/RHIResources.h>


namespace wtr
{
	ShaderState::ShaderState(Memory::RefPtr<const ShaderAsset> shaderAsset)
		: m_shaderAsset(shaderAsset)
	{}

	eResourceState ShaderState::GetResourceState() const
	{
		if (!m_shaderAsset)
		{
			return eResourceState::eError;
		}

		if (!m_shaderAsset->shader)
		{
			return eResourceState::eNone;
		}
		else
		{
			return m_shaderAsset->shader->GetState();
		}
	}

	Memory::RefPtr<const RHIShader> ShaderState::GetShader() const
	{
		if (m_shaderAsset)
		{
			return m_shaderAsset->shader;
		}

		return nullptr;
	}
}