#include <Asset/AssetTypes.h>

#include <RHI/RHIResources.h>

namespace wtr
{
	eAssetState operator|(const eAssetState lhs, const eAssetState rhs)
	{
		return static_cast<eAssetState>(static_cast<uint8_t>(lhs) | static_cast<uint8_t>(rhs));
	}

	eAssetState operator&(const eAssetState lhs, const eAssetState rhs)
	{
		return static_cast<eAssetState>(static_cast<uint8_t>(lhs) & static_cast<uint8_t>(rhs));
	}

	eAssetState operator^(const eAssetState lhs, const eAssetState rhs)
	{
		return static_cast<eAssetState>(static_cast<uint8_t>(lhs) ^ static_cast<uint8_t>(rhs));
	}

	eAssetState operator~(const eAssetState state)
	{
		return static_cast<eAssetState>(~static_cast<uint8_t>(state));
	}

	eAssetState operator |=(eAssetState lhs, const eAssetState rhs)
	{
		lhs = static_cast<eAssetState>(static_cast<uint8_t>(lhs) | static_cast<uint8_t>(rhs));
		return lhs;
	}

	eAssetState operator&=(eAssetState lhs, const eAssetState rhs)
	{
		lhs = static_cast<eAssetState>(static_cast<uint8_t>(lhs) & static_cast<uint8_t>(rhs));
		return lhs;
	}

	eAssetState operator^=(eAssetState lhs, const eAssetState rhs)
	{
		lhs = static_cast<eAssetState>(static_cast<uint8_t>(lhs) ^ static_cast<uint8_t>(rhs));
		return lhs;
	}

	Asset::Asset()
		: id(ECS::UUID())
		, name()
		, path()
		, extension(eExtension::eNone)
		, type(eAsset::eNone)
		, m_state(eAssetState::eNone)
	{
	}

	Asset::Asset(const std::string& path, const eExtension extension, const eAsset type)
		: id(ECS::UUID())
		, name()
		, path(path)
		, extension(extension)
		, type(type)
		, m_state(eAssetState::eNone)
	{
	}

	void Asset::SetState(const eAssetState state)
	{
		m_state.store(state, std::memory_order_release);
	}

	const eAssetState Asset::GetState() const
	{
		return m_state.load(std::memory_order_acquire);
	}

	eResourceState Asset::GetResourceState() const
	{
		return eResourceState::eNone;
	}

	MaterialAsset::MaterialAsset()
		: Asset()
		, textures()
		, vectorValues()
		, scalarValues()
		, shadingModel(eShadingModel::eUnlit)
		, blendMode(eBlendMode::eOpaque)
		, isDoubleSided(false)
		, isPBR(false)
	{
	}

	MaterialAsset::MaterialAsset(const std::string& path, const eExtension extension)
		: Asset(path, extension, eAsset::eMaterial)
		, textures()
		, vectorValues()
		, scalarValues()
		, shadingModel(eShadingModel::eUnlit)
		, blendMode(eBlendMode::eOpaque)
		, isDoubleSided(false)
		, isPBR(false)
	{
	}

	eResourceState MaterialAsset::GetResourceState() const
	{
		if (GetState() == eAssetState::eLoaded && textures.Empty())
		{
			return eResourceState::eNone;
		}
		else
		{
			for (const auto& [slot, texture] : textures)
			{
				if (!texture)
				{
					return eResourceState::eError;
				}
				else if (texture->GetResourceState() == eResourceState::eError)
				{
					return eResourceState::eError;
				}
				else if (texture->GetResourceState() != eResourceState::eReady)
				{
					return eResourceState::eNone;
				}
				else
				{
					//
				}
			}
		}

		return eResourceState::eReady;
	}

	TextureAsset::TextureAsset()
		: Asset()
		, rawTexture()
		, texture()
		, width(0)
		, height(0)
		, depth(0)
		, faces(0)
		, mipLevels(0)
		, sampleCount(0)
		, pixelFormat(ePixelFormat::eNone)
		, isSRGB(false)
		, isCubemap(false)
		, isGenerateMips(false)
	{
	}

	TextureAsset::TextureAsset(const std::string& path, const eExtension extension)
		: Asset(path, extension, eAsset::eTexture)
		, rawTexture()
		, texture()
		, width(0)
		, height(0)
		, depth(0)
		, faces(0)
		, mipLevels(0)
		, sampleCount(0)
		, pixelFormat(ePixelFormat::eNone)
		, isSRGB(false)
		, isCubemap(false)
		, isGenerateMips(false)
	{
	}

	eResourceState TextureAsset::GetResourceState() const
	{
		if (!texture)
		{
			return eResourceState::eNone;
		}
		else
		{
			return texture->GetState();
		}
	}

	MeshAsset::MeshAsset()
		: Asset()
		, rawBuffers()
		, buffers()
		, rawIndex()
		, index()
		, sections()
		, materials()
		, drawMode(eDrawMode::eNone)
	{
	}

	MeshAsset::MeshAsset(const std::string& path, const eExtension extension)
		: Asset(path, extension, eAsset::eMesh)
		, rawBuffers()
		, buffers()
		, rawIndex()
		, index()
		, sections()
		, materials()
		, drawMode(eDrawMode::eNone)
	{
	}

	eResourceState MeshAsset::GetResourceState() const
	{
		if (buffers.Empty())
		{
			return eResourceState::eNone;
		}

		for (const auto& [key, buffer] : buffers)
		{
			if (!buffer)
			{
				return eResourceState::eError;
			}
			else if (buffer->GetState() == eResourceState::eError)
			{
				return eResourceState::eError;
			}
			else if (buffer->GetState() != eResourceState::eReady)
			{
				return eResourceState::eNone;
			}
			else
			{
				//
			}
		}

		if (index)
		{
			if (index->GetState() == eResourceState::eError)
			{
				return eResourceState::eError;
			}
			else if (index->GetState() != eResourceState::eReady)
			{
				return eResourceState::eNone;
			}
			else
			{
				//
			}
		}

		if (!materials.Empty())
		{
			for (const auto& [name, material] : materials)
			{
				if (!material)
				{
					return eResourceState::eError;
				}
				else if (material->GetResourceState() == eResourceState::eError)
				{
					return eResourceState::eError;
				}
				else if (material->GetResourceState() != eResourceState::eReady)
				{
					return eResourceState::eNone;
				}
				else
				{
					//
				}
			}
		}

		return eResourceState::eReady;
	}

	ShaderAsset::ShaderAsset()
		: Asset()
		, rawBuffer()
		, m_shaderType(eShaderType::eNone)
	{}

	ShaderAsset::ShaderAsset(const std::string& path, const eExtension extension)
		: Asset(path, extension, eAsset::eShader)
		, rawBuffer()
		, m_shaderType(eShaderType::eNone)
	{}

	void ShaderAsset::SetShaderType(const eShaderType shaderType)
	{
		m_shaderType = shaderType;
	}

	eShaderType ShaderAsset::GetShaderType() const
	{
		return m_shaderType;
	}

	eResourceState ShaderAsset::GetResourceState() const
	{
		if (!shader)
		{
			return eResourceState::eNone;
		}
		else
		{
			return shader->GetState();
		}
	}
}