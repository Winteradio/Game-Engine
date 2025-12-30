#ifndef __WTR_RHITYPES_H__
#define __WTR_RHITYPES_H__

#include <cstdint>

namespace wtr
{
	enum class eRHIType : uint8_t
	{
		eNone = 0,
		eOpenGL = 1,

		// Not Implemented
		eDirectX11 = 2,
		eDirectX12 = 3,
		eVulkan = 4,
		eEnd,
	};

	enum class eRHIResourceType : uint8_t
	{
		eNone = 0,
		eEnd
	};

	enum class eRHIBufferUsage : uint8_t
	{
		eNone = 0,
		eStatic = 1,
		eDynamic = 2,
		eStream = 3,
		eStaging = 4,
		eEnd
	};

	enum class eRHIPixelFormat : uint8_t
	{
		eNone = 0,
		eR8G8B
	};

	enum class eRHIShaderType : uint8_t
	{
		eNone = 0,
		eVertex = 1,
		eGeometry = 2,
		eHull = 3,
		eFragment = 4,
		eCompute = 5,
		eEnd,
	};

	enum class eRHICullMode : uint8_t
	{
		eNone = 0,
		eCW = 1,
		eCCW = 2,
		eEnd,
	};

	enum class eRHIClearBuffer : uint8_t
	{
		eNone = 0,
		eColor = 1,
		eDepth = 2,
		eStencil = 3,
		eEnd,
	};
};

#endif // __WTR_RHITYPES_H__N