#ifndef __WTR_RHISTATETRACKER_H__
#define __WTR_RHISTATETRACKER_H__

#include <RHI/RHIStates.h>

#include <Memory/include/Pointer/ObjectPtr.h>
#include <Container/include/HashMap.h>
#include <map>

namespace wtr
{
	class RHIBuffer;
	class RHIBufferView;

	class RHITexture;
	class RHITextureView;
};

namespace wtr
{
	class RHIStateTracker
	{
	public:
		struct BufferState
		{
			RHIBufferState prev = {};
			RHIBufferState next = {};

			size_t offset = 0;
			size_t size = 0;
		};

		struct TextureRange
		{
			size_t baseLayer = 0;
			size_t layerCount = 0;
			size_t baseMip = 0;
			size_t mipCount = 0;

			bool operator==(const TextureRange& other) const noexcept
			{
				return baseLayer == other.baseLayer &&
					layerCount == other.layerCount &&
					baseMip == other.baseMip &&
					mipCount == other.mipCount;
			}

			bool operator<(const TextureRange& other) const noexcept
			{
				if (baseLayer != other.baseLayer)
				{
					return baseLayer < other.baseLayer;
				}

				if (baseMip != other.baseMip)
				{
					return baseMip < other.baseMip;
				}

				if (layerCount != other.layerCount)
				{
					return layerCount < other.layerCount;
				}

				return mipCount < other.mipCount;
			}
		};

		struct TextureState
		{
			RHITextureState prev = {};
			RHITextureState next = {};

			TextureRange range;
		};

		using BufferStateMap = std::map<size_t, BufferState>;
		using BufferMap = wtr::HashMap<Memory::RefPtr<const RHIBuffer>, BufferStateMap>;

		using TextureStateMap = std::map<TextureRange, TextureState>;
		using TextureMap = wtr::HashMap<Memory::RefPtr<const RHITexture>, TextureStateMap>;

	public :
		RHIStateTracker();
		~RHIStateTracker() = default;

	public :
		void AddState(Memory::RefPtr<const RHIBuffer> buffer, const BufferState& state);
		void AddState(Memory::RefPtr<const RHITexture> texture, const TextureState& state);

		void RemoveState(Memory::RefPtr<const RHIBuffer> buffer);
		void RemoveState(Memory::RefPtr<const RHITexture> texture);

		const BufferMap FlushBuffer();
		const TextureMap FlushTexture();

	private :
		void MergeBufferState();
		void MergeTextureState();

	private :
		BufferMap m_buffers;
		TextureMap m_textures;
	};
};

#endif // __WTR_RHISTATETRACKER_H__