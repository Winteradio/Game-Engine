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
		eEnd,
	};

	enum class eAssetState : uint8_t
	{
		eNone		= 0x00,
		eError		= 0x01,
		eLoaded		= 0x10,
		eExpried	= 0x20
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

		uint32_t minVertexIndex = 0;
		uint32_t maxVertexIndex = 0;

		std::string materialName;
		std::string name;
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

		Asset();
		Asset(const std::string& path, const eExtension extension, const eAsset type);
		virtual ~Asset() = default;

	public :
		virtual eResourceState GetResourceState() const;

	public :
		void SetState(const eAssetState state);
		const eAssetState GetState() const;

	private :
		std::atomic<eAssetState> m_state;
	};

	class TextureAsset : public Asset
	{
		GENERATE(TextureAsset);
	public :
		Memory::RefPtr<TextureBuffer> rawBuffer;
		Memory::RefPtr<RHITexture> texture;

		uint32_t width;
		uint32_t height;
		uint32_t depth;
		uint32_t faces;
		uint32_t mipLevels;
		uint32_t sampleCount;
		ePixelFormat pixelFormat;

		bool isSRGB;
		bool isCubemap;
		bool isGenerateMips;

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
		wtr::HashMap<eTextureSlot, Memory::RefPtr<const TextureAsset>> textures;
		wtr::HashMap<eVectorSlot, fvec3> vectorValues;
		wtr::HashMap<eScalarSlot, float> scalarValues;

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
		wtr::HashMap<VertexKey, Memory::RefPtr<FormattedBuffer>> rawBuffers;
		wtr::HashMap<VertexKey, Memory::RefPtr<RHIBuffer>> buffers;
		Memory::RefPtr<FormattedBuffer> rawIndex;
		Memory::RefPtr<RHIBuffer> index;

		wtr::DynamicArray<MeshSection> sections;
		wtr::HashMap<std::string, Memory::RefPtr<const MaterialAsset>> materials;

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
		void SetShaderType(const eShaderType shaderType);
		eShaderType GetShaderType() const;
	private :
		eShaderType m_shaderType;
	};
};

#endif // __WTR_ASSETTYPES_H__