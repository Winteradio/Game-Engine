#ifndef __WTR_RHIDESCRIPTIONS_H__
#define __WTR_RHIDESCRIPTIONS_H__

#include <Memory/include/Pointer/RefPtr.h>
#include <Container/include/DynamicArray.h>
#include <Renderer/RenderTypes.h>
#include <RHI/RHIStates.h>
#include <cstdint>

namespace wtr
{
	class RawBuffer;
	class FormattedBuffer;
	class RHIShader;
}

namespace wtr
{
	template<typename T>
	struct RHICreateDesc
	{
		Memory::RefPtr<T> rawBuffer;
	};

	struct RHIVertexAttribute
	{
		uint16_t location			= 0;	// vertex attribute binding location
		uint16_t offset				= 0;	// byte offset in stream
		bool  normalized 			= 1;	// normalize fixed-point data
		bool  integer   			= 0;	// treat as integer input
		uint8_t  divisor     		= 0;	// instancing divisor
		uint8_t  numComponents		= 0;	// number of components (1-4)
		eDataType  componentType 	= eDataType::eNone;			// enum: eDataType
		eVertexSemantic semantic 	= eVertexSemantic::eNone;	// bit value for the semantic (e.g. TEXCOORD)
		uint8_t  semanticIndex 		= 0;	// e.g. TEXCOORD0 -> 0, TEXCOORD1 -> 1
	};

	struct RHIResourceBinding
	{
		uint16_t location 	= 0; // resource binding location
		uint32_t count 		= 0; // number of resources in array
		eBindingType type 	= eBindingType::eNone; // type of the binding resource
	};

	template<enum class eResourceType Type>
	struct RHIDesc
	{
		const eResourceType type = Type;
	};

	struct RHIBufferDesc : RHIDesc<eResourceType::eBuffer>
	{
		eBufferType bufferType = eBufferType::eNone;
		eDataAccess	accessType = eDataAccess::eNone;

		uint32_t	size = 0;
		uint32_t	stride = 0;
	};

	struct RHIBufferCreateDesc : RHIBufferDesc, RHICreateDesc<FormattedBuffer>
	{
	};

	struct RHITextureDesc : RHIDesc<eResourceType::eTexture>
	{
		uint32_t		width = 0;
		uint32_t		height = 0;
		uint32_t		depth = 0;
		uint32_t		mipLevels = 1;
		uint32_t		sampleCount = 1;
		ePixelFormat	format = ePixelFormat::eNone;
		eTextureUsage 	usage = eTextureUsage::eNone;
	};

	struct RHITextureCreateDesc : RHITextureDesc, RHICreateDesc<FormattedBuffer>
	{
	};

	struct RHISamplerDesc : RHIDesc<eResourceType::eSampler>
	{
		eFilterMode		minFilter = eFilterMode::eNone;
		eFilterMode		magFilter = eFilterMode::eNone;
		eFilterMode		mipFilter = eFilterMode::eNone;
		eWrapMode		wrapS = eWrapMode::eNone;
		eWrapMode		wrapT = eWrapMode::eNone;
		eWrapMode		wrapR = eWrapMode::eNone;
	};

	struct RHISamplerCreateDesc : RHISamplerDesc
	{};

	template<enum class eShaderType Type>
	struct RHIShaderDesc : RHIDesc<eResourceType::eShader>
	{
		eShaderType shaderType = Type;
	};

	struct RHIVertexShaderDesc : RHIShaderDesc<eShaderType::eVertex>
	{
		static constexpr size_t MAX_ATTRIBUTES = 16;

		uint32_t numAttributes = 0;
		
		RHIVertexAttribute attributes[MAX_ATTRIBUTES];
	};

	struct RHIGeometryShaderDesc : RHIShaderDesc<eShaderType::eGeometry>
	{

	};

	struct RHIHullShaderDesc : RHIShaderDesc<eShaderType::eHull>
	{

	};

	struct RHIPixelShaderDesc : RHIShaderDesc<eShaderType::ePixel>
	{
	};

	struct RHIComputeShaderDesc : RHIShaderDesc<eShaderType::eCompute>
	{
	};

	struct RHIVertexShaderCreateDesc : RHIVertexShaderDesc, RHICreateDesc<RawBuffer>
	{
	};

	struct RHIGeometryShaderCreateDesc : RHIGeometryShaderDesc, RHICreateDesc<RawBuffer>
	{
	};

	struct RHIHullShaderCreateDesc : RHIHullShaderDesc , RHICreateDesc<RawBuffer>
	{
	};

	struct RHIPixelShaderCreateDesc : RHIPixelShaderDesc, RHICreateDesc<RawBuffer>
	{
	};

	struct RHIComputeShaderCreateDesc : RHIComputeShaderDesc, RHICreateDesc<RawBuffer>
	{
	};

	struct RHIPipeLineDesc : RHIDesc<eResourceType::ePipeLine>
	{
		RHIClearState		clear;
		RHIColorState		color;
		RHIDepthState 		depth;
		RHIStencilState		stencil;
		RHIBlendState		blend;
		RHIRasterizerState	rasterizer;
	};

	struct RHIPipeLineCreateDesc : RHIPipeLineDesc
	{
		Memory::RefPtr<RHIShader> vertexShader;
		Memory::RefPtr<RHIShader> geometryShader;
		Memory::RefPtr<RHIShader> hullShader;
		Memory::RefPtr<RHIShader> computeShader;
		Memory::RefPtr<RHIShader> pixelShader;
	};
};


#endif // __WTR_RHIDESCRIPTIONS_H__