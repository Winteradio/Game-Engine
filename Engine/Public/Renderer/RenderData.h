#ifndef __WTR_RENDERDATA_H__
#define __WTR_RENDERDATA_H__

#include <Renderer/RenderType.h>
#include <Renderer/RenderUtils.h>
#include <Container/include/DynamicArray.h>

namespace wtr
{
	class RawData
	{
	public:
		RawData() = default;
		virtual ~RawData() = default;

	public:
		virtual const void* GetPointer() const = 0;
		virtual uint32_t GetSize() const = 0;
		virtual uint32_t GetCount() const = 0;

		virtual uint32_t GetComponentSize() const = 0;
		virtual uint32_t GetComponentCount() const = 0;
		virtual uint32_t GetComponentColumn() const = 0;
		virtual uint32_t GetComponentRow() const = 0;
		virtual eDataType GetComponentType() const = 0;

	public :
		uint32_t GetStride() const
		{
			return GetComponentSize() * GetComponentCount();
		}

		bool Empty() const
		{
			return GetSize() == 0 || GetPointer() == nullptr;
		}
	};

	template<typename T>
	class ArrayData : public RawData
	{
	public:
		wtr::DynamicArray<T>	data;

	public:
		const void* GetPointer() const override
		{
			return data.Data();
		}

		uint32_t GetSize() const override
		{
			return GetStride() * GetCount();
		}

		uint32_t GetCount() const override
		{
			return static_cast<uint32_t>(data.Size());
		}

		uint32_t GetComponentSize() const override
		{
			return RenderUtils::GetComponentSize<T>();
		}

		uint32_t GetComponentCount() const override
		{
			return RenderUtils::GetComponentCount<T>();
		}

		uint32_t GetComponentColumn() const override
		{
			return RenderUtils::GetColumnCount<T>();
		}

		uint32_t GetComponentRow() const override
		{
			return RenderUtils::GetRowCount<T>();
		}

		eDataType GetComponentType() const override
		{
			return RenderUtils::GetComponentType<T>();
		}
	};

	template<typename T>
	class ScalarData : public RawData
	{
	public:
		T	data;

	public:
		const void* GetPointer() const override
		{
			return reinterpret_cast<const void*>(&data);
		}

		uint32_t GetSize() const override
		{
			return static_cast<uint32_t>(sizeof(T));
		}

		uint32_t GetCount() const override
		{
			return 1;
		}

		uint32_t GetComponentSize() const override
		{
			return RenderUtils::GetComponentSize<T>();
		}

		uint32_t GetComponentCount() const override
		{
			return RenderUtils::GetComponentCount<T>();
		}

		uint32_t GetComponentColumn() const override
		{
			return RenderUtils::GetColumnCount<T>();
		}

		uint32_t GetComponentRow() const override
		{
			return RenderUtils::GetRowCount<T>();
		}

		eDataType GetComponentType() const override
		{
			return RenderUtils::GetComponentType<T>();
		}
	};

	using RefData = Memory::RefPtr<RawData>;

	struct DataBlob
	{
		RefData		bulk;

		uint32_t	offset	= 0;
		uint32_t	size	= 0;
	};

	struct TextureBlob : DataBlob
	{
		uint32_t	level	= 0;
		uint32_t	layer	= 0;
		uint32_t	face	= 0;
	};

	struct BufferData
	{
		wtr::DynamicArray<DataBlob>	blobs;
	};

	struct TextureData
	{
		wtr::DynamicArray<TextureBlob>	blobs;

		uint32_t	width	= 1;
		uint32_t	height	= 1;
		uint32_t	depth	= 1;

		uint32_t	level	= 1;
		uint32_t	layer	= 1;
		uint32_t	face	= 1;
	};

	struct VertexKey
	{
		eVertexSemantic	semantic		= eVertexSemantic::eNone;
		uint8_t			semanticIndex	= 0xFF;

		bool operator==(const VertexKey& other) const
		{
			return semantic == other.semantic && semanticIndex == other.semanticIndex;
		}
	};
};

namespace std
{
	template<>
	struct hash<wtr::VertexKey>
	{
		size_t operator()(const wtr::VertexKey& key) const
		{
			const uint64_t semantic = static_cast<uint64_t>(key.semantic);
			const uint64_t semanticIndex = static_cast<uint64_t>(key.semanticIndex);

			return static_cast<size_t>((semanticIndex << 8) | semantic);
		}
	};
};

#endif // __WTR_RENDERDATA_H__