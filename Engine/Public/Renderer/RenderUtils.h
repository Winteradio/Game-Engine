#ifndef __WTR_RENDERUTILS_H__
#define __WTR_RENDERUTILS_H__

#include <Renderer/RenderType.h>
#include <Framework/Math/MathTypes.h>
#include <Reflection/include/Utils.h>

namespace wtr
{
	struct VertexKey;
};

namespace wtr
{
	namespace RenderUtils
	{
		uint32_t GetComponentSize(const eDataType dataType);
		uint32_t GetPixelSize(const ePixelFormat pixelFormat);
		uint32_t GetPixelDimension(const ePixelFormat pixelFormat);

		int32_t GetVertexLocation(const VertexKey& vertexKey);
		VertexKey GetVertexKey(const std::string& attributeName);

		const std::string& GetSlotName(const eResourceSlot slot);

		uint32_t GetResourceBinding(const std::string& name);
	};

	namespace RenderUtils
	{
        template<typename T>
		constexpr eDataType GetDataType()
		{
			if constexpr (Reflection::Utils::IsSame<T, uint8_t>::value)
			{
				return eDataType::eUByte;
			}
			else if constexpr (Reflection::Utils::IsSame<T, int8_t>::value)
			{
				return eDataType::eByte;
			}
			else if constexpr (Reflection::Utils::IsSame<T, uint16_t>::value)
			{
				return eDataType::eUShort;
			}
			else if constexpr (Reflection::Utils::IsSame<T, int16_t>::value)
			{
				return eDataType::eShort;
			}
			else if constexpr (Reflection::Utils::IsSame<T, uint32_t>::value)
			{
				return eDataType::eUInt;
			}
			else if constexpr (Reflection::Utils::IsSame<T, int32_t>::value)
			{
				return eDataType::eInt;
			}
			else if constexpr (Reflection::Utils::IsSame<T, float>::value)
			{
				return eDataType::eFloat;
			}
			else if constexpr (Reflection::Utils::IsSame<T, double>::value)
			{
				return eDataType::eDouble;
			}
			else
			{
				return eDataType::eNone;
			}
		}

		template<typename T>
		struct ComponentInfo
		{
			constexpr static eDataType type = GetDataType<T>();
			constexpr static uint32_t count = 1;
			constexpr static uint32_t column = 1;
			constexpr static uint32_t row = 1;
			constexpr static uint32_t size = sizeof(T);
		};

		template<typename T, glm::length_t L, glm::qualifier Q>
		struct ComponentInfo<glm::vec<L, T, Q>>
		{
			constexpr static eDataType type = GetDataType<T>();
			constexpr static uint32_t count = L;
			constexpr static uint32_t column = 1;
			constexpr static uint32_t row = L;
			constexpr static uint32_t size = sizeof(T);
		};

		template<typename T, glm::length_t C, glm::length_t R, glm::qualifier Q>
		struct ComponentInfo<glm::mat<C, R, T, Q>>
		{
			constexpr static eDataType type = GetDataType<T>();
			constexpr static uint32_t count = C * R;
			constexpr static uint32_t column = C;
			constexpr static uint32_t row = R;
			constexpr static uint32_t size = sizeof(T);
		};

		template<typename T>
		constexpr eDataType GetComponentType()
		{
			return ComponentInfo<T>::type;
		}

		template<typename T>
		constexpr uint32_t GetComponentSize()
		{
			return ComponentInfo<T>::size;
		}

		template<typename T>
		constexpr uint32_t GetComponentAlign()
		{
			return ComponentInfo<T>::align;
		}

		template<typename T>
		constexpr uint32_t GetComponentCount()
		{
			return ComponentInfo<T>::count;
		}

		template<typename T>
		constexpr uint32_t GetColumnCount()
		{
			return ComponentInfo<T>::column;
		}

		template<typename T>
		constexpr uint32_t GetRowCount()
		{
			return ComponentInfo<T>::row;
		}
	};
};

#endif // __WTR_RENDERUTILS_H__
