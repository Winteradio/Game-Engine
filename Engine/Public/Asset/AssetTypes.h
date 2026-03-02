#ifndef __WTR_ASSETTYPES_H__
#define __WTR_ASSETTYPES_H__

#include <Memory/include/Pointer/RefPtr.h>
#include <Container/include/HashMap.h>
#include <ECS/include/UUID/UUID.h>
#include <string>

namespace wtr
{
	class RHIBuffer;
	class RHITexture;
	class RHIShader;
	enum class eResourceState : uint8_t;
};

namespace wtr
{
	enum class eExtension : uint8_t
	{
		eNone		= 0x00,
		eOBJ		= 0x01,
		eFBX		= 0x02,
		ePNG		= 0x03,
		eJPG		= 0x04,
		eBMP		= 0x05,
		eGLSL		= 0x06,
		eEnd,
	};

	enum class eAsset : uint8_t
	{
		eNone		= 0x00,
		eMesh		= 0x01,
		eMaterial	= 0x02,
		eTexture	= 0x03,
		eShader		= 0x04,
		eCompose	= 0x05,
		eEnd,
	};

	enum class eAssetState : uint8_t
	{
		eNone		= 0x00,
		eLoaded		= 0x01,
		eCreated	= 0x02,
		eReady		= 0x04,
		eAll		= 0xFF
	};

	eAssetState operator|(const eAssetState lhs, const eAssetState rhs);
	eAssetState operator&(const eAssetState lhs, const eAssetState rhs);
	eAssetState operator^(const eAssetState lhs, const eAssetState rhs);
	eAssetState operator~(const eAssetState state);
	eAssetState operator|=(eAssetState lhs, const eAssetState rhs);
	eAssetState operator&=(eAssetState lhs, const eAssetState rhs);
	eAssetState operator^=(eAssetState lhs, const eAssetState rhs);

	class Asset
	{
	public :
		ECS::UUID id;
		const std::string path;
		const eExtension extension;
		const eAsset type;
		mutable std::atomic<eAssetState> state;

		Asset();
		Asset(const std::string& path, const eExtension extension, const eAsset type);
		virtual ~Asset() = default;

	public :
		virtual void SetState(const eAssetState state);
		
		virtual eAssetState GetState() const;

	protected:
		virtual eResourceState GetResourceState() const;
	};

	class MeshAsset : public Asset
	{
	public :
		MeshAsset();
		MeshAsset(const std::string& path, const eExtension extension);
		virtual ~MeshAsset() = default;

	protected:
		virtual eResourceState GetResourceState() const override;

	private :
		wtr::HashMap<std::string, Memory::RefPtr<RHIBuffer>> buffers;
	};

	class MaterialAsset : public Asset
	{
	public :
		MaterialAsset();
		MaterialAsset(const std::string& path, const eExtension extension);
		virtual ~MaterialAsset() = default;

	protected:
		virtual eResourceState GetResourceState() const override;
	};

	class TextureAsset : public Asset
	{
	public :
		TextureAsset();
		TextureAsset(const std::string& path, const eExtension extension);
		virtual ~TextureAsset() = default;

	protected:
		virtual eResourceState GetResourceState() const override;

	private :
		Memory::RefPtr<RHITexture> texture;
	};

	class ShaderAsset : public Asset
	{
	public :
		ShaderAsset();
		ShaderAsset(const std::string& path, const eExtension extension);
		virtual ~ShaderAsset() = default;

	protected:
		virtual eResourceState GetResourceState() const override;

	private :
		Memory::RefPtr<RHIShader> shader;
	};

	class ComposeAsset : public Asset
	{
	public :
		ComposeAsset();
		ComposeAsset(const std::string& path, const eExtension extension);
		virtual ~ComposeAsset() = default;

	protected:
		virtual eAssetState GetState() const override;

	private :
		wtr::DynamicArray<Memory::RefPtr<Asset>> meshs;
		wtr::DynamicArray<Memory::RefPtr<Asset>> materials;
		wtr::DynamicArray<Memory::RefPtr<Asset>> textures;
	};
};

#endif // __WTR_ASSETTYPES_H__