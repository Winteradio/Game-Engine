#ifndef __WTR_STBIIMAGE_H__
#define __WTR_STBIIMAGE_H__	

#include <cstdint>

namespace wtr
{
	class StbiImage
	{
	public :
		StbiImage();
		~StbiImage();

	public :
		bool LoadFromBuffer(const uint8_t* buffer, uint32_t size);

		const void* GetPixels() const;
		const uint32_t GetSize() const;
		const int32_t GetWidth() const;
		const int32_t GetHeight() const;
		const int32_t GetChannels() const;
		const bool Is16Bit() const;

	private :
		void* m_pixels;
		uint32_t m_size;
		int32_t m_width;
		int32_t m_height;
		int32_t m_channels;
		bool m_16bit;
	};
}

#endif // __WTR_STBIIMAGE_H__