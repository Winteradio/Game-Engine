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
		, path("")
		, extension(eExtension::eNone)
		, type(eAsset::eNone)
		, state(eAssetState::eNone)
	{}

	Asset::Asset(const std::string& path, const eExtension extension, const eAsset type)
		: id(ECS::UUID())
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

	MeshAsset::MeshAsset()
		: Asset()
	{}

	MeshAsset::MeshAsset(const std::string& path, const eExtension extension)
		: Asset(path, extension, eAsset::eMesh)
	{}

	eResourceState MeshAsset::GetResourceState() const
	{
		eResourceState resource = eResourceState::eAll;

		for (const auto& [name, buffer] : buffers)
		{
			if (buffer)
			{
				resource &= buffer->GetState();
			}
			else
			{
				resource &= eResourceState::eNone;
				break;
			}
		}

		return resource;
	}

	MaterialAsset::MaterialAsset()
		: Asset()
	{}

	MaterialAsset::MaterialAsset(const std::string& path, const eExtension extension)
		: Asset(path, extension, eAsset::eMaterial)
	{}

	eResourceState MaterialAsset::GetResourceState() const
	{
		return eResourceState::eAll;
	}

	TextureAsset::TextureAsset()
		: Asset()
	{}

	TextureAsset::TextureAsset(const std::string& path, const eExtension extension)
		: Asset(path, extension, eAsset::eTexture)
	{}

	eResourceState TextureAsset::GetResourceState() const
	{
		if (texture)
		{
			return texture->GetState();
		}
		else
		{
			return eResourceState::eNone;
		}
	}

	ShaderAsset::ShaderAsset()
		: Asset()
	{}

	ShaderAsset::ShaderAsset(const std::string& path, const eExtension extension)
		: Asset(path, extension, eAsset::eShader)
	{}

	eResourceState ShaderAsset::GetResourceState() const
	{
		if (shader)
		{
			return shader->GetState();
		}
		else
		{
			return eResourceState::eNone;
		}
	}

	ComposeAsset::ComposeAsset()
		: Asset()
	{
	}

	ComposeAsset::ComposeAsset(const std::string& path, const eExtension extension)
		: Asset(path, extension, eAsset::eCompose)
	{
	}

	eAssetState ComposeAsset::GetState() const
	{
		eAssetState state = eAssetState::eAll;
		for (const auto& mesh : meshs)
		{
			if (mesh)
			{
				state &= mesh->GetState();
			}
			else
			{
				state = eAssetState::eNone;
				break;
			}
		}

		for (const auto& material : materials)
		{
			if (material)
			{
				state &= material->GetState();
			}
			else
			{
				state = eAssetState::eNone;
				break;
			}
		}

		for (const auto& texture : textures)
		{
			if (texture)
			{
				state &= texture->GetState();
			}
			else
			{
				state = eAssetState::eNone;
				break;
			}
		}

		return state;
	}
}