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
		, state(eAssetState::eNone)
	{}

	Asset::Asset(const std::string& path, const eExtension extension, const eAsset type)
		: id(ECS::UUID())
		, name()
		, path(path)
		, extension(extension)
		, type(type)
		, state(eAssetState::eNone)
	{}

	void Asset::SetState(const eAssetState state)
	{
		this->state = state;
	}

	eAssetState Asset::GetState() const
	{
		if (eAssetState::eCreated == state)
		{
			eResourceState resourceState = GetResourceState();

			if (eResourceState::eNone != (eResourceState::eReady & resourceState))
			{
				state = eAssetState::eReady;
			}
		}

		return state;
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
	{}

	MaterialAsset::MaterialAsset(const std::string& path, const eExtension extension)
		: Asset(path, extension, eAsset::eMaterial)
		, textures()
		, vectorValues()
		, scalarValues()
	{}

	eResourceState MaterialAsset::GetResourceState() const
	{
		if (textures.Empty() && vectorValues.Empty() && scalarValues.Empty())
		{
			return eResourceState::eReady;
		}

		eResourceState allState = eResourceState::eAll;
		for (const auto& [slot, texture] : textures)
		{
			if (texture)
			{
				allState &= texture->GetResourceState();
			}
		}

		return allState;
	}

	TextureAsset::TextureAsset()
		: Asset()
		, rawBuffer()
		, texture()
	{}

	TextureAsset::TextureAsset(const std::string& path, const eExtension extension)
		: Asset(path, extension, eAsset::eTexture)
		, rawBuffer()
		, texture()
	{}

	eResourceState TextureAsset::GetResourceState() const
	{
		if (!rawBuffer)
		{
			return eResourceState::eNone;
		}

		if (rawBuffer->data.Empty())
		{
			return eResourceState::eError;
		}

		if (texture)
		{
			return texture->GetState();
		}

		return eResourceState::eError;
	}

	MeshAsset::MeshAsset()
		: Asset()
		, rawBuffers()
		, buffers()
		, rawIndex()
		, index()
		, sections()
		, materials()
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
	{
	}

	eResourceState MeshAsset::GetResourceState() const
	{
		if (rawBuffers.Empty() && !rawIndex)
		{
			return eResourceState::eNone;
		}

		eResourceState resource = eResourceState::eAll;

		for (const auto& [key, buffer] : buffers)
		{
			if (buffer)
			{
				resource &= buffer->GetState();
			}
		}

		for (const auto& material : materials)
		{
			if (material)
			{
				resource &= material->GetResourceState();
			}
		}

		return resource;
	}

	ShaderAsset::ShaderAsset()
		: Asset()
		, rawBuffer()
	{}

	ShaderAsset::ShaderAsset(const std::string& path, const eExtension extension)
		: Asset(path, extension, eAsset::eShader)
		, rawBuffer()
	{}

	eResourceState ShaderAsset::GetResourceState() const
	{
		if (!rawBuffer)
		{
			return eResourceState::eNone;
		}

		if (rawBuffer->data.Empty())
		{
			return eResourceState::eError;
		}

		if (shader)
		{
			return shader->GetState();
		}

		return eResourceState::eError;
	}

	ComposeAsset::ComposeAsset()
		: Asset()
	{
	}

	ComposeAsset::ComposeAsset(const std::string& path, const eExtension extension)
		: Asset(path, extension, eAsset::eCompose)
	{
	}

	eResourceState ComposeAsset::GetResourceState() const
	{
		if (meshs.Empty() && materials.Empty() && textures.Empty())
		{
			return eResourceState::eNone;
		}

		eResourceState allState = eResourceState::eAll;
		for (const auto& mesh : meshs)
		{
			if (mesh)
			{
				allState &= mesh->GetResourceState();
			}
		}

		for (const auto& material : materials)
		{
			if (material)
			{
				allState &= material->GetResourceState();
			}
		}

		for (const auto& texture : textures)
		{
			if (texture)
			{
				allState &= texture->GetResourceState();
			}
		}


		return allState;
	}
}