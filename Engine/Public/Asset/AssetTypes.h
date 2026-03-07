#ifndef __WTR_ASSETTYPES_H__
#define __WTR_ASSETTYPES_H__

#include <Reflection/include/Type/TypeMacro.h>
#include <Memory/include/Pointer/RefPtr.h>
#include <Container/include/HashMap.h>
#include <ECS/include/UUID/UUID.h>
#include <Framework/Math/MathTypes.h>
#include <Renderer/RenderTypes.h>

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
		eMTL		= 0x07,
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
		eAll		= 0x0F,
		eError		= 0x10
	};

	eAssetState operator|(const eAssetState lhs, const eAssetState rhs);
	eAssetState operator&(const eAssetState lhs, const eAssetState rhs);
	eAssetState operator^(const eAssetState lhs, const eAssetState rhs);
	eAssetState operator~(const eAssetState state);
	eAssetState operator|=(eAssetState lhs, const eAssetState rhs);
	eAssetState operator&=(eAssetState lhs, const eAssetState rhs);
	eAssetState operator^=(eAssetState lhs, const eAssetState rhs);

	struct MeshSection
	{
		uint32_t indexOffset = 0;
		uint32_t indexCount = 0;

		uint32_t vertexOffset = 0;
		uint32_t minVertexIndex = 0;
		uint32_t maxVertexIndex = 0;

		uint64_t materialIndex = 0;

		std::string name;
	};

	struct VertexKey
	{
		eVertexSemantic semantic;
		uint8_t semanticIndex;

		operator uint64_t() const
		{
			return static_cast<uint64_t>(semantic) << 32 | static_cast<uint64_t>(semanticIndex);
		}
	};

	struct RawBuffer
	{
		wtr::DynamicArray<uint8_t> data;
	};

	class Asset
	{
		GENERATE(Asset);
	public :
		ECS::UUID id;
		std::string name;
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
		virtual eResourceState GetResourceState() const;
	};

	class TextureAsset : public Asset
	{
	public :
		Memory::RefPtr<RawBuffer> rawBuffer;
		Memory::RefPtr<RHITexture> texture;

		TextureAsset();
		TextureAsset(const std::string& path, const eExtension extension);
		virtual ~TextureAsset() = default;

	public:
		virtual eResourceState GetResourceState() const override;
	};

	class MaterialAsset : public Asset
	{
		GENERATE(MaterialAsset);
	public:
		Memory::RefPtr<TextureAsset> baseColor;
		Memory::RefPtr<TextureAsset> specularMap;
		Memory::RefPtr<TextureAsset> normalMap;

		fvec3 ambientColor;
		fvec3 diffuseColor;
		fvec3 specularColor;
		float shininess;
		float opacity;

		MaterialAsset();
		MaterialAsset(const std::string& path, const eExtension extension);
		virtual ~MaterialAsset() = default;

	public:
		virtual eResourceState GetResourceState() const override;
	};

	class MeshAsset : public Asset
	{
		GENERATE(MeshAsset);
	public:
		wtr::HashMap<VertexKey, Memory::RefPtr<RawBuffer>> rawBuffers;
		wtr::HashMap<VertexKey, Memory::RefPtr<RHIBuffer>> buffers;
		Memory::RefPtr<RawBuffer> rawIndex;
		Memory::RefPtr<RHIBuffer> index;

		wtr::DynamicArray<MeshSection> sections;
		wtr::DynamicArray<Memory::RefPtr<MaterialAsset>> materials;

		MeshAsset();
		MeshAsset(const std::string& path, const eExtension extension);
		virtual ~MeshAsset() = default;

	public:
		virtual eResourceState GetResourceState() const override;
	};

	class ShaderAsset : public Asset
	{
		GENERATE(ShaderAsset);
	public :
		Memory::RefPtr<RawBuffer> rawBuffer;
		Memory::RefPtr<RHIShader> shader;

		ShaderAsset();
		ShaderAsset(const std::string& path, const eExtension extension);
		virtual ~ShaderAsset() = default;

	public:
		virtual eResourceState GetResourceState() const override;
	};

	class ComposeAsset : public Asset
	{
		GENERATE(ComposeAsset);
	public :
		wtr::DynamicArray<Memory::RefPtr<MeshAsset>> meshs;
		wtr::DynamicArray<Memory::RefPtr<MaterialAsset>> materials;
		wtr::DynamicArray<Memory::RefPtr<TextureAsset>> textures;

		ComposeAsset();
		ComposeAsset(const std::string& path, const eExtension extension);
		virtual ~ComposeAsset() = default;

	public:
		virtual eResourceState GetResourceState() const override;
	};
};

#endif // __WTR_ASSETTYPES_H__