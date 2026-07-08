#ifndef __WTR_RHIUTILS_H__
#define __WTR_RHIUTILS_H__

#include <Container/include/DynamicArray.h>
#include <Memory/include/Pointer/RefPtr.h>

namespace wtr
{
	class RHIBuffer;
	class RHIShader;

	struct VertexKey;
	struct RHIShaderCreateDesc;
	struct RHIVertexLayoutCreateDesc;

	class RawData;

	enum class eRenderType : uint8_t;
};

namespace wtr
{
	namespace RHIUtils
	{
		void SetRenderType(const eRenderType renderType);

		void Build(const VertexKey& vertexKey, Memory::RefPtr<const RHIBuffer> buffer, RHIVertexLayoutCreateDesc& layoutDesc);

		DynamicArray<uint32_t> CompileShader(const RHIShaderCreateDesc& info);
		void ReflectShader(const DynamicArray<uint32_t>& shaderSource, Memory::RefPtr<RHIShader> shader);
	};
};

#endif // __WTR_RHIUTILS_H__